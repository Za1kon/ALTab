#include "Wall.hpp"

Wall::Wall(const glm::vec3& pos, const glm::vec2& sz,
	const std::string& vertPath,
	const std::string& fragPath)
	: vao(0), vbo(0), ebo(0), shader(nullptr),
	position(pos), size(sz)
{
	shader = new Shader(vertPath, fragPath);
	// Default style (puedes ajustar)
	style = {
		{0.005f,0.005f,0.03f},  // bgColor
		{0.02f,0.1f,0.4f},      // lineColor
		{0.01f,0.02f,0.08f},    // accentColor
		0.02f,                  // borderSize
		{16.0f,8.0f},           // gridCount
		0.002f,                 // lineWidth
		0.05f                   // glowWidth
	};
	Init();
}

Wall::~Wall() {
	delete shader;
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void Wall::Init() {
	float w = size.x * 0.5f;
	float h = size.y * 0.5f;
	float vertices[] = {
		-w, -h, 0.0f,   0.0f, 0.0f,
		 w, -h, 0.0f,   1.0f, 0.0f,
		 w,  h, 0.0f,   1.0f, 1.0f,
		-w,  h, 0.0f,   0.0f, 1.0f,
	};
	unsigned int indices[] = { 0,1,2, 2,3,0 };

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
}

void Wall::Draw(const glm::mat4& projection, const glm::mat4& view) {
	glm::mat4 model = glm::translate(glm::mat4(1.0f), position);

	shader->Use();
	// matrices
	shader->SetUniform("uModel", model);
	shader->SetUniform("uView", view);
	shader->SetUniform("uProjection", projection);

	// style uniforms
	shader->SetUniform("bgColor", style.bgColor);
	shader->SetUniform("lineColor", style.lineColor);
	shader->SetUniform("accentColor", style.accentColor);
	shader->SetUniform("borderSize", style.borderSize);
	shader->SetUniform("gridCount", style.gridCount);
	shader->SetUniform("lineWidth", style.lineWidth);
	shader->SetUniform("glowWidth", style.glowWidth);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}