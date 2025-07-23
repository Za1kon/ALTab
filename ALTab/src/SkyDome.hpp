// ===== SkyDome.hpp =====
#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include "Shader.hpp"

class SkyDome {
public:
	SkyDome(const std::string& hdrPath, float radius = 50.0f);
	~SkyDome();

	void Draw(const glm::mat4& projection, const glm::mat4& view);

private:
	GLuint vao, vbo, ebo;
	GLuint textureID;
	GLsizei indexCount;
	Shader* shader;
	float radius;

	void GenerateSphere(unsigned int slices, unsigned int stacks);
	void LoadHDRTexture(const std::string& path);
};