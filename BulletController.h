#ifndef BULLET_CONTROLLER_H
#define BULLET_CONTROLLER_H

#include "ScriptableEntity.h"

class BulletController : public ScriptableEntity {

public:
	bool despawned = false;
	float despawnTime = 5;
	float _counter = 0;

	void OnUpdate(Time time) {
		if (!despawned) {
			if (_counter >= despawnTime) {
				despawned = true;
				std::cout << "Bullet Destroyed\n";
				entity.Destroy();
			}
			else _counter += time.deltaTime;
		}
	}

};


#endif