#include "Layer.h"

void Layer::SetClearColor(glm::vec4 color) {
	glClearColor(color.r, color.g, color.b, color.a);
}