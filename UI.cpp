#include "UI.h"

bool UI::inUI = false;

glm::ivec2 UI::ref_resolution = {-1, -1};
std::string UI::font_name = "PixelGameFont";
std::string UI::glyphShader = "glyph";

GLuint UI::vao;

UIAnchor UI::anchorMode = LEFT;

glm::vec2 UI::ratioRef(glm::ivec2 screen_pos) {
	glm::vec2 scaled = glm::vec2{ screen_pos.x, screen_pos.y } / glm::vec2{ ref_resolution.x, ref_resolution.y };
	return scaled;
}

void UI::Init() {

	Font::LoadFont("Arial", "font/arial.ttf", {0, 96});
	Font::LoadFont("PixelGameFont", "font/PixelGameFont.ttf", {0, 96});

	std::cout << "INIT UI\n";
}

void UI::Destroy() {
	glDeleteVertexArrays(1, &vao);
	std::cout << "DESTROY UI\n";
}

void UI::StartUI(glm::ivec2 ref_resolution, std::string fontName, std::string shader) {
	if (inUI) {
		std::cout << "ERROR UI Already started UI block\n";
		return;
	}

	UI::ref_resolution = ref_resolution;
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

	float offsetx = 0;
	if (anchorMode != LEFT) {
		float lastAdvance = 0;
		for (char c : string) {
			auto& e = Font::GetFontChar(ftname, c);
			offsetx += e.bearing.x + (e.advance >> 6);
			lastAdvance = (float)(e.advance >> 6);
		}
		//offsetx -= lastAdvance;
		offsetx *= scale / ref_resolution.x;
		if (anchorMode == CENTER) offsetx /= 2.0f;
	}

	float x = scr_pos.x - offsetx, y = scr_pos.y;

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

void UI::EndUI() {
	if (!inUI) {
		std::cout << "ERROR UI No starting UI block\n";
		return;
	}

	ref_resolution = { -1, -1 };
	glDeleteVertexArrays(1, &vao);
	inUI = false;
}