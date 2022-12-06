#ifndef PLAYER_H
#define PLAYER_H

#include "../header/ScriptableEntity.h"
#include "GameManager.h"
#include "BulletController.h"

class PlayerController : public ScriptableEntity {

public:
	float playerSpeed = 4;
	int maxPlayerHealth = 5;
	int playerHeath = maxPlayerHealth;
	float bulletpersec = 3;
	float shootPower = 12;
	int bulletDamage = 15;
	float bulletImpact = 2.4f;

private:
	glm::vec2 col_idle_size = { 0.35f, 0.92f };
	glm::vec2 col_idle_offset = { 0, 0 };
	glm::vec2 col_run_size = { 0.35f, 0.7f };
	glm::vec2 col_run_offset = { 0, -0.12f };

	const glm::vec4 normalCapeColor = {0.45f, 0.64f, 0.85f, 1};
	glm::vec4 capeColor = { 0.45f, 0.64f, 0.85f, 1 };
	glm::vec4 deltaAdditionToGrayscale = { 0, 0, 0, 0 };

	int cbullet = 12;
	const int fullammo = 12;
	int shotFired = 0;

	bool reloading = false;
	const float reloadTime = 1.2f;
	float _reload = 0;
	float cdtime = 0;
	std::string gunstate = std::to_string(cbullet) + " | " + std::to_string(fullammo);

	std::vector<glm::vec4> heartsCol;
	bool invincible = false;
	bool hurt = false;
	bool heal = false;
	float blink = 0.1f, blinktime = 0;
	float invTime = 1.3f, invCountTime = 0;
	float playerOpacity = 1;

	Ref<UI::Button> hurtButton;
	Ref<UI::Button> healButton;

	Entity cape;

public:

	void Hurt(SpriteRendererComponent& sprite, CollisionComponent& collider,
		SpriteRendererComponent& cape
	) {
		if (heal || playerHeath <= 0 || invincible) return;
		playerSpeed = 5;
		heartsCol[(size_t)playerHeath] = glm::vec4{1, 0.11f, 0.28f, 1};
		playerHeath = glm::clamp<int>(playerHeath - 1, 0, maxPlayerHealth);
		playerOpacity = 0.6f;
		sprite.Color({ 1, 0.41f, 0.38f, playerOpacity });
		SetCapeColor();
		cape.Color({ glm::vec3(capeColor), playerOpacity });
		collider.ignoredTags.insert("Enemy");
		invincible = true;
		hurt = true;
	}

	void Heal(SpriteRendererComponent& sprite, SpriteRendererComponent& cape) {
		if (hurt || playerHeath >= maxPlayerHealth) return;
		playerHeath = glm::clamp<int>(playerHeath + 1, 0, maxPlayerHealth);
		heartsCol[(size_t)playerHeath] = glm::vec4{ 0.11f, 1, 0.28f, 1 };
		sprite.Color({ 0.12f, 1, 0.12f, playerOpacity });
		SetCapeColor();
		cape.Color({ glm::vec3(capeColor), playerOpacity });
		heal = true;
	}

	void Freeze(RigidbodyComponent&  rigidbody, CollisionComponent& collider, AnimatorComponent& animator,
		AnimatorComponent& capeAnimator
		) {
		rigidbody.isStatic = true;
		hurtButton->active = false;
		healButton->active = false;
		collider.active = false;
		rigidbody.velocity = { 0, 0, 0 };
		animator.SetAnimationIndex(1);
		capeAnimator.SetAnimationIndex(1);
	}

	void Shoot(TransformComponent& transform) {
		glm::vec2 direction = glm::normalize(Input::MousePosition());

		Entity bullet = Instantiate();
		auto& renderer = bullet.AddComponent<SpriteRendererComponent>();
		renderer.SetTexture(Texture::library["bullet"]);
		renderer.order = 999;
		bullet.GetComponent<TagComponent>().name = "Bullet" + std::to_string(++shotFired);
		bullet.GetComponent<TagComponent>().tag = "Bullet";
		bullet.GetComponent<TransformComponent>().position = transform.position + glm::vec3(direction * 0.15f, 0);
		auto& rb = bullet.AddComponent<RigidbodyComponent>();
		rb.mass = bulletImpact;
		rb.velocity = glm::vec3(direction, 0) * shootPower;
		auto& colld = bullet.AddComponent<CollisionComponent>();
		colld.ignoredTags.insert("Player");
		colld.ignoredTags.insert("Lamp");
		colld.ignoredTags.insert("Bullet");
		colld.Size({0.2f, 0.2f});
		bullet.AddComponent<NativeScriptComponent>().Bind<BulletController>();
		auto& bCont = bullet.GetScript<BulletController>();
		bCont.damage = bulletDamage;
		bCont.speed = shootPower * 100.0f;
	}

	void SetCapeColor() {
		capeColor = Color::White - (float)playerHeath * deltaAdditionToGrayscale;
	}

	void OnCreate() {
		for (int i = 0; i <= maxPlayerHealth; i++) heartsCol.push_back(normalCapeColor);

		cape = FindEntityOfName("Player cape");

		auto& sprite = GetComponent<SpriteRendererComponent>();
		auto& capeSprite = cape.GetComponent<SpriteRendererComponent>();
		auto& collider = GetComponent<CollisionComponent>();

		deltaAdditionToGrayscale = (Color::White - normalCapeColor) / (float)maxPlayerHealth;
		SetCapeColor();
		capeSprite.Color(capeColor);

		UI::StartUI(glm::ivec2{ 1920, 1080 });
		hurtButton = UI::CreateButton(
			"HURT",
			1,
			Color::White,
			{ -150, -450 },
			{ 200, 100 },
			Color::Black,
			[&]() { Hurt(sprite, collider, capeSprite); },
			{ 0.3f, 0.3f, 0.3f, 1 }
		);
		healButton = UI::CreateButton(
			"HEAL",
			1,
			Color::White,
			{ 150, -450 },
			{ 200, 100 },
			Color::Black,
			[&]() { Heal(sprite, capeSprite); },
			{ 0.3f, 0.3f, 0.3f, 1 }
		);
		UI::EndUI();
	}

	void OnUpdate(Time time){
		if (GameManager::pause) return;

		auto& transform = GetComponent<TransformComponent>();
		auto& rigidbody = GetComponent<RigidbodyComponent>();
		auto& animator = GetComponent<AnimatorComponent>();
		auto& audio = GetComponent<AudioSourceComponent>();
		auto& collider = GetComponent<CollisionComponent>();
		auto& sprite = GetComponent<SpriteRendererComponent>();

		auto& cape_sprite = cape.GetComponent<SpriteRendererComponent>();
		auto& cape_animator = cape.GetComponent<AnimatorComponent>();

		// Invincibility timeout
		if (invincible) {
			invCountTime += time.deltaTime;
			if (invCountTime >= invTime + 0.7f) {
				invincible = false;
				collider.ignoredTags.erase("Enemy");
				playerSpeed = 4;
				invCountTime = 0;
			}
			else if (invCountTime >= invTime) {
				playerOpacity = 1;
				sprite.Color(Color::White);
				cape_sprite.Color(capeColor);
			}
		}
		// Hurt
		if (hurt) {
			blinktime += time.deltaTime;
			if (blinktime >= blink) {
				blinktime = 0;
				hurt = false;
				heartsCol[(size_t)playerHeath + 1] = 
					{ glm::vec3(Color::White - ((float)(maxPlayerHealth - playerHeath + 1) * deltaAdditionToGrayscale)), 0.25f};
				sprite.Color({ 1, 1, 1, playerOpacity });
				cape_sprite.Color({ glm::vec3(capeColor), playerOpacity });
			}
		}
		// Heal
		if (heal) {
			blinktime += time.deltaTime;
			if (blinktime >= blink) {
				blinktime = 0;
				heal = false;
				heartsCol[(size_t)playerHeath] = capeColor;
				sprite.Color({ 1, 1, 1, playerOpacity });
				cape_sprite.Color({ glm::vec3(capeColor), playerOpacity });
			}
		}

		// Death status
		if (GameManager::gameOver) {
			Freeze(rigidbody, collider, animator, cape_animator);
			return;
		}
		if (playerHeath <= 0) {
			GameManager::gameOver = true;
			GameManager::gameState = GameManager::LOSS;
			hurtButton->active = false;
			healButton->active = false;
			return;
		}

		// Movement
		glm::vec2 dir = Input::WASDAxis() * playerSpeed;
		if (dir.x && dir.y) dir /= glm::sqrt(2);
		rigidbody.velocity = glm::vec3(dir, 0);

		// Ammo & Reload
		cdtime += time.deltaTime;
		if (!reloading) {
			if (cdtime >= 1.0f / bulletpersec && Input::GetMouse(Button::Mouse_Left)) {
				cdtime = 0;
				if (cbullet <= 0) {
					cbullet = 0;
					reloading = true;
				}
				else {
					cbullet--;
					Shoot(transform);
				}
			}

			if (cbullet < fullammo && Input::GetKeyDown(Key::R))
				reloading = true;
		}
		else {
			_reload += time.deltaTime;
			if (_reload >= reloadTime) {
				reloading = false;
				cbullet = fullammo;
				_reload = 0;
			}
		}

		if (reloading) gunstate = "RELOADING";
		else gunstate = std::to_string(cbullet) + " | " + std::to_string(fullammo);

		// Animation
		if (glm::length(dir) > 0) {
			animator.SetAnimationIndex(2);
			cape_animator.SetAnimationIndex(2);
			collider.Size(col_run_size);
			collider.Origin(col_run_offset);
		}
		else {
			animator.SetAnimationIndex(1);
			cape_animator.SetAnimationIndex(1);
			collider.Origin(col_idle_offset);
			collider.Size(col_idle_size);
		}

		// Move direction on x axis
		if (dir.x < 0) {
			sprite.flipX = true;
			cape_sprite.flipX = true;
		}
		else if (dir.x > 0) {
			sprite.flipX = false;
			cape_sprite.flipX = false;
		}

		if (Input::GetKeyDown(Key::Y)) Time::timeScale = 30;
		else if (Input::GetKeyUp(Key::Y)) Time::timeScale = 1;
	}

	void OnDrawUI(Time time) {
		auto& transform = GetComponent<TransformComponent>();

		UI::StartUI(glm::ivec2{ 1920, 1080 });

		UI::Anchor(CENTER);
		for (int i = 1; i <= maxPlayerHealth; i++) {
			int pos_x = (i - 1) * 70 - 910;
			int pos_y = -490;
			UI::DrawImage(Texture::library["heart"], {pos_x, pos_y}, {70, 70}, heartsCol[i]);
		}

		UI::Anchor(RIGHT);
		UI::DrawString(gunstate, { 940, -500 }, 1.25f, { 0, 0, 0, 1 });

		std::stringstream position;
		position << std::setprecision(2) << std::fixed << "X:" << transform.position.x << " Y:" << transform.position.y;
		UI::DrawString(position.str(), {940, 500}, 0.65f, {0, 0, 0, 1});

#ifdef MAGIA_DEBUG
		UI::Anchor(CENTER);
		UI::DrawButton(hurtButton, time);
		UI::DrawButton(healButton, time);
#endif

		UI::EndUI();
	}

	void OnCollision(CollisionPacket& packet, Time time) {
		for (auto& box : packet.boxes) {
			if (box.entity.GetComponent<TagComponent>().tag == "Enemy") {
				Hurt(GetComponent<SpriteRendererComponent>(), GetComponent<CollisionComponent>(),
					cape.GetComponent<SpriteRendererComponent>());
				break;
			}
		}
	}
};

#endif