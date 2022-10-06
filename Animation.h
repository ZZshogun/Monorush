#ifndef ANIMATION_CLASS_H
#define ANIMATION_CLASS_H

#include "Core.h"
#include "SpriteSheet.h"

class AnimatedSprite {

	std::vector<Ref<SpriteSheet>> spriteSheets;
	std::vector<glm::vec2> currentIndexes;
	float elapsed = 0;
	float timeStep = -1;

public:

	Transform transform;

	AnimatedSprite(float timeStep = 100);
	static Ref<AnimatedSprite> Create(float timeStep = 100);
	void SetTimeStep(float timeStep);
	void Add(Ref<SpriteSheet>& animationSheet);
	void Draw(int animationIndex, float deltaTime);
};

#endif
