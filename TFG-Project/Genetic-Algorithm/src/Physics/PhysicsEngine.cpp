#include "PhysicsEngine.h"

#include <Render/RenderEngine.h>
#include <Entities/Entity.h>
#include <Render/ImGuiManager.h>

/// <summary>
/// Creates or returns a PhysicsEngine instance.
/// </summary>
/// <returns> Static PhysicsEngine instance pointer. </returns>
PhysicsEngine* PhysicsEngine::GetInstance() {
	if (!instance) {
		instance = new PhysicsEngine();
	}
	return instance;
}

/// <summary>
/// Private constructor.
/// </summary>
PhysicsEngine::PhysicsEngine() {
	device = RenderEngine::GetInstance()->GetDevice();
	smgr = device->GetSceneManager();
}

/// <summary>
/// PhysicsEngine destructor.
/// </summary>
PhysicsEngine::~PhysicsEngine() {
}

/// <summary>
/// Updates entity physics values.
/// </summary>
/// <param name="entity"> Entity. </param>
void PhysicsEngine::UpdateEntity(Entity* entity){
	imGuiManager->EntityTransformable(entity, std::string("Entity " + to_string(entity->GetId())).c_str(), -200, 200);

	const auto& node = device->GetNodeByID(entity->GetId());
	node->SetTranslation(entity->GetPosition());
	node->SetRotation(entity->GetRotation());
	node->SetScalation(entity->GetScalation());
}
