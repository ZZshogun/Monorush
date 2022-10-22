#ifndef NOISE_CLASS_H
#define NOISE_CLASS_H

#include "Core.h"

class Noise {
	using RNG = std::mt19937;
	static RNG rng;
	static std::uniform_int_distribution<int16_t> int16_dist;
	static FastNoise::SmartNode<> boxGenerator;
	
	static int16_t seed;

public:
	static bool log;

	static void Init();
	static void RandomSeed();
	static void GetNoise2D(float* arr, glm::ivec2 size, glm::vec2 offset = { 0, 0 });
};


#endif