#pragma once
#include <string>
#include "Skeleton.h"

namespace AssimpLoader
{
	void LoadSkeleton(const std::string& filePath, Skeleton& skeleton);
}