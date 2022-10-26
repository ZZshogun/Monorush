#ifndef PLAYER_H
#define PLAYER_H

#include "ScriptableEntity.h"
#include "GameManager.h"

class PlayerController : public ScriptableEntity {

public:
	float playerSpeed = 4;
	int maxPlayerHealth = 5;
	bool death = false;

private:
	TransformComponent* transform = NULL;
	RigidbodyComponent* rigidbody = NULL;
	AnimatorComponent* animator = NULL;
	AudioSourceComponent* audio = NULL;
	CollisionComponent* collider = NULL;
	SpriteRendererComponent* sprite = NULL;

	glm::vec2 col_idle_size = { 0.35f, 0.9f };
	glm::vec2 col_idle_offset = { 0, 0 };
	glm::vec2 col_run_size = { 0.35f, 0.7f };
	glm::vec2 col_run_offset = { 0, -0.12f };

	int playerHeath = maxPlayerHealth;
	int cbullet = 12;
	const int fullammo = 12;

	bool reloading = false;
	const float reloadTime = 1.5f;
	float _reload = 0;
	float bulletpersec = 6;
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

	bool DeathStatus() {
		death = playerHeath <= 0;
		return death;
	}

public:

	void Hurt() {
		if (playerHeath <= 0) return;
		playerHeath = glm::clamp<int>(playerHeath - 1, 0, maxPlayerHealth);
		heartsCol[(size_t)playerHeath] = glm::vec4{1, 0.11f, 0.28f, 1};
		hurt = true;
		playerOpacity = 0.6f;
		sprite->Color({ 1, 0.41f, 0.38f, playerOpacity });
		invincible = true;
	}

	void Heal() {
		if (playerHeath >= maxPlayerHealth) return;
		playerHeath = glm::clamp<int>(playerHeath + 1, 0, maxPlayerHealth);
		heartsCol[(size_t)playerHeath - 1] = glm::vec4{ 0.11f, 1, 0.28f, 1 };
		sprite->Color({ 0.12f, 1, 0.12f, playerOpacity });
		heal = true;
	}

	void OnCreate() {
		transform = &GetComponent<TransformComponent>();
		rigidbody = &GetComponent<RigidbodyComponent>();
		animator = &GetComponent<AnimatorComponent>();
		audio = &GetComponent<AudioSourceComponent>();
		collider = &GetComponent<CollisionComponent>();
		sprite = &GetComponent<SpriteRendererComponent>();

		for (int i = 0; i < maxPlayerHealth; i++) heartsCol.emplace_back(0, 0, 0, 1);

		UI::StartUI(glm::ivec2{ 1920, 1080 });
		hurtButton = UI::CreateButton(
			"HURT",
			1,
			{ 1, 1, 1, 1 },
			{ -150, -450 },
			{ 200, 100 },
			{ 0, 0, 0, 1 },
			[&]() { Hurt(); },
			{ 0.3f, 0.3f, 0.3f, 1 }
		);
		healButton = UI::CreateButton(
			"HEAL",
			1,
			{ 1, 1, 1, 1 },
			{ 150, -450 },
			{ 200, 100 },
			{ 0, 0, 0, 1 },
			[&]() { Heal(); },
			{ 0.3f, 0.3f, 0.3f, 1 }
		);
		UI::EndUI();
	}

	void OnUpdate(Time time){

		// Invincibility timeout
		if (invincible) {
			invCountTime += time.deltaTime;
			if (invCountTime >= invTime) {
				invincible = false;
				invCountTime = 0;
				playerOpacity = 1;
				sprite->Color(Color::White);
			}
		}
		// Hurt
		if (hurt) {
			blinktime += time.deltaTime;
			if (blinktime >= blink) {
				blinktime = 0;
				hurt = false;
				heartsCol[(size_t)playerHeath] = { 0, 0, 0 , 0.25f };
				sprite->Color({ 1, 1, 1, playerOpacity });
			}
		}
		// Heal
		if (heal) {
			blinktime += time.deltaTime;
			if (blinktime >= blink) {
				blinktime = 0;
				heal = false;
				heartsCol[(size_t)playerHeath - 1] = Color::Black;
				sprite->Color({ 1, 1, 1, playerOpacity });
			}
		}

		// Death status
		if (death) return;
		if (DeathStatus()) {
			rigidbody->velocity = { 0, 0, 0 };
			rigidbody->isStatic = true;
			hurtButton->active = false;
			healButton->active = false;
			animator->SetAnimationIndex(1);
			collider->active = false;
			GameManager::gameOver = true;
			return;
		}

		// Movement
		glm::vec2 dir = Input::WASDAxis() * playerSpeed;
		if (dir.x && dir.y) dir /= glm::sqrt(2);
		rigidbody->velocity = glm::vec3(dir, 0);

		// Ammo & Reload
		cdtime += time.deltaTime;
		if (!reloading) {
			if (cdtime >= 1.0f / bulletpersec && Input::GetMouseDown(Button::Mouse_Left)) {
				cdtime = 0;
				audio->Play(Audio::AudioBuffers["bounce"]);
				cbullet--;
				if (cbullet < 0) {
					cbullet = 0;
					reloading = true;
					gunstate = "RELOADING";
				}
				else
					gunstate = std::to_string(cbullet) + " | " + std::to_string(fullammo);
			}

			if (cbullet < fullammo && Input::GetKeyDown(Key::R)) {
				reloading = true;
				gunstate = "RELOADING";
			}
		}
		else {
			_reload += time.deltaTime;
			if (_reload >= reloadTime) {
				reloading = false;
				cbullet = fullammo;
				gunstate = std::to_string(cbullet) + " | " + std::to_string(fullammo);
				_reload = 0;
			}
		}

		// Animation
		if (glm::length(dir) > 0) {
			animator->SetAnimationIndex(2);
			collider->Size(col_run_size);
			collider->Origin(col_run_offset);
		}
		else {
			animator->SetAnimationIndex(1);
			collider->Origin(col_idle_offset);
		}

		// Move direction on x axis
		if (dir.x < 0) transform->scale.x = -1;
		else if (dir.x > 0) transform->scale.x = 1;
	}

	void OnDrawUI(Time time) {
		UI::StartUI(glm::ivec2{ 1920, 1080 });

		UI::Anchor(CENTER);
		for (int i = 0; i < maxPlayerHealth; i++) {
			UI::DrawImage(Texture::library["heart"], { -910 + i * 70, -490 }, { 70, 70 }, heartsCol[i]);
		}

		UI::Anchor(RIGHT);
		UI::DrawString(gunstate, { 940, -500 }, 1.25f, { 0, 0, 0, 1 });

		std::stringstream position;
		position << std::setprecision(2) << std::fixed << "X:" << transform->position.x << " Y:" << transform->position.y;
		UI::DrawString(position.str(), {940, 500}, 0.65f, {0, 0, 0, 1});

		UI::Anchor(CENTER);
		UI::DrawButton(hurtButton, time);
		UI::DrawButton(healButton, time);

		UI::EndUI();
	}
};

#endif