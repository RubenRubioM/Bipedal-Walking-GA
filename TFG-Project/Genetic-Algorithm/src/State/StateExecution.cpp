#include "StateExecution.h"

#include <Render/RenderEngine.h>
#include <Physics/PhysicsEngine.h>
#include <Render/ImGuiManager.h>
#include <Entities/ECamera.h>
#include <Entities/EMesh.h>
#include <Entities/Compositions/ESkeleton.h>
#include <Entities/Entity.h>
#include <DataTypes/Transformable.h>

#include <IMGUI/imgui.h>
#include <GLM/gtc/type_ptr.hpp>

/// <summary>
/// StateExecution constructor.
/// </summary>
StateExecution::StateExecution() {
	renderEngine = RenderEngine::GetInstance();
	physicsEngine = PhysicsEngine::GetInstance();
	imGuiManager = ImGuiManager::GetInstance();
	camera = make_unique<ECamera>(Transformable(glm::vec3(-50.0f, 50.0f, 10.0f), glm::vec3(0.0f), glm::vec3(1.0f)), glm::vec3(0, 30, 0));
	camera->SetName("Camera");
	/*renderEngine->AddSkybox("media/skybox/right.jpg"
		, "media/skybox/left.jpg"
		, "media/skybox/top.jpg"
		, "media/skybox/bottom.jpg"
		, "media/skybox/front.jpg"
		, "media/skybox/back.jpg");*/

	// Field
	terrain.push_back(make_unique<EMesh>(Transformable(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(200.0f, 10.0f, 200.0f)), "media/Grass_Block.obj"));
	terrain[0]->SetName("Field");

	// Entities creation.
	skeletonsMeshes.push_back(make_unique<EMesh>(Transformable(glm::vec3(0.0f, 80.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0), glm::vec3(7.0f)), "media/Body.obj"));
	skeletonsMeshes[0]->SetName("Core");
	skeletonsMeshes.push_back(make_unique<EMesh>(Transformable(glm::vec3(-0.5f,0.0f,0.0f), glm::vec3(0.0f,0.0f,-180.0f), glm::vec3(0.25f)), "media/lathi.obj", skeletonsMeshes[0].get()));
	skeletonsMeshes[1]->SetName("Hip1");
	skeletonsMeshes.push_back(make_unique<EMesh>(Transformable(glm::vec3(0.0f,9.0f,0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f)), "media/lathi.obj", skeletonsMeshes[1].get()));
	skeletonsMeshes[2]->SetName("Knee1");
	skeletonsMeshes.push_back(make_unique<EMesh>(Transformable(glm::vec3(0.5f,0.0f,0.0f), glm::vec3(0.0f, 0.0f, -180.0f), glm::vec3(0.25f)), "media/lathi.obj", skeletonsMeshes[0].get()));
	skeletonsMeshes[3]->SetName("Hip2");
	skeletonsMeshes.push_back(make_unique<EMesh>(Transformable(glm::vec3(0.0f, 9.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f)), "media/lathi.obj", skeletonsMeshes[3].get()));
	skeletonsMeshes[4]->SetName("Knee2");
	skeletonsMeshes.push_back(make_unique<EMesh>(Transformable(glm::vec3(-0.5f, 1.5f, 0.0f), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(0.25f)), "media/lathi.obj", skeletonsMeshes[0].get()));
	skeletonsMeshes[5]->SetName("Shoulder1");
	skeletonsMeshes.push_back(make_unique<EMesh>(Transformable(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -180.0f), glm::vec3(0.25f)), "media/lathi.obj", skeletonsMeshes[5].get()));
	skeletonsMeshes[6]->SetName("Elbow1");
	skeletonsMeshes.push_back(make_unique<EMesh>(Transformable(glm::vec3(0.5f, 1.5f, 0.0f), glm::vec3(0.0f, 0.0f, -90.0f), glm::vec3(0.25f)), "media/lathi.obj", skeletonsMeshes[0].get()));
	skeletonsMeshes[7]->SetName("Shoulder2");
	skeletonsMeshes.push_back(make_unique<EMesh>(Transformable(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -180.0f), glm::vec3(0.25f)), "media/lathi.obj", skeletonsMeshes[7].get()));
	skeletonsMeshes[8]->SetName("Elbow2");

	// Skeleton 1.
	skeletons.push_back(make_unique<ESkeleton>(skeletonsMeshes[0].get(), skeletonsMeshes[1].get(), skeletonsMeshes[2].get(), skeletonsMeshes[3].get(), skeletonsMeshes[4].get(), skeletonsMeshes[5].get(), skeletonsMeshes[6].get(), skeletonsMeshes[7].get(), skeletonsMeshes[8].get()));
	skeletons[0]->SetFlexibility(ESkeleton::Flexibility::HIGH);
	AddEntities();
}

/// <summary>
/// StateExecution destructor.
/// </summary>
StateExecution::~StateExecution() {
}

/// <summary>
/// StateExecution initialization.
/// </summary>
void StateExecution::InitState() {
}

/// <summary>
/// Called at the start of every frame.
/// </summary>
void StateExecution::InitFrame() {
	imGuiManager->FrameInit();
}

/// <summary>
/// StateExecution update.
/// </summary>
void StateExecution::Update() {
	imGuiManager->Begin("Entities transformables");

	for (const auto& skeleton : skeletons) {
		physicsEngine->UpdateSkeleton(skeleton.get());
	}

	for (const auto& mesh : terrain) {
		physicsEngine->UpdateEntity(mesh.get());
	}


	physicsEngine->UpdateCamera(camera.get());
	imGuiManager->End();
}

/// <summary>
/// StateExecution render.
/// </summary>
void StateExecution::Render() {
	renderEngine->BeginScene();

	imGuiManager->Begin("Debug");
	imGuiManager->Checkbox("Show bounding boxes", &showBoundingBoxes);
	if (showBoundingBoxes) {
		for (const auto& mesh : skeletonsMeshes) {
			renderEngine->DrawBoundingBox(mesh.get());
		}

		for (const auto& mesh : terrain) {
			renderEngine->DrawBoundingBox(mesh.get());
		}
	}
	imGuiManager->Checkbox("Gravity", &physicsEngine->GetGravityActivated());
	imGuiManager->End();

	renderEngine->DrawAll();
	imGuiManager->Render();
	renderEngine->EndScene();
}

/// <summary>
/// Add all the entities to the render engine.
/// </summary>
void StateExecution::AddEntities() {
	renderEngine->AddCamera(camera.get());
	for (const auto& mesh : skeletonsMeshes) {
		renderEngine->AddMesh(mesh.get());
		physicsEngine->SetEntityValues(mesh.get());
	}

	for (const auto& mesh : terrain) {
		renderEngine->AddMesh(mesh.get());
		physicsEngine->AddCollidingMesh(mesh.get());
		physicsEngine->SetEntityValues(mesh.get());
	}

}
