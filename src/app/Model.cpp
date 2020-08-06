#include "Model.h"
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>

void Model::CompleteFromVectors()
{
	glGenVertexArrays(1, &m_gl_vao);
	glGenBuffers(1, &m_gl_vertexBuffer);
	glGenBuffers(1, &m_gl_indexBuffer);

	glBindVertexArray(m_gl_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_gl_vertexBuffer);

	// update vertices
	glBufferData(GL_ARRAY_BUFFER, m_vertexVector.size() * sizeof(glm::vec3), &m_vertexVector[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_gl_indexBuffer);
	// update indices to draw
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexVector.size() * sizeof(unsigned int), &m_indexVector[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	glBindVertexArray(0);
}

void Model::CreateFromOBJ(const std::string& filePath, float size)
{
	std::ifstream infile(filePath);
	if (infile.fail())
		std::cout << "Could not read model file: " << filePath << "\n";

	std::string line;

	glm::vec3 currentVertexPosition;
	while (std::getline(infile, line))
	{
		if (line.find("v ") == 0)
		{
			int p = 2;
			int q = 2;
			for (; line[q] != ' '; q++);
			currentVertexPosition.x = size * std::stof(line.substr(p, q - p));
			p = q = q + 1;
			for (; line[q] != ' '; q++);
			currentVertexPosition.y = size * std::stof(line.substr(p, q - p));
			p = q = q + 1;
			for (; q < line.size() && line[q] != '\n'; q++);
			currentVertexPosition.z = size * std::stof(line.substr(p, q - p));
			m_vertexVector.emplace_back(currentVertexPosition);
		}
		else if (line.find("f ") == 0)
		{
			for (int i = 1; i < line.size(); i++) // from the space character
			{
				if (line[i] == ' ')
				{
					int q = i = i + 1;
					for (; line[q] != '/'; q++);
					m_indexVector.push_back(std::stoi(line.substr(i, q - i)) - 1);
					/*i = q = q + 1;
					for (; line[q] != '/'; q++);*/
					//nVtx.coordID = std::stoi(line.substr(i, q - i));
					//newPolygon.verts.push_back(nVtx);
				}
			}
		}
	}

	std::cout << "finished loading obj\n\tvertex count: " << m_vertexVector.size() << std::endl;
	CompleteFromVectors();
}

void Model::Draw()
{
	glBindVertexArray(m_gl_vao);
	glDrawElements(GL_TRIANGLES, m_indexVector.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}
