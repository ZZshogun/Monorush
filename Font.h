#ifndef FONT_CLASS_H
#define FONT_CLASS_H

#include "Core.h"
#include "Texture.h"

struct FontChar {
	GLuint texID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	uint32_t advance;
};

class Font {
	using FontFace = std::map<char, FontChar>;
	static std::map<std::string, FontFace> fonts_map;

public:
	Font() = default;
	static void LoadFont(std::string name, std::string path, glm::ivec2 charsetsSize = {0, 48});

	static FontFace& GetFont(std::string fontName);
	static FontChar& GetFontChar(std::string fontName, char character);
};

#endif