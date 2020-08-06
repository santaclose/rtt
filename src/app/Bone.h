#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>

struct Bone
{
	std::string name;

	Bone* parent = nullptr;
	std::vector<Bone*> children;

	glm::mat4 offsetMatrix = glm::mat4(1.0f);
	glm::mat4 animOffsetMatrix = glm::mat4(1.0f);
};