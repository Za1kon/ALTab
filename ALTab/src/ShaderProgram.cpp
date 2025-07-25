// File: ShaderProgram.cpp
#include "ShaderProgram.hpp"
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

ShaderProgram::ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) {
	std::string vertSrc = loadSource(vertexPath);
	std::string fragSrc = loadSource(fragmentPath);
	unsigned int vertID = compileShader(GL_VERTEX_SHADER, vertSrc);
	unsigned int fragID = compileShader(GL_FRAGMENT_SHADER, fragSrc);
	ID_ = glCreateProgram();
	glAttachShader(ID_, vertID);
	glAttachShader(ID_, fragID);
	glLinkProgram(ID_);
	// Revisar errores de link
	int success;
	glGetProgramiv(ID_, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(ID_, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertID);
	glDeleteShader(fragID);
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(ID_);
}

void ShaderProgram::use() const {
	glUseProgram(ID_);
}

void ShaderProgram::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID_, name.c_str()), (int)value);
}

void ShaderProgram::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID_, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID_, name.c_str()), value);
}

void ShaderProgram::setVec3(const std::string& name, const glm::vec3& vec) const {
	glUniform3fv(glGetUniformLocation(ID_, name.c_str()), 1, &vec[0]);
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const {
	glUniformMatrix4fv(glGetUniformLocation(ID_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

std::string ShaderProgram::loadSource(const std::string& filePath) const {
	std::ifstream file(filePath);
	std::stringstream ss;
	ss << file.rdbuf();
	return ss.str();
}

unsigned int ShaderProgram::compileShader(unsigned int type, const std::string& source) const {
	unsigned int shader = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return shader;
}
