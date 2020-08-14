#include "Skeleton.h"
#include <iostream>
#include "AssimpLoader.h"

Skeleton::Skeleton(const std::string& filePath)
{
	AssimpLoader::LoadSkeleton(filePath, *this);
}

void Skeleton::FreeJoints(Joint* root)
{
	for (Joint* c : root->children)
		FreeJoints(c);
	delete root;
}

Skeleton::~Skeleton()
{
	if (rootJoint)
		FreeJoints(rootJoint);
}
