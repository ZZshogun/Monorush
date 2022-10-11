#ifndef PLAYER_H

#include "ScriptableEntity.h"

class PlayerController : public ScriptableEntity {
public:

	float playerSpeed = 4;

	void OnUpdate(Time time){
		glm::vec2 dir = Input::WASDAxis();
		dir *= playerSpeed;
		if (dir.x && dir.y) dir /= glm::sqrt(2);
		GetComponent<RigidbodyComponent>().velocity = glm::vec3(dir, 0);
		auto& pos = GetComponent<TransformComponent>().position;
	}

};

#endif