#include "Animation.h"

AnimatedSprite::AnimatedSprite(float timeStep) {
	this->timeStep = timeStep / 1000.0f;
}

void AnimatedSprite::SetTimeStep(float timeStep) {
	this->timeStep = timeStep / 1000.0f;
}

//void AnimatedSprite::Add(Ref<SpriteSheet>& animationSheet) {
//	this->spriteSheets.push_back(animationSheet);
//	this->currentIndexes.push_back({ 0, 0 });
//}

Ref<AnimatedSprite> AnimatedSprite::Create(float timeStep) {
	return std::make_shared<AnimatedSprite>(timeStep);
}