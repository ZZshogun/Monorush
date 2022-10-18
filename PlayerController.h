#ifndef PLAYER_H

#include "ScriptableEntity.h"

class PlayerController : public ScriptableEntity {
public:

	float playerSpeed = 4;
	RigidbodyComponent* rigidbody = NULL;
	AnimatorComponent* animator = NULL;
	AudioSourceComponent* audio = NULL;

	short playerHeath = 5;
	short cbullet = 12;
	short fullammo = 12;

	float bulletpersec = 6;
	float cdtime = 0;

	glm::vec4 heartCol = { 0, 0, 0 ,1 };
	bool hurt = false;
	float blink = 0.1f, blinktime = 0;

	void OnCreate() {
		rigidbody = &GetComponent<RigidbodyComponent>();
		animator = &GetComponent<AnimatorComponent>();
		audio = &GetComponent<AudioSourceComponent>();
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
			cbullet = glm::clamp<short>(cbullet - 1, 0, fullammo);
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
				heartCol = { 0, 0, 0 ,1 };
			}
		}

		if (Input::GetKeyDown(Key::H)) {
			playerHeath--;
			heartCol = glm::vec4{ 1, 0.11f, 0.28f, 1 };
			hurt = true;
		}
	}

	void OnDrawUI(Time time) {
		UI::StartUI(glm::ivec2{ 1920, 1080 });

		UI::Anchor(CENTER);
		for (int i = 0; i < playerHeath; i++) {
			UI::DrawImage(Texture::library["heart"], { -910 + i * 70, -490 }, { 70, 70 }, heartCol);
		}

		std::string bullet = std::to_string(cbullet) + " | " + std::to_string(fullammo);
		UI::Anchor(RIGHT);
		UI::DrawString(bullet, { 940, -500 }, 1.25f, { 0, 0, 0, 1 });

		UI::EndUI();
	}

};

#endif