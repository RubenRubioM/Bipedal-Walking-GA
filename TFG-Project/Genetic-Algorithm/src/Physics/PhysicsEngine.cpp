#include "PhysicsEngine.h"

#include <Render/RenderEngine.h>
#include <Entities/Entity.h>
#include <Entities/ECamera.h>
#include <Entities/EMesh.h>
#include <Render/ImGuiManager.h>
#include <DataTypes/Transformable.h>

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
	delete instance;
}

/// <summary>
/// Updates entity physics values.
/// </summary>
/// <param name="entity"> Entity. </param>
void PhysicsEngine::UpdateEntity(Entity* entity) {
	if (imGuiManager->Header(std::string(std::to_string(entity->GetId()) + ". " + entity->GetName()))) {
		imGuiManager->EntityTransformable(entity, std::string(entity->GetName()));
	}

	const auto& node = device->GetNodeByID(entity->GetId());
	node->SetTranslation(entity->GetPosition());
	node->SetRotation(entity->GetRotation());
	node->SetScalation(entity->GetScalation());
}

/// <summary>
/// Updates entities physics values.
/// </summary>
/// <param name="entities"> Entities. </param>
void PhysicsEngine::UpdateEntities(std::vector<std::unique_ptr<EMesh>> entities){
	
}

/// <summary>
/// Updates entity camera.
/// </summary>
/// <param name="camera"> Camera entity. </param>
/// <param name="target"> Target position. </param>
void PhysicsEngine::UpdateCamera(Entity* camera, glm::vec3 target) {
	if (imGuiManager->Header(std::string(std::to_string(camera->GetId()) + ". " + camera->GetName()))) {
		imGuiManager->Vec3Slider(camera->GetPositionPtr()
			, std::string(camera->GetName() + " position")
			, camera->GetPositionBoundaries().first
			, camera->GetPositionBoundaries().second);
	}

	const auto& cam = device->GetActiveCamera();
	const auto& camNode = device->GetActiveCameraNode();

	camNode->SetTranslation(camera->GetPosition());
	camNode->SetRotation(camera->GetRotation());
	camNode->SetScalation(camera->GetScalation());

	cam->SetCameraTarget(target);
}