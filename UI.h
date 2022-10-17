#ifndef UI_CLASS_H
#define UI_CLASS_H

#include "Magia.h"
#include "Font.h"

enum UIAnchor {
	LEFT,
	CENTER,
	RIGHT
};

class UI {

	// scaled position & size
	struct Button {
		std::string text;
		float textScale;
		glm::vec4 textColor;
		glm::vec2 position;
		glm::vec2 size;
		glm::vec4 color = {1, 1, 1 ,1};
		Ref<Texture> texture = NULL;
	};

	UI();
	static bool inUI;

	static glm::ivec2 ref_resolution;
	static std::string font_name;
	static glm::vec2 ratioRef(glm::ivec2 screen_pos);

	static std::string glyphShader;
	static std::string imageShader;

	static GLuint vao;

	static UIAnchor anchorMode;

	static std::vector<Button> buttons;

public:

	static void Init();
	static void Destroy();

	static void StartUI();
	static void StartUI(std::string fontName, std::string shader = "");
	static void StartUI(glm::ivec2 ref_resolution, std::string fontName = "", std::string shader = "");
	static void EndUI();
	
	static void Anchor(UIAnchor anchorEnum);

	static void DrawString(std::string string, glm::ivec2 screen_pos, float scale, glm::vec4 color, std::string fontName = "");
	static void DrawImage(Ref<Texture>& image, glm::ivec2 screen_pos, glm::ivec2 screen_size, glm::vec4 color = {1, 1, 1, 1});
	static void DrawButtons();

	// TODO : Make buttons clickable
	static void CreateButton(
		std::string text,
		float scale,
		glm::vec4 textColor,
		Ref<Texture> image, 
		glm::ivec2 screen_pos, 
		glm::ivec2 screen_size, 
		glm::vec4 color = {1, 1, 1 ,1}
	);
	static void CreateButton(
		std::string text, 
		float scale, 
		glm::vec4 textColor,
		glm::ivec2 screen_pos, 
		glm::ivec2 screen_size, 
		glm::vec4 color
	);
};

#endif