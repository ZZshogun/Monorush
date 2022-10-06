#include "Animation.h"

AnimatedSprite::AnimatedSprite(float timeStep) {
	this->timeStep = timeStep / 1000.0f;
}

void AnimatedSprite::SetTimeStep(float timeStep) {
	this->timeStep = timeStep / 1000.0f;
}

void AnimatedSprite::Add(Ref<SpriteSheet>& animationSheet) {
	this->spriteSheets.push_back(animationSheet);
	this->currentIndexes.push_back({ 0, 0 });
}

void AnimatedSprite::Draw(int animationIndex, float deltaTime) {
	if (spriteSheets.empty()) return;
	animationIndex = glm::clamp(animationIndex, 0, (int)spriteSheets.size() - 1);

	elapsed += deltaTime;
	glm::vec2 sheetSize = spriteSheets[animationIndex]->size();
	glm::vec2* currentIndex = &currentIndexes[animationIndex];

	if (elapsed >= timeStep) {
		elapsed = 0;
		currentIndex->x++;
		if (currentIndex->x >= sheetSize.x) {
			currentIndex->x = 0;
			currentIndex->y++;
		}
		if (currentIndex->y >= sheetSize.y) {
			currentIndex->y = 0;
		}
	}
	spriteSheets[animationIndex]->transform = transform;
	spriteSheets[animationIndex]->Draw(*currentIndex);
}

Ref<AnimatedSprite> AnimatedSprite::Create(float timeStep) {
	return std::make_shared<AnimatedSprite>(timeStep);
}