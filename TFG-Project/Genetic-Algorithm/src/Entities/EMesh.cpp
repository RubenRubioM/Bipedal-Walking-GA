#include "EMesh.h"

#include <DataTypes/Transformable.h>

/// <summary>
/// EMesh constructor.
/// </summary>
/// <param name="meshPath"> Mesh path.</param>
/// <param name="parentId"> Parent Id.</param>
EMesh::EMesh(const std::string meshPath, const uint32_t parentId) : 
	Entity(parentId), 
	meshPath(meshPath){
}

/// <summary>
/// EMesh constructor
/// </summary>
/// <param name="transform"> Transformable. </param>
/// <param name="meshPath"> Mesh path. </param>
/// <param name="parentId"> Parent Id. </param>
EMesh::EMesh(const Transformable transform, const std::string meshPath, const uint32_t parentId) :
	Entity(transform, parentId),
	meshPath(meshPath)
{
}

/// <summary>
/// EMesh destructor.
/// </summary>
EMesh::~EMesh(){
}
