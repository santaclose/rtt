#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
#include <vector>

struct Bone
{
	std::string name;

	Bone* parent = nullptr;
	std::vector<Bone*> children;

	float size;
	glm::fquat rotation;

	glm::mat4 GetMatrix() const;
};