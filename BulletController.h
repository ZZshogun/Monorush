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
	float speed = 0;

	void OnUpdate(Time time) {
		if (!despawned) {
			GetComponent<TransformComponent>().rotation.z -= speed * time.deltaTime;
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
				auto& enemy_rb = box.entity.GetComponent<RigidbodyComponent>();
				auto& bullet_rb = GetComponent<RigidbodyComponent>();
				float totalMass = enemy_rb.mass + bullet_rb.mass;
				enemy_rb.velocity += bullet_rb.velocity * (bullet_rb.mass / totalMass);
				break;
			}
		}
		entity.Destroy();
	}
};


#endif