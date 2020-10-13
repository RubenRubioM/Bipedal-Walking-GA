#include "Entity.h"

#include <iostream>

/// <summary>
/// Entity constructor. Increments the static ID field everytime an entity is created.
/// </summary>
Entity::Entity() {
	id = nextId;
	nextId++;
}

/// <summary>
/// Entity constructor with parent.
/// </summary>
/// <param name="parentId"> Parent Id. </param>
Entity::Entity(Entity* parent) : Entity() {
	this->parent = parent;
}

/// <summary>
/// Entity constructor with parent.
/// </summary>
/// <param name="parentId"> Parent Id. </param>
/// <param name="transform"> Transformable </param>
Entity::Entity(const Transformable transform, Entity* parent) : Entity(parent) {
	transformable = transform;
	position = transform.position;
	rotation = transform.rotation;
	scalation = transform.scalation;
}

/// <summary>
/// Entity destructor.
/// </summary>
Entity::~Entity() {
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