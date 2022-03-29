#pragma once

#include <string>
#include <glm/glm.hpp>

class Shader {
public:
	Shader(unsigned int id);
	~Shader();

	void use();
	void uniformMatrix(std::string name, glm::mat4 matrix);
	void uniform1i(std::string name, int x);
	void uniform1f(std::string name, float x);
	void uniform2f(std::string name, float x, float y);
	void uniform3f(std::string name, float x, float y, float z);
private:
	unsigned int m_id;

	unsigned int m_getUniformLocation(std::string name);
};

extern Shader* load_shader(std::string vertexFile, std::string fragmentFile);