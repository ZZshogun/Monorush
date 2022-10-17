#include "Font.h"

std::map<std::string, Font::FontFace> Font::fonts_map;

void Font::LoadFont(std::string name, std::string path, glm::ivec2 charsetsSize) {

	if (fonts_map.find(name) != fonts_map.end()) return;

	FT_Error ftFail;
	FT_Library ftLibrary;
	ftFail = FT_Init_FreeType(&ftLibrary);
	if (ftFail) {
		std::cout << "ERROR FreeType_library code " << ftFail << "\n";
	}
	FT_Face face;
	ftFail = FT_New_Face(ftLibrary, path.c_str(), 0, &face);
	if (ftFail) {
		std::cout << "ERROR FreeType_FontFace code " << ftFail << "\n";
	}
	FT_Set_Pixel_Sizes(face, charsetsSize.x, charsetsSize.y);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (unsigned char c = 0; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR Freetype_char " << c << "\n";
			continue;
		}
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		FontChar ftChar{};
		ftChar.texID = texture;
		ftChar.size = { face->glyph->bitmap.width, face->glyph->bitmap.rows };
		ftChar.bearing = { face->glyph->bitmap_left, face->glyph->bitmap_top };
		ftChar.advance = face->glyph->advance.x;
		fonts_map[name][c] = ftChar;
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ftLibrary);

	std::cout << "CREATE Font " << name << " " << path << "\n";
}

Font::FontFace& Font::GetFont(std::string fontName) {
	return fonts_map[fontName];
}

FontChar& Font::GetFontChar(std::string fontName, char character) {
	if (fonts_map.find(fontName) == fonts_map.end()) {
		throw std::exception("Font Face not found");
	}
	return fonts_map[fontName][character];
}