#ifndef PLAYER_H

#include "ScriptableEntity.h"

class PlayerController : public ScriptableEntity {
public:

	float playerSpeed = 4;
	RigidbodyComponent* rigidbody = NULL;
	AnimatorComponent* animator = NULL;

	void OnCreate() {
		rigidbody = &GetComponent<RigidbodyComponent>();
		animator = &GetComponent<AnimatorComponent>();
	}

	void OnUpdate(Time time){
		glm::vec2 dir = Input::WASDAxis();
		dir *= playerSpeed;
		if (dir.x && dir.y) dir /= glm::sqrt(2);
		rigidbody->velocity = glm::vec3(dir, 0);

		if (dir.x < 0) {
			animator->current_id = 2;
		}
		else if (dir.x > 0) {
			animator->current_id = 1;
		}
	}

};

#endif