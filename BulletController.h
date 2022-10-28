#ifndef BULLET_CONTROLLER_H
#define BULLET_CONTROLLER_H

#include "ScriptableEntity.h"
#include "EnemyController.h"

class BulletController : public ScriptableEntity {
public:
	bool despawned = false;
	float despawnTime = 5;
	float _counter = 0;
	int damage = 0;

	void OnUpdate(Time time) {
		if (!despawned) {
			if (_counter >= despawnTime) {
				despawned = true;
				entity.Destroy();
				return;
			}
			else _counter += time.deltaTime;
		}
	}

	void OnCollision(CollisionPacket& packet, Time time) {
		for (auto& box : packet.boxes) {
			if (box.entity.GetComponent<TagComponent>().tag == "Enemy") {
				box.entity.GetScript<EnemyController>().Hurt(damage);
				break;
			}
		}
		entity.Destroy();
	}
};


#endif