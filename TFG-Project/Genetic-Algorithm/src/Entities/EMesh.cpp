#include "EMesh.h"

/// <summary>
/// EMesh constructor.
/// </summary>
/// <param name="meshPath"> Mesh path.</param>
/// <param name="parentId"> Parent Id.</param>
EMesh::EMesh(const std::string meshPath, uint32_t parentId) : 
	Entity(parentId), meshPath(meshPath){
}

/// <summary>
/// EMesh destructor.
/// </summary>
EMesh::~EMesh(){
}
