#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.hpp"

class Wall {
public:
	Wall(const glm::vec3& position, const glm::vec2& size, const std::string& vertPath, const std::string& fragPath);
	~Wall();

	void Draw(const glm::mat4& projection, const glm::mat4& view);

private:
	GLuint vao, vbo, ebo;
	Shader* shader;
	glm::vec3 position;
	glm::vec2 size;

	void Init();
};