#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>  // for glm::value_ptr

class Shader {
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath, bool fromFile = true);
	~Shader();

	void Use() const;

	// Uniform overloads
	void SetUniform(const std::string& name, float v) const;
	void SetUniform(const std::string& name, int v)   const;  // for sampler uniforms
	void SetUniform(const std::string & name, const glm::vec2 & v) const;
	void SetUniform(const std::string& name, const glm::vec3& v) const;
	void SetUniform(const std::string& name, const glm::mat4& m) const;

private:
	unsigned int ID;
	unsigned int compileShader(unsigned int type, const std::string& src);
	int getUniformLocation(const std::string& name) const;
	std::string LoadFromFile(const std::string& path);
};

#endif // SHADER_HPP