#include "Noise.h"

FastNoise::SmartNode<> Noise::boxGenerator;
Noise::RNG Noise::rng;
std::uniform_int_distribution<int16_t> Noise::int16_dist;
int16_t Noise::seed = 1337;

bool Noise::log = true;

void Noise::Init() {
	std::random_device dev;
	std::mt19937 rng(dev());
	rng.seed((unsigned)time(NULL));
	int16_dist = std::uniform_int_distribution<int16_t>(INT16_MIN, INT16_MAX);
	RandomSeed();

	boxGenerator = FastNoise::NewFromEncodedNodeTree("HgATAJqZ2kIHAABSuB4/");
}

void Noise::RandomSeed() {
	seed = int16_dist(rng);
	if (log) std::cout << "Noise seed : " << seed << "\n";
}

void Noise::GetNoise2D(float* arr, glm::ivec2 size, glm::vec2 offset) {
	boxGenerator->GenUniformGrid2D
	(arr, (int)offset.x, (int)offset.y, (int)size.x, (int)size.y, 0.0075f, seed);
}