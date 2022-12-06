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
public:
	// Holds button data
	struct Button {
		bool active = true;
		bool draw = false;
		std::string text = "";
		float textScale = -1;
		glm::vec4 textColor = Color::White;
		glm::vec2 position = { 0, 0 };
		glm::vec2 size = { -1, -1 };
		glm::vec4 color = Color::White;
		glm::vec4 presscolor = Color::White;
		Ref<Texture> texture = NULL;
		glm::ivec2 ref_resolution = { -1, -1 };
		std::function<void()> hover_function = NULL;
		std::function<void()> function = NULL;

		const float resetPress = 0.1f;
		const float resetBlink = 0.15f;
		float blink = 0;
		float press = 0;
		glm::vec4 cur_color = Color::White;
		bool hovered = false;
		
		Button() {
			//std::cout << "CREATE Button " << text << "\n";
		}

		Button(std::string text, float texScale, glm::vec4 textColor, glm::vec2 position, glm::vec2 size,
			glm::vec4 color, glm::vec4 pressColor, Ref<Texture>& texture, glm::ivec2 ref_res,
			std::function<void()> hover_func, std::function<void()> func) :
			text(text), textScale(texScale), textColor(textColor), position(position), size(size), color(color),
			presscolor(pressColor), texture(texture), ref_resolution(ref_res), hover_function(hover_func),
			function(func)
		{
			//std::cout << "CREATE Button " << text << "\n";
		}

		~Button() {
			//std::cout << "DELETE Button " << text << "\n";
		}

		bool isValid() {
			return size.x > 0 && size.y > 0 && textScale >= 0;
		}

		Button& operator=(const Button& b) {
			text = b.text;
			textScale = b.textScale;
			textColor = b.textColor;
			position = b.position;
			size = b.size;
			color = b.color;
			presscolor = b.presscolor;
			texture = b.texture;
			ref_resolution = b.ref_resolution;
			hover_function = b.hover_function;
			function = b.function;
			return *this;
		}
	};

private:
	UI();
	static bool inUI;

	static glm::ivec2 ref_resolution;
	static std::string font_name;
	static glm::vec2 ratioRef(glm::ivec2 screen_pos);

	static std::string glyphShader;
	static std::string imageShader;

	static UIAnchor anchorMode;
	static std::vector<Ref<Button>> buttons;
	static Button null_button;

	static Ref<VAO> vao;
	static Ref<VBO> vbo;
	static Ref<EBO> ebo;

public:
	static bool onEvents;
	static Ref<Button> on_button;
	static Ref<Button> clicked_button;
	static Button hovering_button;

	static bool log;

	static void Init();
	static void ClearBuffers();
	static void Destroy();

	static void PollsEvent(GLFWwindow* window, Time time);

	static void StartUI();
	static void StartUI(std::string fontName, std::string shader = "");
	static void StartUI(glm::ivec2 ref_resolution, std::string fontName = "", std::string shader = "");
	static void EndUI();
	
	static void Anchor(UIAnchor anchorEnum);

	static void DrawString(std::string string, glm::ivec2 screen_pos, float scale, glm::vec4 color, std::string fontName = "");
	static void DrawImage(Ref<Texture>& image, glm::ivec2 screen_pos, glm::ivec2 screen_size, glm::vec4 color = Color::White);
	static void DrawImage(glm::ivec2 screen_pos, glm::ivec2 screen_size, glm::vec4 color);
	static void DrawButton(Ref<Button>& button, Time time);

	static Ref<Button> CreateButton(
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
	static Ref<Button> CreateButton(
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