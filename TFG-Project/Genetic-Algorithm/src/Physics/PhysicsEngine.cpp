#include "PhysicsEngine.h"

#include <Render/RenderEngine.h>
#include <Entities/Entity.h>
#include <Entities/ECamera.h>
#include <Entities/Compositions/ESkeleton.h>
#include <Entities/EMesh.h>
#include <Render/ImGuiManager.h>
#include <DataTypes/Transformable.h>
#include <DataTypes/OBBCollider.h>
#include <Utils/Utils.h>

#include <limits>       // std::numeric_limits

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
/// Init entity physics values.
/// </summary>
/// <param name="entity"> Entity. </param>
void PhysicsEngine::SetEntityValues(Entity* entity) {
	const auto& node = device->GetNodeByID(entity->GetId());
	node->SetTranslation(entity->GetPosition());
	node->SetRotation(entity->GetRotation());
	node->SetScalation(entity->GetScalation());

	// Update boundingBox in case the scalation has changed.
	if (auto mesh = dynamic_cast<EMesh*>(entity)) {
		mesh->SetCollider(CalculateOBB(mesh, node));
	}
}

/// <summary>
/// Updates entity physics values.
/// </summary>
/// <param name="entity"> Entity. </param>
void PhysicsEngine::UpdateEntity(Entity* entity) {
	SetEntityValues(entity);
}

/// <summary>
/// Updates skeleton physics values.
/// </summary>
/// <param name="skeleton"> skeleton. </param>
void PhysicsEngine::UpdateSkeleton(ESkeleton* skeleton) {

	// If the skeleton is dead we do nothing.
	if (skeleton->IsDead()) return;

	auto eSkeleton = skeleton->GetSkeleton();

	if(gravityActivated)
		ApplyGravity(skeleton);

	ApplySkeletonMovement(skeleton);

	/* --------------------------------------- IMPORTANT ----------------------------------------------
	* It's possible that the mecanism to check if the leg is on air or not based on the obb field collider
	* will have to be done more times. For example, if I apply gravity and then rotate and move the skeleton
	* I wont' be able due to the legsOnAir are updated at the end (FixPosition) and I can't move if both of my legs are
	* in the field.
	* -------------------------------------------------------------------------------------------------
	*/

	// There is a possible that when all is finished I can FixPosition before SetEntityValues.
	for (auto joint : eSkeleton)
		SetEntityValues(joint);

	// Now after all the changes have been done we fix the posible positions errors.
	FixPosition(skeleton);

	// At the end we check if the skeleton has die or not.
	skeleton->SetIsDead(SkeletonDead(skeleton));
}

/// <summary>
/// Updates entity camera.
/// </summary>
/// <param name="camera"> Camera entity. </param>
void PhysicsEngine::UpdateCamera(Entity* camera) {
	if (imGuiManager->Header(std::string(std::to_string(camera->GetId()) + ". " + camera->GetName()))) {
		imGuiManager->Vec3Slider(camera->GetPositionPtr()
			, std::string(camera->GetName() + " position")
			, camera->GetPositionBoundaries().first
			, camera->GetPositionBoundaries().second);
		imGuiManager->Vec3Slider(static_cast<ECamera*>(camera)->GetTargetPtr()
			, std::string(camera->GetName() + " target")
			, camera->GetPositionBoundaries().first
			, camera->GetPositionBoundaries().second);
	}

	const auto& cam = device->GetActiveCamera();
	const auto& camNode = device->GetActiveCameraNode();

	camNode->SetTranslation(camera->GetPosition());
	camNode->SetRotation(camera->GetRotation());
	camNode->SetScalation(camera->GetScalation());

	cam->SetCameraTarget(static_cast<ECamera*>(camera)->GetTarget());
}

/// <summary>
/// Updates camera with target.
/// </summary>
/// <param name="camera"> Camera entity. </param>
/// <param name="skeleton"> Skeleton target. </param>
void PhysicsEngine::UpdateCamera(Entity* camera, ESkeleton* skeleton) {
	auto targetPosition = skeleton->GetCore()->GetPosition();
	camera->SetPosition(glm::vec3(targetPosition.x - 100, targetPosition.y + 40,targetPosition.z + 10));
	static_cast<ECamera*>(camera)->SetTarget(targetPosition);
	const auto& cam = device->GetActiveCamera();
	const auto& camNode = device->GetActiveCameraNode();

	camNode->SetTranslation(camera->GetPosition());
	camNode->SetRotation(camera->GetRotation());
	camNode->SetScalation(camera->GetScalation());

	cam->SetCameraTarget(static_cast<ECamera*>(camera)->GetTarget());
}


/// <summary>
/// Adds a colliding object to the physics engine
/// </summary>
/// <param name="entity"> Colliding object. </param>
void PhysicsEngine::AddCollidingMesh(EMesh* entity) {
	for (auto mesh : collidingMeshes) {
		if (mesh->GetId() == entity->GetId()) {
			return;
		}
	}
	collidingMeshes.push_back(entity);

}

/// <summary>
/// Returns whether to apply gravity or not
/// </summary>
/// <param name="skeleton"> Skeleton to apply gravity. </param>
/// <returns> True if gravity applied, false if not. </returns>
void PhysicsEngine::ApplyGravity(ESkeleton* skeleton) const {
	if (skeleton->GetOnAir()) {
		auto core = skeleton->GetCore();
		glm::vec3 movement = gravity * Utils::deltaTime;
		core->SetPosition(core->GetPosition() + movement);

		// TODO: Maybe it's not worth it update de obb just for 1 frame
		for (auto joint : skeleton->GetSkeleton()) {
			joint->GetCollider()->TranslateOBB(movement);
		}
	}

}

/// <summary>
/// Apply the movement to the skeleton
/// </summary>
/// <param name="skeleton"> Skeleton to move. </param>
void PhysicsEngine::ApplySkeletonMovement(ESkeleton* skeleton) const {
	auto eSkeleton = skeleton->GetSkeleton();
	auto core = eSkeleton[0];
	auto hip1 = eSkeleton[1];
	auto knee1 = eSkeleton[2];
	auto hip2 = eSkeleton[3];
	auto knee2 = eSkeleton[4];
	auto applyJointRotation = [](EMesh* joint) {
		joint->SetRotation(joint->GetRotation() + (joint->GetRotationVelocity() * Utils::deltaTime));

		auto rotationBoundaries = joint->GetRotationBoundaries();
		auto lowerRotation = (rotationBoundaries.first < rotationBoundaries.second) ? rotationBoundaries.first : rotationBoundaries.second;
		auto greaterRotation = (rotationBoundaries.first >= rotationBoundaries.second) ? rotationBoundaries.first : rotationBoundaries.second;
		joint->SetRotationBoundaries(std::pair<float, float>(lowerRotation, greaterRotation));

		if (joint->GetRotation().x >= joint->GetRotationBoundaries().second) {
			joint->SetRotation(glm::vec3(joint->GetRotationBoundaries().second, joint->GetRotation().y, joint->GetRotation().z));
			joint->SetRotationVelocity(-joint->GetRotationVelocity());
		}
		if (joint->GetRotation().x <= joint->GetRotationBoundaries().first) {
			joint->SetRotation(glm::vec3(joint->GetRotationBoundaries().first, joint->GetRotation().y, joint->GetRotation().z));
			joint->SetRotationVelocity(-joint->GetRotationVelocity());
		}

	};

	// TODO: Check the speed of the core.
	auto checkCoreMovement = [&core](EMesh* hip, EMesh* knee, bool onAir) {
		// If (hip going backwards and still above 0 rotation || knee going backwards and hip above 0 rotation) && touching the floor
		if (((hip->GetRotationVelocity().x < 0 && hip->GetRotation().x > 0) || (knee->GetRotationVelocity().x < 0 && hip->GetRotation().x > 0)) && !onAir) {
			auto position = core->GetPosition();
			float newZ = position.z + ((std::abs(hip->GetRotationVelocity().x / 10) + std::abs(knee->GetRotationVelocity().x / 10)) * Utils::deltaTime);
			core->SetPosition(glm::vec3(position.x, position.y, newZ));

			return true;
		}
		return false;
	};

	applyJointRotation(hip1);
	applyJointRotation(knee1);
	applyJointRotation(hip2);
	applyJointRotation(knee2);

	// To just apply one leg at a time
	checkCoreMovement(hip1, knee1, skeleton->GetLeg1OnAir());
	checkCoreMovement(hip2, knee2, skeleton->GetLeg2OnAir());
}

/// <summary>
/// Fix position.
/// </summary>
/// <param name="skeleton"> Skeleton entity to fix. </param>
/// <returns> If the position has been fixed. </return>
bool PhysicsEngine::FixPosition(ESkeleton* skeleton) const {
	auto core = skeleton->GetCore();
	auto leg1 = skeleton->GetLeg1()[1];
	auto leg2 = skeleton->GetLeg2()[1];

	// We look for the lowest leg OBB "y" coordinate in order to know the lowest point in the legs.
	float skeletonMinYLeg1 = std::numeric_limits<float>::max();
	float skeletonMinYLeg2 = std::numeric_limits<float>::max();
	auto leg1Vertexs = leg1->GetCollider()->GetVertexs();
	auto leg2Vertexs = leg2->GetCollider()->GetVertexs();
	for (uint16_t i = 0; i < 8; i++) {
		skeletonMinYLeg1 = (leg1Vertexs[i].y < skeletonMinYLeg1) ? leg1Vertexs[i].y : skeletonMinYLeg1;
		skeletonMinYLeg2 = (leg2Vertexs[i].y < skeletonMinYLeg2) ? leg2Vertexs[i].y : skeletonMinYLeg2;
	}

	// Now we look for the highest terrain collider OBB "y" coordinate.
	float terrainMaxY = std::numeric_limits<float>::lowest();

	// For each loop but we only have one and it's plane
	for (auto collider : collidingMeshes) {
		for (auto vertex : collider->GetCollider()->GetVertexs()) {
			terrainMaxY = (vertex.y > terrainMaxY) ? vertex.y : terrainMaxY;
		}
	}

	// Update if any leg is touching the floor or not
	skeleton->SetLeg1OnAir((skeletonMinYLeg1 <= terrainMaxY) ? false : true);
	skeleton->SetLeg2OnAir((skeletonMinYLeg2 <= terrainMaxY) ? false : true);

	float skeletonMinY = (skeletonMinYLeg1 <= skeletonMinYLeg2) ? skeletonMinYLeg1 : skeletonMinYLeg2;
	if (skeletonMinY <= terrainMaxY) {
		// Fix position to set the skeleton above the terrain
		//TODO: Be carefull with this "17". It is because I have to place the model below the terrain but not to much. 
		float positionToPlace = ((core->GetCollider()->GetCenter().y - skeletonMinY) + terrainMaxY) - 17;
		core->SetPosition(glm::vec3(core->GetPosition().x, positionToPlace, core->GetPosition().z));
		skeleton->SetOnAir(false);

		return true;
	}else {
		skeleton->SetOnAir(true);

		return false;
	}
}


/// <summary>
/// Check if the skeleton has die.
/// </summary>
/// <param name="skeleton"> Skeleton to check. </param>
/// <returns> If is dead or not. </returns>
bool PhysicsEngine::SkeletonDead(ESkeleton* skeleton) const {
	auto core = skeleton->GetCore();
	auto leg1 = skeleton->GetLeg1();
	auto leg2 = skeleton->GetLeg2();
	/* Cases where the skeleton is should die.
		- Case 1: Both hips rotations are less -30º.
		- Case 2: Both hip rotations minus knee rotation are greater 30º.
		- Case 3: The hip in front minus knee rotation is greater than 90º and the hip behind is lower than -90º.
			- Case 3.1: Same but with the other leg.
		- Case 4: The hip in front rotation is greater than 120º and the hip behind is lower than -90º.
			- Case 4.1: Same but with the other leg.
		- Case 5: The hip behind rotation lower than -30º and hip in front on air.
			- Case 5.1: Same but with the other leg.
	*/

	// Case 1
	if (leg1[0]->GetRotation().x < -30 && leg2[0]->GetRotation().x < -30)
		return true;

	// Case 2
	if (leg1[0]->GetRotation().x - std::abs(leg1[1]->GetRotation().x) > 30 && leg2[0]->GetRotation().x - std::abs(leg2[1]->GetRotation().x) > 30)
		return true;

	// Case 3
	if (leg1[0]->GetRotation().x - std::abs(leg1[1]->GetRotation().x) > 90 && leg2[0]->GetRotation().x < -90)
		return true;

		// Case 3.1
		if (leg2[0]->GetRotation().x - std::abs(leg2[1]->GetRotation().x) > 90 && leg1[0]->GetRotation().x < -90)
			return true;

	// Case 4
	if (leg1[0]->GetRotation().x > 120 && leg2[0]->GetRotation().x < -90)
		return true;
		
		// Case 4.1
		if (leg2[0]->GetRotation().x > 120 && leg1[0]->GetRotation().x < -90)
			return true;

	// Case 5
	if (leg1[0]->GetRotation().x < -30 && skeleton->GetLeg2OnAir())
		return true;
		
		// Case 5.1
		if (leg2[0]->GetRotation().x < -30 && skeleton->GetLeg1OnAir())
			return true;

	return false;
}

/// <summary>
/// Calculate and assign OBB collider to the entity
/// </summary>
/// <param name="mesh"> Mesh. </param>
/// <param name="node"> Node. </param>
OBBCollider PhysicsEngine::CalculateOBB(EMesh* mesh, CLE::CLNode* node) {
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

	/*glm::vec3 p0 = glm::vec3(0);
	glm::vec3 p1 = glm::vec3(0);
	glm::vec3 p2 = glm::vec3(0);
	glm::vec3 p3 = glm::vec3(0);
	glm::vec3 p4 = glm::vec3(0);
	glm::vec3 p5 = glm::vec3(0);
	glm::vec3 p6 = glm::vec3(0);
	glm::vec3 p7 = glm::vec3(0);*/

	std::vector<glm::vec3> vertexs = { p0,p1,p2,p3,p4,p5,p6,p7 };

	glm::mat3 axes;
	axes[0] = glm::vec3(glm::distance(p0.x, p4.x) > 0 ? 1 : -1, 0, 0);
	axes[1] = glm::vec3(0, glm::distance(p0.y, p1.y) > 0 ? 1 : -1, 0);
	axes[2] = glm::vec3(0, 0, glm::distance(p0.z, p2.z) > 0 ? 1 : -1);

	return OBBCollider(center, vertexs, mesh->GetDimensions(), axes);

}

