#include "ECamera.h"

#include <DataTypes/Transformable.h>

/// <summary>
/// ECamera constructor.
/// </summary>
/// <param name="target"> Camera target. </param>
/// <param name="parentId"> Parent Id.</param>
ECamera::ECamera(const glm::vec3 target, const uint32_t parentId) :
	Entity(parentId), 
	target(target) 
{
}

/// <summary>
/// ECamera constructor
/// </summary>
/// <param name="transform"> Transformable. </param>
/// <param name="target"> Camera target. </param>
/// <param name="parentId"> Parent Id. </param>
ECamera::ECamera(const Transformable transform, const glm::vec3 target, const uint32_t parentId) :
	Entity(transform,parentId),
	target(target)
{
}

/// <summary>
/// ECamera destructor.
/// </summary>
ECamera::~ECamera(){
}
