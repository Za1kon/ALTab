// ===== Shader.cpp =====
#include "Shader.hpp"
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath, bool fromFile) {
	std::string vertSrc = fromFile ? LoadFromFile(vertexPath) : vertexPath;
	std::string fragSrc = fromFile ? LoadFromFile(fragmentPath) : fragmentPath;

	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertSrc);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragSrc);

	ID = glCreateProgram();
	glAttachShader(ID, vs);
	glAttachShader(ID, fs);
	glLinkProgram(ID);

	int success;
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		int len;
		glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &len);
		std::vector<char> log(len);
		glGetProgramInfoLog(ID, len, &len, log.data());
		std::cerr << "Program link error:\n" << log.data() << "\n";
	}

	glDeleteShader(vs);
	glDeleteShader(fs);
}

Shader::~Shader() {
	glDeleteProgram(ID);
}

void Shader::Use() const {
	glUseProgram(ID);
}

int Shader::getUniformLocation(const std::string& name) const {
	return glGetUniformLocation(ID, name.c_str());
}

void Shader::SetUniform(const std::string& name, float v) const {
	int loc = getUniformLocation(name);
	if (loc != -1) glUniform1f(loc, v);
}

void Shader::SetUniform(const std::string& name, int v) const {
	int loc = getUniformLocation(name);
	if (loc != -1) glUniform1i(loc, v);
}

void Shader::SetUniform(const std::string & name, const glm::vec2 & v) const {
	int loc = getUniformLocation(name);
	if (loc != -1) glUniform2fv(loc, 1, glm::value_ptr(v));
}

void Shader::SetUniform(const std::string& name, const glm::vec3& v) const {
	int loc = getUniformLocation(name);
	if (loc != -1) glUniform3fv(loc, 1, glm::value_ptr(v));
}

void Shader::SetUniform(const std::string& name, const glm::mat4& m) const {
	int loc = getUniformLocation(name);
	if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}

unsigned int Shader::compileShader(unsigned int type, const std::string& src) {
	unsigned int sh = glCreateShader(type);
	const char* s = src.c_str();
	glShaderSource(sh, 1, &s, nullptr);
	glCompileShader(sh);

	int success;
	glGetShaderiv(sh, GL_COMPILE_STATUS, &success);
	if (!success) {
		int len;
		glGetShaderiv(sh, GL_INFO_LOG_LENGTH, &len);
		std::vector<char> log(len);
		glGetShaderInfoLog(sh, len, &len, log.data());
		std::cerr << "Shader compile error ("
			<< (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT")
			<< "):\n" << log.data() << "\n";
	}
	return sh;
}

std::string Shader::LoadFromFile(const std::string& path) {
	std::ifstream file(path);
	if (!file) {
		std::cerr << "Error loading shader file: " << path << "\n";
		return "";
	}
	std::cout << "Loaded shader: " << path << "\n";  // <- agregá esto
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}