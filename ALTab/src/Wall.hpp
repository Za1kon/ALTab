#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "Shader.hpp"

struct WallStyle {
	glm::vec3 bgColor;
	glm::vec3 lineColor;
	glm::vec3 accentColor;
	float borderSize;
	glm::vec2 gridCount;
	float lineWidth;
	float glowWidth;
};

class Wall {
public:
	Wall(const glm::vec3& pos, const glm::vec2& sz,
		const std::string& vertPath,
		const std::string& fragPath);
	~Wall();

	void Init();
	void Draw(const glm::mat4& projection, const glm::mat4& view);
	void SetStyle(const WallStyle& s) { style = s; }

private:
	GLuint vao, vbo, ebo;
	Shader* shader;
	glm::vec3 position;
	glm::vec2 size;
	WallStyle style;
};