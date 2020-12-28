#include "EMesh.h"

#include <DataTypes/Transformable.h>
#include <DataTypes/OBBCollider.h>

#include <iostream>

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

/// <summary>
/// Sets OBBCollider.
/// </summary>
/// <param name="collider"> Entity collider. </param>
void EMesh::SetCollider(OBBCollider collider){
	if (!this->collider) {
		this->collider = std::make_unique<OBBCollider>(collider.GetCenter(), collider.GetVertexs(), collider.GetSize(), collider.GetAxes());
	}else{
		this->collider->SetCenter(collider.GetCenter());
		this->collider->SetVertexs(collider.GetVertexs());
		this->collider->SetSize(collider.GetSize());
		this->collider->SetAxes(collider.GetAxes());
	}
}
