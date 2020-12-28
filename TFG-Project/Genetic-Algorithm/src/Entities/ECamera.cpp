#include "ECamera.h"

#include <DataTypes/Transformable.h>

/// <summary>
/// ECamera constructor.
/// </summary>
/// <param name="target"> Camera target. </param>
/// <param name="parent"> Parent. </param>
ECamera::ECamera(const glm::vec3 target, Entity* parent) :
	Entity(parent),
	target(target)
{
}

/// <summary>
/// ECamera constructor
/// </summary>
/// <param name="transform"> Transformable. </param>
/// <param name="target"> Camera target. </param>
/// <param name="parent"> Parent. </param>
ECamera::ECamera(const Transformable transform, const glm::vec3 target, Entity* parent) :
	Entity(transform, parent),
	target(target)
{
}

/// <summary>
/// ECamera destructor.
/// </summary>
ECamera::~ECamera() {
}
