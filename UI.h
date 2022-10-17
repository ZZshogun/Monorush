#ifndef UI_CLASS_H
#define UI_CLASS_H

#include "Magia.h"
#include "Font.h"

class UI {

	UI();
	static bool inUI;

	static glm::ivec2 ref_resolution;
	static std::string font_name;
	static glm::vec2 ratioRef(glm::ivec2 screen_pos);

	static std::string glyphShader;

	static GLuint vao;

public:

	static void Init();
	static void Destroy();

	static void StartUI();
	static void StartUI(std::string fontName, std::string shader = "");
	static void StartUI(glm::ivec2 ref_resolution, std::string fontName = "", std::string shader = "");
	static void DrawString(std::string string, glm::ivec2 screen_pos, float scale, glm::vec4 color, std::string fontName = "");
	static void EndUI();
};

#endif