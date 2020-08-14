#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

struct Joint {

	glm::mat4 transform;
	std::string name;
	std::vector<Joint*> children;
};

class Skeleton
{
public:
	Joint* rootJoint = nullptr;

private:
	void FreeJoints(Joint* root);

public:
	Skeleton(const std::string& filePath);
	~Skeleton();
};