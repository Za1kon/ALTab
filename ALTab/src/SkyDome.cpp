// ===== SkyDome.cpp =====
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "utils/stb_image.h"
#include "SkyDome.hpp"
#include <vector>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#ifndef GL_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#endif
#ifndef GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
#endif

SkyDome::SkyDome(const std::string& hdrPath, float radius)
	: vao(0), vbo(0), ebo(0), textureID(0), shader(nullptr), radius(radius) {
	GenerateSphere(32, 16);
	LoadHDRTexture(hdrPath);
	shader = new Shader("shaders/skydome.vert", "shaders/skydome.frag");
}

SkyDome::~SkyDome() {
	glDeleteTextures(1, &textureID);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
	delete shader;
}

void SkyDome::GenerateSphere(unsigned int slices, unsigned int stacks) {
	std::vector<glm::vec3> positions;
	std::vector<unsigned int> indices;
	for (unsigned int y = 0; y <= stacks; ++y) {
		float v = float(y) / stacks;
		float theta = v * glm::pi<float>();
		for (unsigned int x = 0; x <= slices; ++x) {
			float u = float(x) / slices;
			float phi = u * glm::two_pi<float>();
			positions.emplace_back(
				sin(theta) * cos(phi),
				cos(theta),
				sin(theta) * sin(phi)
			);
		}
	}
	for (unsigned int y = 0; y < stacks; ++y) {
		for (unsigned int x = 0; x < slices; ++x) {
			unsigned int i0 = y * (slices + 1) + x;
			unsigned int i1 = i0 + slices + 1;
			indices.insert(indices.end(), { i0, i1, i0 + 1, i1, i1 + 1, i0 + 1 });
		}
	}
	indexCount = static_cast<GLsizei>(indices.size());
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glBindVertexArray(0);
}

void SkyDome::LoadHDRTexture(const std::string& path) {
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	float* data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);
	if (!data) {
		std::cerr << "Failed to load HDR image: " << path << "\n";
		return;
	}
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	float maxAniso = 0.0f;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);
	stbi_image_free(data);
}

void SkyDome::Draw(const glm::mat4& projection, const glm::mat4& view) {
	glDepthFunc(GL_LEQUAL); // asegura que se renderice detrás de todo

	shader->Use();

	// Eliminar traslación de la vista (mantiene el skybox centrado en cámara)
	glm::mat4 viewNoTranslation = glm::mat4(glm::mat3(view));
	glm::mat4 mvp = projection * viewNoTranslation;

	shader->SetUniform("uMVP", mvp);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	shader->SetUniform("uHDR", 0);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS); // restaurar valor por defecto
}