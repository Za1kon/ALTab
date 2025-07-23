// ===== Scene.hpp =====
#pragma once
#include <glm/glm.hpp>

class Scene {
public:
	virtual ~Scene() = default;
	virtual void Init() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(const glm::mat4& projection, const glm::mat4& view) = 0;
};