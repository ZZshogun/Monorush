#ifndef MATH_HELPER_H
#define MATH_HELPER_H

#include <assert.h>

namespace Math {
	static float Random(float min, float max) {
		assert(min < max);
		float diff = max - min;
		float r = (float)rand() / (float)RAND_MAX;
		return min + r * diff;
	}
}

#endif
