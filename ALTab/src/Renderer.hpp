#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Mesh.hpp"
#include "Shader.hpp"
#include <glm/glm.hpp>

class Renderer {
public:
    // Dibuja la malla usando el shader y la matriz MVP dada
    void Draw(const Mesh& mesh, const Shader& shader, const glm::mat4& mvp) const;
};

#endif // RENDERER_HPP