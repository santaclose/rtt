#pragma once
#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Model
{
	unsigned int m_gl_vertexBuffer;
	unsigned int m_gl_indexBuffer;
	unsigned int m_gl_vao;
	std::vector<glm::vec3> m_vertexVector;
	std::vector<unsigned int> m_indexVector;

	void CompleteFromVectors();

public:
	void CreateFromOBJ(const std::string& filePath, float size = 1.0);
	void Draw();
};