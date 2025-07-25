// File: ShaderProgram.hpp
#pragma once

#include <string>
#include <glm/glm.hpp>

// Clase auxiliar para cargar y usar shaders GLSL
class ShaderProgram {
public:
	ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);
	~ShaderProgram();

	void use() const;
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3(const std::string& name, const glm::vec3& vec) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;

private:
	unsigned int ID_;  // ID del programa de shader
	std::string loadSource(const std::string& filePath) const;
	unsigned int compileShader(unsigned int type, const std::string& source) const;
};