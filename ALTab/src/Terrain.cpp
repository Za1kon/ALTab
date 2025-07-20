#include "Terrain.hpp"

Terrain::Terrain(float size, int res) : vao(0), vbo(0), ebo(0), indexCount(0) {
	GenerateMesh(size, res);
}

Terrain::~Terrain() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void Terrain::GenerateMesh(float size, int res) {
	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;

	for (int z = 0; z <= res; ++z) {
		for (int x = 0; x <= res; ++x) {
			float xf = (float)x / res;
			float zf = (float)z / res;
			vertices.emplace_back(
				(xf - 0.5f) * size,   // X
				0.0f,                 // Y
				(zf - 0.5f) * size    // Z
			);
		}
	}

	for (int z = 0; z < res; ++z) {
		for (int x = 0; x < res; ++x) {
			int start = z * (res + 1) + x;
			indices.push_back(start);
			indices.push_back(start + 1);
			indices.push_back(start + res + 1);
			indices.push_back(start + 1);
			indices.push_back(start + res + 2);
			indices.push_back(start + res + 1);
		}
	}

	indexCount = static_cast<GLsizei>(indices.size());

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	glBindVertexArray(0);
}

void Terrain::Draw() const {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}