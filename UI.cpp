#include "UI.h"

bool UI::inUI = false;
bool UI::onEvents = false;
Ref<UI::Button> UI::on_button = NULL;
Ref<UI::Button> UI::clicked_button = NULL;

glm::ivec2 UI::ref_resolution = {-1, -1};
std::string UI::font_name = "PixelGameFont";
std::string UI::glyphShader = "glyph";
std::string UI::imageShader = "image";

GLuint UI::vao;
std::vector<Ref<UI::Button>> UI::buttons;
UI::Button UI::hovering_button{};
UI::Button UI::null_button{};

UIAnchor UI::anchorMode = LEFT;

bool UI::log = true;

glm::vec2 UI::ratioRef(glm::ivec2 screen_pos) {
	glm::vec2 scaled = glm::vec2{ screen_pos } / glm::vec2{ ref_resolution };
	return scaled;
}

void UI::Init() {

	if (!log) Font::log = false;
	Font::LoadFont("Arial", "font/arial.ttf", {0, 48});
	Font::LoadFont("PixelGameFont", "font/PixelGameFont.ttf", {0, 48});

	if (log) std::cout << "INIT UI\n";
}

void UI::ClearBuffers() {
	for (auto& e : buttons) {
		//std::cout << e->text << "\n";
		e.reset();
	}
	buttons.clear();
}

void UI::Destroy() {
	glDeleteVertexArrays(1, &vao);
	if (log) std::cout << "DELETE UI\n";
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
	Ref<Shader>& shader = Shader::LUT[glyphShader];
	shader->Bind();

	glUniform4f(glGetUniformLocation(shader->handle, "fColor"), color.r, color.g, color.b, color.a);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vao);

	float offsetx = 0;
	float offsety = 0;
	size_t n = string.size() - 1;
	for (size_t i = 0; i <= n; i++) {
		FontChar& e = Font::GetFontChar(ftname, string[i]);
		if (i < n) offsetx += e.bearing.x + (e.advance >> 6);
		else offsetx += e.bearing.x + e.size.x;
		offsety = glm::max(offsety, (float)e.bearing.y / 2);
	}
	offsetx *= scale;
	offsety *= scale;
	if (anchorMode == CENTER) offsetx /= 2.0f;
	else if (anchorMode == LEFT) offsetx = 0;

	float x = (float)screen_pos.x - offsetx;
	float y = (float)screen_pos.y - offsety;

	for (char c : string) {
		FontChar& ftChar = Font::GetFontChar(ftname, c);
		glm::vec2 scaledSize = glm::vec2(ftChar.size) * scale;
		scaledSize = ratioRef(scaledSize);

		float width = scaledSize.x;
		float height = scaledSize.y;

		glm::vec2 scaledPos{0};
		scaledPos.x = x + ftChar.bearing.x * scale;
		scaledPos.y = y - (ftChar.size.y - ftChar.bearing.y) * scale;
		scaledPos = ratioRef(scaledPos);

		float xpos = scaledPos.x;
		float ypos = scaledPos.y;

		std::vector<Vertex> vertices = {
			Vertex{ glm::vec3{ xpos, ypos, 0 }, glm::vec2{ 0, 1 } },
			Vertex{ glm::vec3{ xpos, ypos + height, 0 }, glm::vec2{ 0, 0 } },
			Vertex{ glm::vec3{ xpos + width, ypos + height, 0 }, glm::vec2{ 1, 0 } },
			Vertex{ glm::vec3{ xpos + width, ypos, 0 }, glm::vec2{ 1, 1} },
		};
		glBindTexture(GL_TEXTURE_2D, ftChar.texID);
		//Texture::defaultTex->Bind();
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

		x += (ftChar.advance >> 6) * scale;
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

Ref<UI::Button> UI::CreateButton(
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
	Ref<Button> ref = std::make_shared<Button>
		(text, scale, textColor, screen_pos, screen_size, color, 
			presscolor, image, ref_resolution, hover_function, function);
	UI::buttons.emplace_back(ref);
	return ref;
}

Ref<UI::Button> UI::CreateButton(
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
	return CreateButton(text, scale, textColor, NULL, screen_pos, screen_size, function, color, presscolor, hover_function);
}

void UI::DrawButton(Ref<Button>& button, Time time) {
	if (!button) return;
	button->draw = true;
	Ref<Texture> tex = button->texture ? button->texture : Texture::defaultTex;
	
	if (button->blink > 0 && button->cur_color == button->presscolor) {
		button->blink -= time.deltaTime;
	}
	else if (button->cur_color == button->presscolor) {
		button->blink = 0;
	}
	
	button->cur_color = button->blink ? button->presscolor : button->color;
	glm::ivec2 f_ref = ref_resolution;
	ref_resolution = button->ref_resolution;
	DrawImage(tex, button->position, button->size, button->cur_color);
	DrawString(button->text, button->position, button->textScale, button->textColor);
	ref_resolution = f_ref;
}

void UI::DrawButtons(Time time) {
	for (Ref<Button>& button : buttons) {
		if (button->isValid())
			DrawButton(button, time);
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

void UI::PollsEvent(GLFWwindow* window, Time time) {
	glm::ivec2 resolution = {0, 0};
	glfwGetWindowSize(window, &resolution.x, &resolution.y);

	UI::onEvents = false;
	UI::on_button = NULL;
	for (int i = 0; i < buttons.size(); i++) {
		Ref<Button> button = buttons[i];
		if (!button->draw) continue;
		glm::ivec2 rawPos = glm::ivec2(Input::MousePosition()) - resolution / 2;
		glm::ivec2 clickPos = glm::ivec2{ rawPos.x, -rawPos.y };
		clickPos = glm::vec2{clickPos} / glm::vec2{resolution / 2} * glm::vec2{ button->ref_resolution };

		if (Math::InVec2(clickPos, button->position, button->size)) {
			UI::onEvents = true;
			UI::on_button = button;
			if (button->active && !button->hovered) {
				button->hovered = true;
				UI::hovering_button = *button;
				button->hover_function();
			}

			if (button->active) {
				if (!button->press && Input::mouseCode[Mouse_Left] == Input::KeyState::KEY_DOWN) {
					button->blink = button->resetBlink;
					button->press = button->resetPress;
					*button = hovering_button;
					hovering_button = null_button;
					clicked_button = button;
					if (button->function) button->function();
					UI::hovering_button = *button;
					button->hover_function();
				}
				else {
					button->press -= time.deltaTime;
					if (button->press <= 0) {
						button->press = 0;
						clicked_button = NULL;
					}
				}
			}
			break;
		}
		if (button->hovered && !UI::onEvents) {
			button->hovered = false;
			*button = hovering_button;
			hovering_button = null_button;
		}
		button->draw = false;
	}
}