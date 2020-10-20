#include "PhysicsEngine.h"

#include <Render/RenderEngine.h>
#include <Entities/Entity.h>
#include <Entities/ECamera.h>
#include <Entities/EMesh.h>
#include <Render/ImGuiManager.h>
#include <DataTypes/Transformable.h>
#include <DataTypes/OBBCollider.h>

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
	
	// Update boundingBox in case the scalation has changed.
	if (auto mesh = dynamic_cast<EMesh*>(entity)) {
		CalculateOBB(mesh, node);
	}
}

/// <summary>
/// Check if a mesh is colliding with any collidingMeshes.
/// </summary>
/// <param name="Mesh"> Mesh. </param>
void PhysicsEngine::CheckCollision(EMesh* mesh) {
	// https://gamedev.stackexchange.com/questions/49041/oriented-bounding-box-how-to
	auto meshCollider = mesh->GetCollider();

	for (auto collidingMesh : collidingMeshes) {
		std::cout << "Colliding: " << meshCollider->IsColliding(collidingMesh->GetCollider()) << std::endl;
	}
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

/// <summary>
/// Calculate and assign OBB collider to the entity
/// </summary>
/// <param name="mesh"> Mesh. </param>
/// <param name="node"> Node. </param>
void PhysicsEngine::CalculateOBB(EMesh* mesh, CLE::CLNode* node) {
	mesh->SetDimensions(node->CalculateBoundingBoxOBB());

	/*
		IMPORTANT: All pivots are in the bottom center of the mesh.
		We have to change every measure to LOCAL space. (EMesh dimensions are in WORLD space)

		^  +height
		|
		|   / -depth
		|  /
		| /
		|/------> +width

		   /3-------- 7
		  / |       / |
		 /  |      /  |
		1---------5   |
		|  /2- - -|- -6
		| /       |  /
		|/        | /
		0---------4
		a1  = 0->1
		a2  = 0->2
		a3  = 0->4
		a4  = 1->3
		a5  = 1->5
		a6  = 2->3
		a7  = 2->6
		a8  = 3->7
		a9  = 4->5
		a10 = 4->6
		a11 = 5->7
		a12 = 6->7
	*/

	glm::mat4 model = node->GetTransformationMat();
	auto pivot = node->GetGlobalTranslation();
	auto localWidth = mesh->GetDimensions().x / node->GetGlobalScalation().x;
	auto localHeight = mesh->GetDimensions().y / node->GetGlobalScalation().y;
	auto localDepth = mesh->GetDimensions().z / node->GetGlobalScalation().z;

	glm::vec3 center = model * glm::vec4(pivot.x, localHeight / 2, pivot.z, 1);
	glm::vec3 p0 = model * glm::vec4(-(localWidth / 2), 0, +(localDepth / 2), 1);
	glm::vec3 p1 = model * glm::vec4(-(localWidth / 2), localHeight, +(localDepth / 2), 1);
	glm::vec3 p2 = model * glm::vec4(-(localWidth / 2), 0, -(localDepth / 2), 1);
	glm::vec3 p3 = model * glm::vec4(-(localWidth / 2), localHeight, -(localDepth / 2), 1);
	glm::vec3 p4 = model * glm::vec4(+(localWidth / 2), 0, +(localDepth / 2), 1);
	glm::vec3 p5 = model * glm::vec4(+(localWidth / 2), localHeight, +(localDepth / 2), 1);
	glm::vec3 p6 = model * glm::vec4(+(localWidth / 2), 0, -(localDepth / 2), 1);
	glm::vec3 p7 = model * glm::vec4(+(localWidth / 2), localHeight, -(localDepth / 2), 1);

	std::vector<glm::vec3> vertexs = { p0,p1,p2,p3,p4,p5,p6,p7 };

	glm::mat3 axes;
	axes[0] = glm::vec3(glm::distance(p0.x, p4.x) > 0 ? 1 : -1, 0, 0);
	axes[1] = glm::vec3(0, glm::distance(p0.y, p1.y) > 0 ? 1 : -1, 0);
	axes[2] = glm::vec3(0, 0, glm::distance(p0.z, p2.z) > 0 ? 1 : -1);

	mesh->SetCollider(OBBCollider(center, vertexs, mesh->GetDimensions(), axes));

}