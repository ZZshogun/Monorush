#ifndef PLAYER_H

#include "ScriptableEntity.h"

class PlayerController : public ScriptableEntity {
public:

	float playerSpeed = 4;
	RigidbodyComponent* rigidbody = NULL;
	AnimatorComponent* animator = NULL;
	AudioSourceComponent* audio = NULL;

	const int maxPlayerHealth = 5;
	int playerHeath = maxPlayerHealth;
	int cbullet = 12;
	int fullammo = 12;

	float bulletpersec = 6;
	float cdtime = 0;

	std::vector<glm::vec4> heartsCol;
	bool hurt = false;
	bool heal = false;
	float blink = 0.1f, blinktime = 0;

	Ref<UI::Button> hurtButton;
	Ref<UI::Button> healButton;

	void Hurt() {
		playerHeath = glm::clamp<int>(playerHeath - 1, 0, maxPlayerHealth);
		heartsCol[(size_t)playerHeath] = glm::vec4{1, 0.11f, 0.28f, 1};
		hurt = true;
	}

	void Heal() {
		playerHeath = glm::clamp<int>(playerHeath + 1, 0, maxPlayerHealth);
		heartsCol[(size_t)playerHeath - 1] = glm::vec4{ 0.11f, 1, 0.28f, 1 };
		heal = true;
	}

	void OnCreate() {
		rigidbody = &GetComponent<RigidbodyComponent>();
		animator = &GetComponent<AnimatorComponent>();
		audio = &GetComponent<AudioSourceComponent>();

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
		glm::vec2 dir = Input::WASDAxis();
		dir *= playerSpeed;
		if (dir.x && dir.y) dir /= glm::sqrt(2);
		rigidbody->velocity = glm::vec3(dir, 0);

		cdtime += time.deltaTime;

		if (cdtime >= 1.0f / bulletpersec && Input::GetMouseDown(Button::Mouse_Left)) {
			cdtime = 0;
			audio->Play(Audio::AudioBuffers["bounce"]);
			cbullet = glm::clamp<int>(cbullet - 1, 0, fullammo);
		}

		if (dir.x < 0) {
			animator->current_id = 2;
		}
		else if (dir.x > 0) {
			animator->current_id = 1;
		}

		if (hurt) {
			blinktime += time.deltaTime;
			if (blinktime >= blink) {
				blinktime = 0;
				hurt = false;
				heartsCol[(size_t)playerHeath] = {0, 0, 0 , 0.25f};
			}
		}
		if (heal) {
			blinktime += time.deltaTime;
			if (blinktime >= blink) {
				blinktime = 0;
				heal = false;
				heartsCol[(size_t)playerHeath - 1] = { 0, 0, 0 , 1 };
			}
		}
	}

	void OnDrawUI(Time time) {
		UI::StartUI(glm::ivec2{ 1920, 1080 });

		UI::Anchor(CENTER);
		for (int i = 0; i < maxPlayerHealth; i++) {
			UI::DrawImage(Texture::library["heart"], { -910 + i * 70, -490 }, { 70, 70 }, heartsCol[i]);
		}

		std::string bullet = std::to_string(cbullet) + " | " + std::to_string(fullammo);
		UI::Anchor(RIGHT);
		UI::DrawString(bullet, { 940, -500 }, 1.25f, { 0, 0, 0, 1 });

		UI::Anchor(CENTER);
		UI::DrawButton(hurtButton, time);
		UI::DrawButton(healButton, time);

		UI::EndUI();
	}

};

#endif