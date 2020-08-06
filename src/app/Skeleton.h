#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Bone.h"

class Skeleton
{
public:
	Bone* m_rootBone;

private:
	void ComputePoseTransformations(Bone* root);
	void FreeBones(Bone* root);
	void SkeletonFromAssimp(const aiNode* node, std::unordered_map<std::string, bool>& necessityMap, Bone* parentBone, const aiMesh* mesh);

public:
	Skeleton(const std::string& filePath);
	~Skeleton();
};