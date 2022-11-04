#ifndef CORE_H
#define CORE_H

// Includes

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <AudioFile/AudioFile.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <queue>
#include <random>
#include <assert.h>

#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <EnTT/entt.hpp>

#include <ft2build.h>
#include <freetype/freetype.h>

#include <FastNoise/FastNoise.h>

#include "GLMHelper.h"
#include "MathHelper.h"

// Macros

template<typename T>
using Ref = std::shared_ptr<T>;

namespace Color {
	const glm::vec4 Transparent = { 0, 0, 0, 0 };
	const glm::vec4 Black = { 0, 0, 0, 1 };
	const glm::vec4 White = { 1, 1, 1, 1 };
}

#endif