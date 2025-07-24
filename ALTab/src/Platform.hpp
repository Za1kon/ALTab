// ===== Platform.hpp =====
#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Platform {
public:
	Platform(float size = 10.0f, int resolution = 100);
	~Platform();

	void Draw() const;

private:
	struct Vertex {
		glm::vec3 position;
		glm::vec2 uv;
	};

	GLuint vao, vbo, ebo;
	GLsizei indexCount;

	void GenerateMesh(float size, int resolution);
};