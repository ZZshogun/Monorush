#ifndef UI_CLASS_H
#define UI_CLASS_H

#include "Magia.h"
#include "Time.h"
#include "Font.h"

enum UIAnchor {
	LEFT,
	CENTER,
	RIGHT
};

class UI {

	// Holds button data
	struct Button {
		std::string text = "";
		float textScale = -1;
		glm::vec4 textColor = Color::White;
		glm::vec2 position = {0, 0};
		glm::vec2 size = {-1, -1};
		glm::vec4 color = Color::White;
		glm::vec4 presscolor = Color::White;
		Ref<Texture> texture = NULL;
		glm::ivec2 ref_resolution = {-1, -1};
		std::function<void()> hover_function = NULL;
		std::function<void()> function = NULL;

		const float resetBlink = 0.15f;
		float press = 0;
		glm::vec4 cur_color = Color::White;
		bool hovered = false;

		Button operator=(const Button b) {
			this->text = b.text;
			this->textScale = b.textScale;
			this->textColor = b.textColor;
			this->position = b.position;
			this->size = b.size;
			this->color = b.color;
			this->presscolor = b.presscolor;
			this->texture = b.texture;
			this->ref_resolution = b.ref_resolution;
			this->hover_function = b.hover_function;
			this->function = b.function;
			return *this;
		}
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
	static std::vector<Button> _buttons;

public:
	static bool onEvents;
	static Button* on_button;

	static void Init();
	static void Destroy();

	static void PollsEvent(GLFWwindow* window);

	static void StartUI();
	static void StartUI(std::string fontName, std::string shader = "");
	static void StartUI(glm::ivec2 ref_resolution, std::string fontName = "", std::string shader = "");
	static void EndUI();
	
	static void Anchor(UIAnchor anchorEnum);

	static void DrawString(std::string string, glm::ivec2 screen_pos, float scale, glm::vec4 color, std::string fontName = "");
	static void DrawImage(Ref<Texture>& image, glm::ivec2 screen_pos, glm::ivec2 screen_size, glm::vec4 color = Color::White);
	static void DrawButtons(Time time);

	static void CreateButton(
		std::string text,
		float scale,
		glm::vec4 textColor,
		Ref<Texture> image, 
		glm::ivec2 screen_pos, 
		glm::ivec2 screen_size, 
		std::function<void()> function = [](){},
		glm::vec4 color = Color::White,
		glm::vec4 presscolor = Color::White,
		std::function<void()> hover_function = [](){}
	);
	static void CreateButton(
		std::string text, 
		float scale, 
		glm::vec4 textColor,
		glm::ivec2 screen_pos, 
		glm::ivec2 screen_size, 
		glm::vec4 color,
		std::function<void()> function = []() {},
		glm::vec4 presscolor = Color::White,
		std::function<void()> hover_function = []() {}
	);
};

#endif