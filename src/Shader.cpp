#include "../header/shader.h"

std::unordered_map<std::string, Ref<Shader>> Shader::LUT;
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

	const GLsizei bufSize = 16;
	GLint location, count, varsize, length;
	GLenum type;
	GLchar name[bufSize];

	glGetProgramiv(handle, GL_ACTIVE_ATTRIBUTES, &count);
	for (location = 0; location < count; location++)
	{
		glGetActiveAttrib(handle, (GLuint)location, bufSize, &length, &varsize, &type, name);
		attribLocation[name] = GLVariable{ location, type, name };
	}

	glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &count);
	for (location = 0; location < count; location++)
	{
		glGetActiveUniform(handle, (GLuint)location, bufSize, &length, &varsize, &type, name);
		uniformLocation[name] = GLVariable{ location, type, name };
	}

	if (Shader::log) std::cout << "CREATE Shader " << handle << " " << vertexfile << " " << fragmentfile << "\n";
}

void Shader::Init() {
#ifdef MAGIA_DEBUG
	LUT["unlit"] = Create("../shaders/unlit.vert", "../shaders/unlit.frag");
	LUT["glyph"] = Create("../shaders/glyph.vert", "../shaders/glyph.frag");
	LUT["image"] = Create("../shaders/image.vert", "../shaders/image.frag");
	LUT["unlit-edgefade"] = Create("../shaders/unlit.vert", "../shaders/unlit-edgefade.frag");
#else
	LUT["unlit"] = Create("shaders/unlit.vert", "shaders/unlit.frag");
	LUT["glyph"] = Create("shaders/glyph.vert", "shaders/glyph.frag");
	LUT["image"] = Create("shaders/image.vert", "shaders/image.frag");
	LUT["unlit-edgefade"] = Create("shaders/unlit.vert", "shaders/unlit-edgefade.frag");
#endif
}

void Shader::Bind() {
	glUseProgram(handle);
}

Shader::GLVariable& Shader::GetAttribLocation(std::string attrib) {
	assert(attribLocation.find(attrib) != attribLocation.end());
	return attribLocation[attrib];
}

Shader::GLVariable& Shader::GetUniformLocation(std::string uniform) {
	assert(uniformLocation.find(uniform) != uniformLocation.end());
	return uniformLocation[uniform];
}

void Shader::UniformSampler2D(std::string uniform, unsigned int value) {
	Bind();
	auto& uni = GetUniformLocation(uniform);
	assert(uni.type == GL_SAMPLER_2D);
	glUniform1i(uni.location, value);
}
void Shader::UniformFloat(std::string uniform, float value) {
	Bind();
	auto& uni = GetUniformLocation(uniform);
	assert(uni.type == GL_FLOAT);
	glUniform1f(uni.location, value);
}
void Shader::UniformVec2(std::string uniform, glm::vec2 vec) {
	Bind();
	auto& uni = GetUniformLocation(uniform);
	assert(uni.type == GL_FLOAT_VEC2);
	glUniform2f(uni.location, vec.x, vec.y);
}
void Shader::UniformVec3(std::string uniform, glm::vec3 vec) {
	Bind();
	auto& uni = GetUniformLocation(uniform);
	assert(uni.type == GL_FLOAT_VEC3);
	glUniform3f(uni.location, vec.x, vec.y, vec.z);
}
void Shader::UniformVec4(std::string uniform, glm::vec4 vec) {
	Bind();
	auto& uni = GetUniformLocation(uniform);
	assert(uni.type == GL_FLOAT_VEC4);
	glUniform4f(uni.location, vec.x, vec.y, vec.z, vec.w);
}
void Shader::UniformMat4(std::string uniform, glm::mat4 mat) {
	Bind();
	auto& uni = GetUniformLocation(uniform);
	assert(uni.type == GL_FLOAT_MAT4);
	glUniformMatrix4fv(uni.location, 1, GL_FALSE, glm::value_ptr(mat));
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