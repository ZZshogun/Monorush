#include "Time.h"

void Time::_UpdateTime(float time) {
	this->_prevTime = this->_currentTime;
	this->_currentTime = time;

	this->unscaledDeltaTime = this->_currentTime - this->_prevTime;
	this->deltaTime = this->timeScale * this->unscaledDeltaTime;
}