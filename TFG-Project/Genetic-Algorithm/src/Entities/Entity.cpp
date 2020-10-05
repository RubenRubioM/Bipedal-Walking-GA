#include "Entity.h"

#include <iostream>

/// <summary>
/// Entity constructor. Increments the static ID field everytime an entity is created.
/// </summary>
Entity::Entity(){
	id++;
}

/// <summary>
/// Entity constructor with parent.
/// </summary>
/// <param name="parentId"> Parent Id. </param>
Entity::Entity(uint32_t parentId) : parentId(parentId){
}

/// <summary>
/// Entity destructor.
/// </summary>
Entity::~Entity(){
}
