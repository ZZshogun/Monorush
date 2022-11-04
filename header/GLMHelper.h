#ifndef HELPER_GLM_H
#define HELPER_GLM_H

#include <glm/glm.hpp>
#include <string>

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

	static vec3 lerp(vec3 a, vec3 b, float t) {
		vec3 interpolate = (b - a) * clamp<float>(t, 0, 1);
		return a + interpolate;
	}

	template <typename vec>
	static std::string ToString(vec object) {
		std::stringstream ss;
		ss << std::setprecision(2) << std::fixed;
		for (int i = 0; i < object.length() - 1; i++) {
			ss << object[i] << " ";
		}
		ss << object[object.length() - 1];
		return ss.str();
	}
}

#endif