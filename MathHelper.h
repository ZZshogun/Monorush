#ifndef MATH_HELPER_H
#define MATH_HELPER_H

#include <assert.h>
#include <glm/glm.hpp>

namespace Math {
	static float Random(float min, float max) {
		assert(min < max);
		float diff = max - min;
		float r = (float)rand() / (float)RAND_MAX;
		return min + r * diff;
	}

	static bool InBox2i(glm::ivec2 point, glm::ivec2 box_origin, glm::ivec2 box_size) {
		glm::ivec2 start = box_origin - box_size / 2;
		glm::ivec2 end = box_origin + box_size / 2;
		if (start.x <= point.x && start.y <= point.y && point.x <= end.x && point.y <= end.y) return true;
		else return false;
	}

	static bool InBox2(glm::vec2 point, glm::vec2 box_origin, glm::vec2 box_size) {
		glm::vec2 start = box_origin - box_size / 2.0f;
		glm::vec2 end = box_origin + box_size / 2.0f;
		if (start.x <= point.x && start.y <= point.y && point.x <= end.x && point.y <= end.y) return true;
		else return false;
	}

	template <typename T> 
	static int Sign(T val) {
		return (T(0) < val) - (val < T(0));
	}
}

#endif
