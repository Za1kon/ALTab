#include "Renderer.hpp"
#include <glad/glad.h>

void Renderer::Draw(const Mesh& mesh, const Shader& shader, const glm::mat4& mvp) const {
    shader.Use();
    shader.SetUniform("uMVP", mvp);
    mesh.Draw();
}