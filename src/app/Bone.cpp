#include "Bone.h"

glm::mat4 Bone::GetMatrix() const
{
	return ((glm::mat4)rotation) * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, size, 0.0f));
}
