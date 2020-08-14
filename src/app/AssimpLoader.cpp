#include "AssimpLoader.h"
#include <iostream>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtx/matrix_decompose.hpp>

const aiBone* FindAssimpBone(const std::string& name, const aiMesh& mesh)
{
	for (unsigned int i = 0; i < mesh.mNumBones; i++)
	{
		if (mesh.mBones[i]->mName.data == name)
			return mesh.mBones[i];
	}
	return nullptr;
}

const aiNode* FindAssimpNode(const std::string& name, const aiNode& root)
{
	if (name == root.mName.data)
		return &root;
	else if (root.mNumChildren > 0)
	{
		for (unsigned int i = 0; i < root.mNumChildren; i++)
		{
			const aiNode* value = FindAssimpNode(name, *root.mChildren[i]);
			if (value)
				return value;
		}
		return nullptr;
	}
	else
		return nullptr;
}

void PrintAllNodes(const aiNode* node, const std::string& tabs, std::unordered_map<std::string, bool>& necessityMap)
{
	std::cout << tabs << node->mName.data;
	if (necessityMap[node->mName.data])
		std::cout << " [PART OF THE SKELETON]";
	std::cout << std::endl;
	aiMatrix4x4t currentMat = node->mTransformation;
	currentMat = currentMat.Transpose();
	std::cout << tabs << currentMat.a1 << ' ' << currentMat.a2 << ' ' << currentMat.a3 << ' ' << currentMat.a4 << std::endl;
	std::cout << tabs << currentMat.b1 << ' ' << currentMat.b2 << ' ' << currentMat.b3 << ' ' << currentMat.b4 << std::endl;
	std::cout << tabs << currentMat.c1 << ' ' << currentMat.c2 << ' ' << currentMat.c3 << ' ' << currentMat.c4 << std::endl;
	std::cout << tabs << currentMat.d1 << ' ' << currentMat.d2 << ' ' << currentMat.d3 << ' ' << currentMat.d4 << std::endl;

	std::string tabsNext = tabs + "    ";
	for (int i = 0; i < node->mNumChildren; i++)
		PrintAllNodes(node->mChildren[i], tabsNext, necessityMap);
}

void LoadJoints(Joint* joint, aiNode* node, std::unordered_map<std::string, bool>& necessityMap, bool parentIsSkeleton = false)
{
	Joint* currentJoint = joint;

	bool isSkeletonNode = parentIsSkeleton || necessityMap[node->mName.data];

	if (isSkeletonNode)
	{
		std::cout << "Loading joint " << node->mName.data << std::endl;
		currentJoint = new Joint();
		currentJoint->name = node->mName.data;
		currentJoint->transform = *((glm::mat4*)(&(node->mTransformation.Transpose())));
		joint->children.push_back(currentJoint);
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
		LoadJoints(currentJoint, node->mChildren[i], necessityMap, isSkeletonNode);
}

void AssimpLoader::LoadSkeleton(const std::string& filePath, Skeleton& skeleton)
{
	skeleton.rootJoint = new Joint();
	skeleton.rootJoint->name = "rttRoot";
	skeleton.rootJoint->transform = glm::mat4(1.0f);

	Assimp::Importer importer;
	auto assimpScene = importer.ReadFile(filePath.c_str(), NULL);

	if (!assimpScene || assimpScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !assimpScene->mRootNode) // if is Not Zero
	{
		std::cout << "[ASSIMP ERROR] " << importer.GetErrorString() << std::endl;
		return;
	}

	if (assimpScene->mNumMeshes != 1)
		std::cout << "[Skeleton loader] More than one mesh object found\n";
	auto targetMesh = assimpScene->mMeshes[0];

	std::unordered_map<std::string, bool> necessityMap; // stores which nodes are necessary for the skeleton
	unsigned int boneCount = targetMesh->mNumBones;
	for (unsigned int i = 0; i < boneCount; i++)
	{
		if (FindAssimpNode(targetMesh->mBones[i]->mName.data, *assimpScene->mRootNode))
			necessityMap[targetMesh->mBones[i]->mName.data] = true;
		else
			necessityMap[targetMesh->mBones[i]->mName.data] = false;
	}

	std::cout << "PRINTING ALL NODES\n";
	PrintAllNodes(assimpScene->mRootNode, "", necessityMap);

	std::cout << "LOADING JOINTS\n";
	LoadJoints(skeleton.rootJoint, assimpScene->mRootNode, necessityMap);

	std::cout << "skeleton loaded\n";
}