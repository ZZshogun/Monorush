#ifndef PLAYER_H

#include "ScriptableEntity.h"

class PlayerController : public ScriptableEntity {
public:

	float playerSpeed = 4;
	RigidbodyComponent* rigidbody = NULL;
	AnimatorComponent* animator = NULL;
	AudioSourceComponent* audio = NULL;

	uint16_t playerHeath = 6;

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

		if (Input::GetMouseDown(GLFW_MOUSE_BUTTON_LEFT)) {
			audio->Play(Audio::AudioBuffers["bounce"]);
		}

		if (dir.x < 0) {
			animator->current_id = 2;
		}
		else if (dir.x > 0) {
			animator->current_id = 1;
		}
	}

	void OnDrawUI(Time time) {
		UI::StartUI(glm::ivec2{ 1920, 1080 });

		UI::DrawString(std::to_string(playerHeath), { -1220, -1020 }, 1.25f, { 0, 0, 0, 1 });

		UI::EndUI();
	}

};

#endif