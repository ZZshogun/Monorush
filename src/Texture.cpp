#include "../header/Texture.h"

Ref<Texture> Texture::defaultTex;
bool Texture::log = true;
std::map <std::string, Ref<Texture>> Texture::library;

void Texture::Init() {
	defaultTex = Texture::Create("sprite_default", "texture/sprite_default.png");
}

Texture::Texture(std::string name, const char* file, GLuint slot, bool absolutePath) {
	this->unit = slot;
	this->name = name;

	std::string path = std::string();

	if (absolutePath) path = file;
	else path = MAGIA_PATH(std::string(file));

	int imgW, imgH, imgCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(path.c_str(), &imgW, &imgH, &imgCh, 0);

	glGenTextures(1, &handle);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, handle);

	glEnable(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	switch (imgCh) {
		case 1:	
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgW, imgH, 0, GL_RED, GL_UNSIGNED_BYTE, bytes);
			break;
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgW, imgH, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
			break;
		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgW, imgH, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
			break;
		default:
			throw std::invalid_argument("Texture bit depth is invalid. (" + std::to_string(imgCh) + " bits)");
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	if (log) std::cout << "CREATE Texture " << handle << " " << path << "\n";


	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Ref<Texture> Texture::Create() {
	return defaultTex;
}

Ref<Texture> Texture::Create(std::string name, std::string file, bool abs, GLuint slot) {
	if (library.find(name) != library.end()) library[name]->Delete();
	library[name] = std::make_shared<Texture>(name, file.c_str(), slot, abs);
	return library[name];
}

void Texture::ClearHandles() {
	for (auto& tex : library) {
		if (tex.first == defaultTex->name) continue;
		tex.second->Delete();
	}
	library.clear();
	library[defaultTex->name] = defaultTex;
}

void Texture::TexUnit(Ref<Shader>& shader, const char* uniform, GLuint unit) {
	shader->UniformSampler2D(uniform, unit);
}

void Texture::Bind() {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, handle);
}

void Texture::Unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() {
	if (disposed) return;

	disposed = true;
	if (log) std::cout << "DELETE Texture " << handle << "\n";
	glDeleteTextures(1, &handle);
}

Texture::~Texture() {
	Delete();
}