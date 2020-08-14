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

/*void SkeletonFromAssimp(const aiNode* node, std::unordered_map<std::string, bool>& necessityMap, Bone* parentBone, const aiMesh* mesh)
{
	Bone* currentBone = parentBone;
	if (node->mParent && necessityMap[node->mParent->mName.data])
	{
		std::cout << "node " << node->mName.data << " is part of the skeleton\n";
		currentBone = new Bone();
		parentBone->children.push_back(currentBone);
		currentBone->name = node->mName.data;
		aiMatrix4x4t childT = node->mTransformation;
		aiMatrix4x4t parentT = node->mParent->mTransformation;
		parentT = parentT.Transpose();
		childT = childT.Transpose();
		std::cout << childT.a1 << ' ' << childT.a2 << ' ' << childT.a3 << ' ' << childT.a4 << std::endl;
		std::cout << childT.b1 << ' ' << childT.b2 << ' ' << childT.b3 << ' ' << childT.b4 << std::endl;
		std::cout << childT.c1 << ' ' << childT.c2 << ' ' << childT.c3 << ' ' << childT.c4 << std::endl;
		std::cout << childT.d1 << ' ' << childT.d2 << ' ' << childT.d3 << ' ' << childT.d4 << std::endl;

		glm::mat4 transformation;
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;

		transformation = *((glm::mat4*)(&parentT));
		glm::decompose(transformation, scale, rotation, translation, skew, perspective);
		currentBone->rotation = rotation;

		transformation = *((glm::mat4*)(&childT));
		glm::decompose(transformation, scale, rotation, translation, skew, perspective);
		currentBone->size = glm::length(translation);

		std::cout << "SIZE OF THE BONE: " << currentBone->size << std::endl;
	}
	else if (node->mParent && !necessityMap[node->mParent->mName.data] && necessityMap[node->mName.data])
	{
		std::cout << "node " << node->mName.data << " is a root bone of the skeleton\n";
		aiMatrix4x4t transform = node->mTransformation;
		transform.Transpose();
		std::cout << transform.a1 << ' ' << transform.a2 << ' ' << transform.a3 << ' ' << transform.a4 << std::endl;
		std::cout << transform.b1 << ' ' << transform.b2 << ' ' << transform.b3 << ' ' << transform.b4 << std::endl;
		std::cout << transform.c1 << ' ' << transform.c2 << ' ' << transform.c3 << ' ' << transform.c4 << std::endl;
		std::cout << transform.d1 << ' ' << transform.d2 << ' ' << transform.d3 << ' ' << transform.d4 << std::endl;

	}
	else
		std::cout << "node " << node->mName.data << " is NOT part of the skeleton\n";
	for (unsigned int i = 0; i < node->mNumChildren; i++)
		SkeletonFromAssimp(node->mChildren[i], necessityMap, currentBone, mesh);
}*/

void LoadRootBones(std::vector<const aiNode*>& nodes, Skeleton& skeleton, const aiNode* node, const aiMesh* mesh, std::unordered_map<std::string, bool>& necessityMap)
{
	/*for (int i = 0; i < node->mNumChildren; i++)
	{
		const aiNode* child = node->mChildren[i];
		for (int j = 0; j < child->mNumChildren; j++)
		{
			const aiNode* childChild = child->mChildren[j];
			if (necessityMap[childChild->mName.data] && necessityMap[child->mName.data] && !necessityMap[node->mName.data])
			{
				std::cout << "node " << child->mName.data << " -> " << childChild->mName.data << " is a root bone\n";
				aiMatrix4x4t transform = child->mTransformation;
				transform.Transpose();

				glm::mat4 transformation;
				glm::vec3 scale;
				glm::quat rotation;
				glm::vec3 translation;
				glm::vec3 skew;
				glm::vec4 perspective;

				transformation = *((glm::mat4*)(&transform));
				glm::decompose(transformation, scale, rotation, translation, skew, perspective);

				skeleton.m_rootBones.push_back({ new Bone(), translation });
				nodes.push_back(childChild);
				skeleton.m_rootBones.back().bone->rotation = rotation;

				transform = childChild->mTransformation;
				transform.Transpose();

				transformation = *((glm::mat4*)(&transform));
				glm::decompose(transformation, scale, rotation, translation, skew, perspective);

				skeleton.m_rootBones.back().bone->size = glm::length(translation);
			}
		}
		LoadRootBones(nodes, skeleton, child, mesh, necessityMap);
	}*/
	
	if ((!node->mParent || !necessityMap[node->mParent->mName.data]) && necessityMap[node->mName.data])
	{
		for (int i = 0; i < node->mNumChildren; i++)
		{
			std::cout << "node " << node->mName.data << " -> " << node->mChildren[i]->mName.data << " is a root bone of the skeleton\n";
			aiMatrix4x4t transform = node->mTransformation;
			transform.Transpose();

			std::cout << transform.a1 << ' ' << transform.a2 << ' ' << transform.a3 << ' ' << transform.a4 << std::endl;
			std::cout << transform.b1 << ' ' << transform.b2 << ' ' << transform.b3 << ' ' << transform.b4 << std::endl;
			std::cout << transform.c1 << ' ' << transform.c2 << ' ' << transform.c3 << ' ' << transform.c4 << std::endl;
			std::cout << transform.d1 << ' ' << transform.d2 << ' ' << transform.d3 << ' ' << transform.d4 << std::endl;

			glm::mat4 transformation;
			glm::vec3 scale;
			glm::quat rotation;
			glm::vec3 translation;
			glm::vec3 skew;
			glm::vec4 perspective;

			transformation = *((glm::mat4*)(&transform));
			glm::decompose(transformation, scale, rotation, translation, skew, perspective);

			skeleton.m_rootBones.push_back({ new Bone() });
			nodes.push_back(node->mChildren[i]);
			skeleton.m_rootBones.back().bone->rotation = rotation;
			skeleton.m_rootBones.back().offset = translation;

			transform = node->mChildren[i]->mTransformation;
			transform.Transpose();

			transformation = *((glm::mat4*)(&transform));
			glm::decompose(transformation, scale, rotation, translation, skew, perspective);

			skeleton.m_rootBones.back().bone->size = glm::length(translation);
		}
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
		LoadRootBones(nodes, skeleton, node->mChildren[i], mesh, necessityMap);
}

void LoadBoneTree(const aiNode* node, std::unordered_map<std::string, bool>& necessityMap, Bone* parentBone, const aiMesh* mesh)
{
	for (int i = 0; i < node->mNumChildren; i++)
	{
		const aiNode* child = node->mChildren[i];
		std::cout << "node " << node->mName.data << " -> " << child->mName.data << " is part of the skeleton\n";
		Bone* currentBone = new Bone();
		parentBone->children.push_back(currentBone);
		currentBone->name = child->mName.data;

		aiMatrix4x4t childT = child->mTransformation;
		aiMatrix4x4t parentT = node->mTransformation;
		parentT = parentT.Transpose();
		childT = childT.Transpose();

		std::cout << childT.a1 << ' ' << childT.a2 << ' ' << childT.a3 << ' ' << childT.a4 << std::endl;
		std::cout << childT.b1 << ' ' << childT.b2 << ' ' << childT.b3 << ' ' << childT.b4 << std::endl;
		std::cout << childT.c1 << ' ' << childT.c2 << ' ' << childT.c3 << ' ' << childT.c4 << std::endl;
		std::cout << childT.d1 << ' ' << childT.d2 << ' ' << childT.d3 << ' ' << childT.d4 << std::endl;

		glm::mat4 transformation;
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;

		transformation = *((glm::mat4*)(&parentT));
		glm::decompose(transformation, scale, rotation, translation, skew, perspective);
		currentBone->rotation = rotation;

		transformation = *((glm::mat4*)(&childT));
		glm::decompose(transformation, scale, rotation, translation, skew, perspective);
		currentBone->size = glm::length(translation);

		std::cout << "SIZE OF THE BONE: " << currentBone->size << std::endl;
		LoadBoneTree(child, necessityMap, currentBone, mesh);
	}

	/*Bone* currentBone = parentBone;
	if (node->mParent && necessityMap[node->mParent->mName.data])
	{
		std::cout << "node " << node->mName.data << " is part of the skeleton\n";
		currentBone = new Bone();
		parentBone->children.push_back(currentBone);
		currentBone->name = node->mName.data;
		aiMatrix4x4t childT = node->mTransformation;
		aiMatrix4x4t parentT = node->mParent->mTransformation;
		parentT = parentT.Transpose();
		childT = childT.Transpose();
		std::cout << childT.a1 << ' ' << childT.a2 << ' ' << childT.a3 << ' ' << childT.a4 << std::endl;
		std::cout << childT.b1 << ' ' << childT.b2 << ' ' << childT.b3 << ' ' << childT.b4 << std::endl;
		std::cout << childT.c1 << ' ' << childT.c2 << ' ' << childT.c3 << ' ' << childT.c4 << std::endl;
		std::cout << childT.d1 << ' ' << childT.d2 << ' ' << childT.d3 << ' ' << childT.d4 << std::endl;

		glm::mat4 transformation;
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;

		transformation = *((glm::mat4*)(&parentT));
		glm::decompose(transformation, scale, rotation, translation, skew, perspective);
		currentBone->rotation = rotation;

		transformation = *((glm::mat4*)(&childT));
		glm::decompose(transformation, scale, rotation, translation, skew, perspective);
		currentBone->size = glm::length(translation);

		std::cout << "SIZE OF THE BONE: " << currentBone->size << std::endl;
	}
	else
		std::cout << "node " << node->mName.data << " is NOT part of the skeleton\n";
	for (unsigned int i = 0; i < node->mNumChildren; i++)
		LoadBoneTree(node->mChildren[i], necessityMap, currentBone, mesh);*/
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


void AssimpLoader::LoadSkeleton(const std::string& filePath, Skeleton& skeleton)
{
	/*skeleton.m_rootBone = new Bone();
	skeleton.m_rootBone->size = 0.0f;
	skeleton.m_rootBone->rotation = glm::fquat(1.0f, 0.0f, 0.0f, 0.0f);
	skeleton.m_rootBone->name = "root";*/

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
	//SkeletonFromAssimp(assimpScene->mRootNode, necessityMap, skeleton.m_rootBone, targetMesh);

	std::cout << "PRINTING ALL NODES\n";
	PrintAllNodes(assimpScene->mRootNode, "", necessityMap);

	std::cout << "LOADING ROOT BONES\n";
	std::vector<const aiNode*> aiNodes;
	LoadRootBones(aiNodes, skeleton, assimpScene->mRootNode, targetMesh, necessityMap);

	std::cout << "LOADING BONE TREES\n";
	for (int i = 0; i < aiNodes.size(); i++)
		LoadBoneTree(aiNodes[i], necessityMap, skeleton.m_rootBones[i].bone, targetMesh);

	std::cout << "skeleton loaded\n";
}