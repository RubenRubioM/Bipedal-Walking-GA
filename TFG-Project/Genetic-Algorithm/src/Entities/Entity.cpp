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
Entity::Entity(uint32_t parentId) : parentId(parentId){
	id = nextId;
	nextId++;
}

/// <summary>
/// Entity destructor.
/// </summary>
Entity::~Entity(){
}
