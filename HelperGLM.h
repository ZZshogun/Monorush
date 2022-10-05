#ifndef HELPER_GLM_H
#define HELPER_GLM_H

#include "Core.h"

namespace glm {
	constexpr float max(vec2 vec) {
		return max(vec.x, vec.y);
	}

	constexpr float max(vec3 vec) {
		return max(max(vec.x, vec.y), vec.z);
	}

	constexpr float max(vec4 vec) {
		return max(max(max(vec.x, vec.y), vec.z), vec.w);
	}
}

#endif