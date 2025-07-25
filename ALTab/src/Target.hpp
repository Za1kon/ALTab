// ==== Target.hpp ====
#pragma once
#include <glm/glm.hpp>
#include "ShaderProgram.hpp"

class Target {
public:
	Target();
	~Target();

	void setPosition(const glm::vec3& pos);
	void setScale(float s);
	void setActive(bool a);
	bool isActive() const;

	void draw(const ShaderProgram& shader) const;

private:
	glm::vec3 position_;
	float scale_;
	bool active_;

	unsigned int vao_, vbo_;
	int sphereVertexCount = 0;

	void initMesh();
};