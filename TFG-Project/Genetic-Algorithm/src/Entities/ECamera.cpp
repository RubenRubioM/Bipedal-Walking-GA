#include "ECamera.h"

/// <summary>
/// ECamera constructor.
/// </summary>
/// <param name="target"> Camera target. </param>
/// <param name="parentId"> Parent Id.</param>
ECamera::ECamera(glm::vec3 target, uint32_t parentId) :
	Entity(parentId), target(target) {
}

/// <summary>
/// ECamera destructor.
/// </summary>
ECamera::~ECamera(){
}
