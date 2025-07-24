// ===== Platform.cpp =====
#include "Platform.hpp"

Platform::Platform(float size, int res) : vao(0), vbo(0), ebo(0), indexCount(0) {
	GenerateMesh(size, res);
}

Platform::~Platform() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void Platform::GenerateMesh(float size, int res) {
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	for (int z = 0; z <= res; ++z) {
		for (int x = 0; x <= res; ++x) {
			float xf = static_cast<float>(x) / res;
			float zf = static_cast<float>(z) / res;
			Vertex v;
			v.position = glm::vec3((xf - 0.5f) * size, 0.0f, (zf - 0.5f) * size);
			v.uv = glm::vec2(xf, zf);
			vertices.push_back(v);
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
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	// UV attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	glBindVertexArray(0);
}

void Platform::Draw() const {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
