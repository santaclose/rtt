#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Bone.h"

struct RootBone {

	Bone* bone;
	glm::vec3 offset;
};

class Skeleton
{
public:
	std::vector<RootBone> m_rootBones;
	//Bone* m_rootBone = nullptr;

private:
	//void ComputePoseTransformations(Bone* root);
	void FreeBones(Bone* root);
	//void SkeletonFromAssimp(const aiNode* node, std::unordered_map<std::string, bool>& necessityMap, Bone* parentBone, const aiMesh* mesh);

public:
	Skeleton();
	Skeleton(const std::string& filePath);
	~Skeleton();
};