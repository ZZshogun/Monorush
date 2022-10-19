#include "UI.h"

bool UI::inUI = false;
bool UI::onEvents = false;
UI::Button* UI::on_button = NULL;

glm::ivec2 UI::ref_resolution = {-1, -1};
std::string UI::font_name = "PixelGameFont";
std::string UI::glyphShader = "glyph";
std::string UI::imageShader = "image";

GLuint UI::vao;
std::vector<UI::Button> UI::buttons;
std::vector<UI::Button> UI::_buttons;

UIAnchor UI::anchorMode = LEFT;

glm::vec2 UI::ratioRef(glm::ivec2 screen_pos) {
	glm::vec2 scaled = glm::vec2{ screen_pos } / glm::vec2{ ref_resolution };
	return scaled;
}

void UI::Init() {
	Font::LoadFont("Arial", "font/arial.ttf", {0, 48});
	Font::LoadFont("PixelGameFont", "font/PixelGameFont.ttf", {0, 48});

	std::cout << "INIT UI\n";
}

void UI::ClearBuffers() {
	buttons.clear();
	_buttons.clear();
}

void UI::Destroy() {
	glDeleteVertexArrays(1, &vao);
	std::cout << "DELETE UI\n";
}

void UI::StartUI(glm::ivec2 ref_resolution, std::string fontName, std::string shader) {
	if (inUI) {
		std::cout << "ERROR UI Already started UI block\n";
		return;
	}

	UI::ref_resolution = ref_resolution / 2;
	if (fontName != "") UI::font_name = fontName;
	if (shader != "") UI::glyphShader = shader;

	glGenVertexArrays(1, &vao);

	anchorMode = LEFT;

	inUI = true;
}

void UI::StartUI(std::string fontName, std::string shader) {
	glm::ivec2 res = ref_resolution == glm::ivec2{-1, -1} ? glm::ivec2{ 1280, 720 } : ref_resolution;
	StartUI(res, fontName, shader);
}

void UI::StartUI() {
	glm::ivec2 res = ref_resolution == glm::ivec2{ -1, -1 } ? glm::ivec2{ 1280, 720 } : ref_resolution;
	StartUI(res, font_name);
}

void UI::Anchor(UIAnchor anchorEnum) {
	if (!inUI) {
		std::cout << "ERROR UI No starting UI block\n";
		return;
	}
	anchorMode = anchorEnum;
}

void UI::DrawString(std::string string, glm::ivec2 screen_pos, float scale, glm::vec4 color, std::string fontName) {
	if (!inUI) {
		std::cout << "ERROR UI No starting UI block\n";
		return;
	}

	std::vector<GLuint> indices = {
		0, 3, 1,
		1, 3, 2,
	};

	std::string ftname = fontName == "" ? font_name : fontName;
	glm::vec2 scr_pos = ratioRef(screen_pos);

	Ref<Shader>& shader = Shader::LUT[glyphShader];
	shader->Bind();

	glUniform4f(glGetUniformLocation(shader->handle, "fColor"), color.r, color.g, color.b, color.a);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vao);

	float offsetx = 0, offsety = 0;
	if (anchorMode != LEFT) {
		for (char c : string) {
			FontChar& e = Font::GetFontChar(ftname, c);
			offsetx += e.bearing.x + (e.advance >> 6);
			offsety = glm::max<float>(offsety, (float)e.bearing.y);
		}
		offsetx *= scale / ref_resolution.x;
		offsety *= scale / ref_resolution.y;
		if (anchorMode == CENTER) offsetx /= 2.0f;
	}

	float x = scr_pos.x - offsetx + 0.005f; // 0.005 -> magic number
	float y = scr_pos.y - offsety / 2.0f;

	for (char c : string) {
		FontChar& ftChar = Font::GetFontChar(ftname, c);
		glm::vec2 scaledSize = ratioRef(ftChar.size);
		glm::vec2 scaledBearing = ratioRef(ftChar.bearing);

		float xpos = x + scaledBearing.x * scale;
		float ypos = y - (scaledSize.y - scaledBearing.y) * scale;

		float width = scaledSize.x * scale;
		float height = scaledSize.y * scale;

		std::vector<Vertex> vertices = {
			Vertex{ glm::vec3{ xpos, ypos, 0 }, glm::vec2{ 0, 1 } },
			Vertex{ glm::vec3{ xpos, ypos + height, 0 }, glm::vec2{ 0, 0 } },
			Vertex{ glm::vec3{ xpos + width, ypos + height, 0 }, glm::vec2{ 1, 0 } },
			Vertex{ glm::vec3{ xpos + width, ypos, 0 }, glm::vec2{ 1, 1} },
		};
		glBindTexture(GL_TEXTURE_2D, ftChar.texID);
		glUniform1i(glGetUniformLocation(shader->handle, "tex0"), 0);

		VBO vbo(vertices);
		EBO ebo(indices);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, (void*)0);

		vbo.Unbind();
		ebo.Unbind();

		vbo.Delete();
		ebo.Delete();

		x += (ftChar.advance >> 6) * scale / ref_resolution.x;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void UI::DrawImage(Ref<Texture>& image, glm::ivec2 screen_pos, glm::ivec2 screen_size, glm::vec4 color) {
	if (!inUI) {
		std::cout << "ERROR UI No starting UI block\n";
		return;
	}

	std::vector<GLuint> indices = {
		0, 3, 1,
		1, 3, 2,
	};

	glm::vec2 scr_pos = ratioRef(screen_pos);
	glm::vec2 scr_size = ratioRef(screen_size);

	if (anchorMode == CENTER) {
		scr_pos -= scr_size / 2.0f;
	}
	else if (anchorMode == RIGHT) {
		scr_pos.x -= scr_size.x;
		scr_pos.y -= scr_size.y / 2.0f;
	}
	else if (anchorMode == LEFT) {
		scr_pos.y -= scr_size.y / 2.0f;
	}

	std::vector<Vertex> vertices = {
		Vertex{ glm::vec3{ scr_pos.x, scr_pos.y, 0 }, glm::vec2{ 0, 0 } },
		Vertex{ glm::vec3{ scr_pos.x, scr_pos.y + scr_size.y, 0 }, glm::vec2{ 0, 1 } },
		Vertex{ glm::vec3{ scr_pos.x + scr_size.x, scr_pos.y + scr_size.y, 0 }, glm::vec2{ 1, 1 } },
		Vertex{ glm::vec3{ scr_pos.x + scr_size.x, scr_pos.y, 0 }, glm::vec2{ 1, 0 } },
	};

	Ref<Shader>& shader = Shader::LUT[imageShader];
	shader->Bind();

	glUniform4f(glGetUniformLocation(shader->handle, "fColor"), color.r, color.g, color.b, color.a);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vao);

	image->Bind();
	image->TexUnit(shader, "tex0", 0);

	VBO vbo(vertices);
	EBO ebo(indices);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, (void*)0);

	vbo.Unbind();
	ebo.Unbind();

	vbo.Delete();
	ebo.Delete();
}

void UI::CreateButton(
	std::string text,
	float scale,
	glm::vec4 textColor,
	Ref<Texture> image,
	glm::ivec2 screen_pos,
	glm::ivec2 screen_size,
	std::function<void()> function,
	glm::vec4 color,
	glm::vec4 presscolor,
	std::function<void()> hover_function
) {
	Button button;
	button.color = color;
	button.presscolor = presscolor;
	button.text = text;
	button.textScale = scale;
	button.textColor = textColor;
	button.texture = image;
	button.position = screen_pos;
	button.size = screen_size;
	button.function = function;
	button.hover_function = hover_function;
	button.ref_resolution = UI::ref_resolution;

	UI::_buttons.emplace_back(button);
	UI::buttons.emplace_back(button);
}

void UI::CreateButton(
	std::string text,
	float scale,
	glm::vec4 textColor,
	glm::ivec2 screen_pos,
	glm::ivec2 screen_size,
	glm::vec4 color,
	std::function<void()> function,
	glm::vec4 presscolor,
	std::function<void()> hover_function

) {
	CreateButton(text, scale, textColor, NULL, screen_pos, screen_size, function, color, presscolor, hover_function);
}

void UI::DrawButtons(Time time) {
	for (Button& button : buttons) {
		Ref<Texture> tex = button.texture.get() ? button.texture : Texture::defaultTex;

		if (button.press > 0 && button.cur_color == button.presscolor) {
			button.press -= time.deltaTime;
		}
		else if (button.cur_color == button.presscolor) {
			button.press = 0;
		}

		button.cur_color = button.press ? button.presscolor : button.color;
		glm::ivec2 f_ref = ref_resolution;
		ref_resolution = button.ref_resolution;
		DrawImage(tex, button.position, button.size, button.cur_color);
		DrawString(button.text, button.position, button.textScale, button.textColor);
		ref_resolution = f_ref;
	}
}

void UI::EndUI() {
	if (!inUI) {
		std::cout << "ERROR UI No starting UI block\n";
		return;
	}

	ref_resolution = { -1, -1 };
	glDeleteVertexArrays(1, &vao);
	inUI = false;
}

void UI::PollsEvent(GLFWwindow* window) {
	glm::ivec2 resolution = {0, 0};
	glfwGetWindowSize(window, &resolution.x, &resolution.y);

	UI::onEvents = false;
	UI::on_button = NULL;
	for (int i = 0; i < buttons.size(); i++) {
		Button& button = buttons[i];

		glm::ivec2 rawPos = glm::ivec2(Input::MousePosition()) - resolution / 2;
		glm::ivec2 clickPos = glm::ivec2{ rawPos.x, -rawPos.y };
		clickPos = glm::vec2{clickPos} / glm::vec2{resolution / 2} * glm::vec2{ button.ref_resolution };

		if (Math::InVec2(clickPos, button.position, button.size)) {
			UI::onEvents = true;
			UI::on_button = &button;
			if (!button.hovered) {
				button.hover_function();
				button.hovered = true;
			}

			if (Input::mouseCode[Mouse_Left] == Input::KeyState::KEY_DOWN) {
				if (button.function) button.function();
				button.press = button.resetBlink;
			}
			break;
		}
		if (button.hovered && !UI::onEvents) {
			button.hovered = false;
			button = _buttons[i];
		}
	}
}