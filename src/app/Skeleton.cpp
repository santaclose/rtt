#include "Skeleton.h"
#include <iostream>

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
/*
static glm::mat4 AiToGLMMat4(const aiMatrix4x4& in_mat)
{
	glm::mat4 tmp;
	tmp[0][0] = (float)in_mat.a1;
	tmp[1][0] = (float)in_mat.b1;
	tmp[2][0] = (float)in_mat.c1;
	tmp[3][0] = (float)in_mat.d1;

	tmp[0][1] = (float)in_mat.a2;
	tmp[1][1] = (float)in_mat.b2;
	tmp[2][1] = (float)in_mat.c2;
	tmp[3][1] = (float)in_mat.d2;

	tmp[0][2] = (float)in_mat.a3;
	tmp[1][2] = (float)in_mat.b3;
	tmp[2][2] = (float)in_mat.c3;
	tmp[3][2] = (float)in_mat.d3;

	tmp[0][3] = (float)in_mat.a4;
	tmp[1][3] = (float)in_mat.b4;
	tmp[2][3] = (float)in_mat.c4;
	tmp[3][3] = (float)in_mat.d4;
	return tmp;
}*/

void Skeleton::SkeletonFromAssimp(const aiNode* node, std::unordered_map<std::string, bool>& necessityMap, Bone* parentBone, const aiMesh* mesh)
{
	Bone* currentBone = parentBone;
	if (necessityMap[node->mName.data])
	{
		std::cout << "node " << node->mName.data << " is part of the skeleton\n";
		currentBone = new Bone();
		parentBone->children.push_back(currentBone);
		currentBone->name = node->mName.data;
		aiMatrix4x4t assimpOffsetMatrix = FindAssimpBone(currentBone->name, *mesh)->mOffsetMatrix;
		currentBone->offsetMatrix = (*(glm::mat4*) & assimpOffsetMatrix.Transpose());
	}
	else
		std::cout << "node " << node->mName.data << " is NOT part of the skeleton\n";
	for (unsigned int i = 0; i < node->mNumChildren; i++)
		SkeletonFromAssimp(node->mChildren[i], necessityMap, currentBone, mesh);
}

Skeleton::Skeleton(const std::string& filePath)
{
	m_rootBone = new Bone();
	m_rootBone->offsetMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_rootBone->name = "root";

	Bone* chestBone = new Bone();
	chestBone->offsetMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	chestBone->name = "chest";

	Bone* headBone = new Bone();
	headBone->offsetMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	headBone->name = "head";

	Bone* leftBone = new Bone();
	leftBone->offsetMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 1.0f, 0.0f));
	leftBone->name = "left";

	Bone* rightBone = new Bone();
	rightBone->offsetMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
	rightBone->name = "right";

	Bone* leftUpBone = new Bone();
	leftUpBone->offsetMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	leftUpBone->name = "leftUp";

	Bone* rightUpBone = new Bone();
	rightUpBone->offsetMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));// *glm::rotate(glm::mat4(1.0f), 3.1415926f, glm::vec3(0.0f, 1.0f, 0.0f));
	rightUpBone->name = "rightUp";

	m_rootBone->children.push_back(chestBone);

	chestBone->children.push_back(leftBone);
	chestBone->children.push_back(rightBone);
	chestBone->children.push_back(headBone);

	rightBone->children.push_back(rightUpBone);
	leftBone->children.push_back(leftUpBone);


	/*m_rootBone = new Bone();
	m_rootBone->offsetMatrix = glm::mat4(1.0f);
	m_rootBone->name = "root";

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
	SkeletonFromAssimp(assimpScene->mRootNode, necessityMap, m_rootBone, targetMesh);
	std::cout << "skeleton loaded\n";*/
}
/*
void Skeleton::ComputePoseTransformations(Bone* root)
{
	glm::mat4 parentPoseTransform = root->parent ? root->parent->poseTransformation : glm::mat4(1.0f);
	glm::vec3 currentUp = parentPoseTransform * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec3 rotationAxis = glm::cross(glm::normalize(root->tail - root->head), currentUp);
	root->poseTransformation =
		glm::translate(glm::mat4(1.0f), -root->head + root->tail) *
		glm::rotate(glm::mat4(1.0f), 3.1415926f, glm::vec3(0.0f, 1.0f, 0.0f));
	for (Bone* b : root->children)
		ComputePoseTransformations(b);
}*/

void Skeleton::FreeBones(Bone* root)
{
	for (Bone* c : root->children)
		FreeBones(c);
	delete root;
}

Skeleton::~Skeleton()
{
	FreeBones(m_rootBone);
}
