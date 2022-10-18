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

	// Holds button data
	struct Button {
		std::string text;
		float textScale;
		glm::vec4 textColor;
		glm::vec2 position;
		glm::vec2 size;
		glm::vec4 color = {1, 1, 1 ,1};
		Ref<Texture> texture = NULL;
		glm::ivec2 ref_resolution;
		std::function<void()> function = NULL;
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

	static bool ClickEvent(glm::ivec2 resolution);

	static void StartUI();
	static void StartUI(std::string fontName, std::string shader = "");
	static void StartUI(glm::ivec2 ref_resolution, std::string fontName = "", std::string shader = "");
	static void EndUI();
	
	static void Anchor(UIAnchor anchorEnum);

	static void DrawString(std::string string, glm::ivec2 screen_pos, float scale, glm::vec4 color, std::string fontName = "");
	static void DrawImage(Ref<Texture>& image, glm::ivec2 screen_pos, glm::ivec2 screen_size, glm::vec4 color = {1, 1, 1, 1});
	static void DrawButtons();

	static void CreateButton(
		std::string text,
		float scale,
		glm::vec4 textColor,
		Ref<Texture> image, 
		glm::ivec2 screen_pos, 
		glm::ivec2 screen_size, 
		std::function<void()> function,
		glm::vec4 color = {1, 1, 1 ,1}
	);
	static void CreateButton(
		std::string text, 
		float scale, 
		glm::vec4 textColor,
		glm::ivec2 screen_pos, 
		glm::ivec2 screen_size, 
		glm::vec4 color,
		std::function<void()> function
	);
};

#endif