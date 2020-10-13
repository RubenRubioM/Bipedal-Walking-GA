#include "EMesh.h"

#include <DataTypes/Transformable.h>

/// <summary>
/// EMesh constructor.
/// </summary>
/// <param name="meshPath"> Mesh path.</param>
/// <param name="parent"> Parent.</param>
EMesh::EMesh(const std::string meshPath, Entity* parent) :
	Entity(parent),
	meshPath(meshPath) {
}

/// <summary>
/// EMesh constructor
/// </summary>
/// <param name="transform"> Transformable. </param>
/// <param name="meshPath"> Mesh path. </param>
/// <param name="parent"> Parent. </param>
EMesh::EMesh(const Transformable transform, const std::string meshPath, Entity* parent) :
	Entity(transform, parent),
	meshPath(meshPath)
{
}

/// <summary>
/// EMesh destructor.
/// </summary>
EMesh::~EMesh() {
}
