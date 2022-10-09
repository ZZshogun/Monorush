#ifndef PLAYER_H

#include "ScriptableEntity.h"

class PlayerController : public ScriptableEntity {
public:

	float playerSpeed = 4;

	void OnUpdate(Time time){
		glm::vec2 dir = Input::WASDAxis();
		dir *= playerSpeed * time.deltaTime;
		if (dir.x && dir.y) dir /= glm::sqrt(2);
		GetComponent<TransformComponent>().position += glm::vec3(dir, 0);
	}

};

#endif