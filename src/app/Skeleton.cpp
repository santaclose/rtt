#include "Skeleton.h"
#include <iostream>
#include "AssimpLoader.h"

Skeleton::Skeleton()
{
	glm::mat4 boneMatrix;

	m_rootBones.push_back({ new Bone(), glm::vec3(0.0f, 0.0f, 0.0f) });
	m_rootBones.back().bone->size = 1.0f;
	m_rootBones.back().bone->rotation = glm::fquat(1.0f, 0.0f, 0.0f, 0.0f);
	m_rootBones.back().bone->name = "root";

	std::cout << m_rootBones[0].bone->name << std::endl;
	boneMatrix = m_rootBones[0].bone->GetMatrix();
	std::cout << boneMatrix[0][0] << ' ' << boneMatrix[0][1] << ' ' << boneMatrix[0][2] << ' ' << boneMatrix[0][3] << std::endl;
	std::cout << boneMatrix[1][0] << ' ' << boneMatrix[1][1] << ' ' << boneMatrix[1][2] << ' ' << boneMatrix[1][3] << std::endl;
	std::cout << boneMatrix[2][0] << ' ' << boneMatrix[2][1] << ' ' << boneMatrix[2][2] << ' ' << boneMatrix[2][3] << std::endl;
	std::cout << boneMatrix[3][0] << ' ' << boneMatrix[3][1] << ' ' << boneMatrix[3][2] << ' ' << boneMatrix[3][3] << std::endl;

	Bone* chestBone = new Bone();
	chestBone->size = 1.0f;
	chestBone->rotation = glm::fquat(1.0f, 0.0f, 0.0f, 0.0f);
	chestBone->name = "chest";

	std::cout << chestBone->name << std::endl;
	boneMatrix = chestBone->GetMatrix();
	std::cout << boneMatrix[0][0] << ' ' << boneMatrix[0][1] << ' ' << boneMatrix[0][2] << ' ' << boneMatrix[0][3] << std::endl;
	std::cout << boneMatrix[1][0] << ' ' << boneMatrix[1][1] << ' ' << boneMatrix[1][2] << ' ' << boneMatrix[1][3] << std::endl;
	std::cout << boneMatrix[2][0] << ' ' << boneMatrix[2][1] << ' ' << boneMatrix[2][2] << ' ' << boneMatrix[2][3] << std::endl;
	std::cout << boneMatrix[3][0] << ' ' << boneMatrix[3][1] << ' ' << boneMatrix[3][2] << ' ' << boneMatrix[3][3] << std::endl;

	Bone* headBone = new Bone();
	headBone->size = 1.0f;
	headBone->rotation = glm::fquat(1.0f, 0.0f, 0.0f, 0.0f);
	headBone->name = "head";

	std::cout << headBone->name << std::endl;
	boneMatrix = headBone->GetMatrix();
	std::cout << boneMatrix[0][0] << ' ' << boneMatrix[0][1] << ' ' << boneMatrix[0][2] << ' ' << boneMatrix[0][3] << std::endl;
	std::cout << boneMatrix[1][0] << ' ' << boneMatrix[1][1] << ' ' << boneMatrix[1][2] << ' ' << boneMatrix[1][3] << std::endl;
	std::cout << boneMatrix[2][0] << ' ' << boneMatrix[2][1] << ' ' << boneMatrix[2][2] << ' ' << boneMatrix[2][3] << std::endl;
	std::cout << boneMatrix[3][0] << ' ' << boneMatrix[3][1] << ' ' << boneMatrix[3][2] << ' ' << boneMatrix[3][3] << std::endl;

	Bone* leftBone = new Bone();
	leftBone->size = 1.0f;
	leftBone->rotation = glm::fquat(glm::vec3(0.0f, 0.0f, glm::radians(45.0f)));
	leftBone->name = "left";

	std::cout << leftBone->name << std::endl;
	boneMatrix = leftBone->GetMatrix();
	std::cout << boneMatrix[0][0] << ' ' << boneMatrix[0][1] << ' ' << boneMatrix[0][2] << ' ' << boneMatrix[0][3] << std::endl;
	std::cout << boneMatrix[1][0] << ' ' << boneMatrix[1][1] << ' ' << boneMatrix[1][2] << ' ' << boneMatrix[1][3] << std::endl;
	std::cout << boneMatrix[2][0] << ' ' << boneMatrix[2][1] << ' ' << boneMatrix[2][2] << ' ' << boneMatrix[2][3] << std::endl;
	std::cout << boneMatrix[3][0] << ' ' << boneMatrix[3][1] << ' ' << boneMatrix[3][2] << ' ' << boneMatrix[3][3] << std::endl;

	Bone* rightBone = new Bone();
	rightBone->size = 1.0f;
	rightBone->rotation = glm::fquat(glm::vec3(0.0f, 0.0f, glm::radians(-45.0f)));
	rightBone->name = "right";

	std::cout << rightBone->name << std::endl;
	boneMatrix = rightBone->GetMatrix();
	std::cout << boneMatrix[0][0] << ' ' << boneMatrix[0][1] << ' ' << boneMatrix[0][2] << ' ' << boneMatrix[0][3] << std::endl;
	std::cout << boneMatrix[1][0] << ' ' << boneMatrix[1][1] << ' ' << boneMatrix[1][2] << ' ' << boneMatrix[1][3] << std::endl;
	std::cout << boneMatrix[2][0] << ' ' << boneMatrix[2][1] << ' ' << boneMatrix[2][2] << ' ' << boneMatrix[2][3] << std::endl;
	std::cout << boneMatrix[3][0] << ' ' << boneMatrix[3][1] << ' ' << boneMatrix[3][2] << ' ' << boneMatrix[3][3] << std::endl;

	Bone* leftUpBone = new Bone();
	leftUpBone->size = 1.0f;
	leftUpBone->rotation = glm::fquat(glm::vec3(0.0f, 0.0f, glm::radians(-45.0f)));
	leftUpBone->name = "leftUp";

	std::cout << leftUpBone->name << std::endl;
	boneMatrix = leftUpBone->GetMatrix();
	std::cout << boneMatrix[0][0] << ' ' << boneMatrix[0][1] << ' ' << boneMatrix[0][2] << ' ' << boneMatrix[0][3] << std::endl;
	std::cout << boneMatrix[1][0] << ' ' << boneMatrix[1][1] << ' ' << boneMatrix[1][2] << ' ' << boneMatrix[1][3] << std::endl;
	std::cout << boneMatrix[2][0] << ' ' << boneMatrix[2][1] << ' ' << boneMatrix[2][2] << ' ' << boneMatrix[2][3] << std::endl;
	std::cout << boneMatrix[3][0] << ' ' << boneMatrix[3][1] << ' ' << boneMatrix[3][2] << ' ' << boneMatrix[3][3] << std::endl;

	Bone* rightUpBone = new Bone();
	rightUpBone->size = 1.0f;
	rightUpBone->rotation = glm::fquat(glm::vec3(0.0f, 0.0f, glm::radians(45.0f)));
	rightUpBone->name = "rightUp";

	std::cout << rightUpBone->name << std::endl;
	boneMatrix = rightUpBone->GetMatrix();
	std::cout << boneMatrix[0][0] << ' ' << boneMatrix[0][1] << ' ' << boneMatrix[0][2] << ' ' << boneMatrix[0][3] << std::endl;
	std::cout << boneMatrix[1][0] << ' ' << boneMatrix[1][1] << ' ' << boneMatrix[1][2] << ' ' << boneMatrix[1][3] << std::endl;
	std::cout << boneMatrix[2][0] << ' ' << boneMatrix[2][1] << ' ' << boneMatrix[2][2] << ' ' << boneMatrix[2][3] << std::endl;
	std::cout << boneMatrix[3][0] << ' ' << boneMatrix[3][1] << ' ' << boneMatrix[3][2] << ' ' << boneMatrix[3][3] << std::endl;

	m_rootBones[0].bone->children.push_back(chestBone);

	chestBone->children.push_back(leftBone);
	chestBone->children.push_back(rightBone);
	chestBone->children.push_back(headBone);

	rightBone->children.push_back(rightUpBone);
	leftBone->children.push_back(leftUpBone);
}

Skeleton::Skeleton(const std::string& filePath)
{
	AssimpLoader::LoadSkeleton(filePath, *this);
}

void Skeleton::FreeBones(Bone* root)
{
	for (Bone* c : root->children)
		FreeBones(c);
	delete root;
}

Skeleton::~Skeleton()
{

	for (const RootBone& rb : m_rootBones)
		FreeBones(rb.bone);
}
