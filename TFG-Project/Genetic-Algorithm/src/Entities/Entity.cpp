#include "Entity.h"

#include <iostream>

/// <summary>
/// Entity constructor. Increments the static ID field everytime an entity is created.
/// </summary>
Entity::Entity(){
	id = nextId;
	nextId++;
}

/// <summary>
/// Entity constructor with parent.
/// </summary>
/// <param name="parentId"> Parent Id. </param>
Entity::Entity(const uint32_t parentId) : Entity(){
	this->parentId = parentId;
}

/// <summary>
/// Entity constructor with parent.
/// </summary>
/// <param name="parentId"> Parent Id. </param>
/// <param name="transform"> Transformable </param>
Entity::Entity(const Transformable transform, const uint32_t parentId) : Entity(parentId) {
	transformable = transform;
	position = transform.position;
	rotation = transform.rotation;
	scalation = transform.scalation;
}

/// <summary>
/// Entity destructor.
/// </summary>
Entity::~Entity(){
}

/// <summary>
/// Sets the entity transformable.
/// </summary>
/// <param name="transformable"> Entity transformable. </param>
void Entity::SetTransformable(const Transformable transformable) {
	this->transformable = transformable;
	this->position = this->transformable.position;
	this->rotation = this->transformable.rotation;
	this->scalation = this->transformable.scalation;
}