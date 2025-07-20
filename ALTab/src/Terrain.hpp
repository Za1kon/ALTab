#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class Terrain {
public:
	Terrain(float size = 10.0f, int resolution = 100);
	~Terrain();

	void Draw() const;

private:
	GLuint vao, vbo, ebo;
	GLsizei indexCount;

	void GenerateMesh(float size, int resolution);
};