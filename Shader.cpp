#include "Shader.h"

std::map<std::string, Ref<Shader>> Shader::LUT;
bool Shader::log = true;

std::string Read_from_file(const char* filepath) {
	std::string ret = "";
	std::ifstream file(filepath);

	std::string tmp;
	while (std::getline(file, tmp)) {
		ret.append(tmp + '\n');
	}

	file.close();
	return ret;
}

Shader::Shader() {
	handle = NULL;
}

Ref<Shader> Shader::Create(const char* vertexCode, const char* fragmentCode) {
	return std::make_shared<Shader>(vertexCode, fragmentCode);
}

Shader::Shader(const char* vertexfile, const char* fragmentfile) {

	std::string vertex = Read_from_file(vertexfile);
	const char* vertexCode = vertex.c_str();

	int ret;
	char log[512];

	int vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vertexCode, NULL);
	glCompileShader(vert);

	glGetShaderiv(vert, GL_COMPILE_STATUS, &ret);
	if (!ret) {
		glGetShaderInfoLog(vert, 512, 0, log);
		std::cout << "ERROR VERTEX " << vert << " | " << log << "\n";
	}

	std::string fragC = Read_from_file(fragmentfile);
	const char* fragCode = fragC.c_str();

	int frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &fragCode, NULL);
	glCompileShader(frag);

	glGetShaderiv(frag, GL_COMPILE_STATUS, &ret);
	if (!ret) {
		glGetShaderInfoLog(frag, 512, 0, log);
		std::cout << "ERROR FRAGMENT " << frag << " | " << log << "\n";
	}

	handle = glCreateProgram();
	glAttachShader(handle, vert);
	glAttachShader(handle, frag);
	glLinkProgram(handle);

	glDetachShader(handle, vert);
	glDetachShader(handle, frag);

	glDeleteShader(vert);
	glDeleteShader(frag);

	if (Shader::log) std::cout << "CREATE Shader " << handle << " " << vertexfile << " " << fragmentfile << "\n";
}

void Shader::Init() {
	LUT["unlit"] = Create("unlit.vert", "unlit.frag");
	LUT["glyph"] = Create("glyph.vert", "glyph.frag");
	LUT["image"] = Create("image.vert", "image.frag");
	LUT["unlit-edgefade"] = Create("unlit.vert", "unlit-edgefade.frag");
}

void Shader::Bind() {
	glUseProgram(handle);
}

Shader::~Shader() {
	Delete();
}

void Shader::Delete() {
	if (disposed) return;

	if (log) std::cout << "DELETE Shader " << handle << "\n";
	glDeleteProgram(handle);
	disposed = true;
}