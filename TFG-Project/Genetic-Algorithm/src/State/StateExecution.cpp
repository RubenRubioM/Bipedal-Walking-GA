#include "StateExecution.h"

#include <Render/RenderEngine.h>
#include <Physics/PhysicsEngine.h>
#include <Render/ImGuiManager.h>
#include <Entities/ECamera.h>
#include <Entities/EMesh.h>
#include <Entities/Entity.h>
#include <DataTypes/Transformable.h>

#include <IMGUI/imgui.h>
#include <GLM/gtc/type_ptr.hpp>

/// <summary>
/// StateExecution constructor.
/// </summary>
StateExecution::StateExecution(){
	renderEngine = RenderEngine::GetInstance();
	physicsEngine = PhysicsEngine::GetInstance();
	imGuiManager = ImGuiManager::GetInstance();
	camera = make_unique<ECamera>(Transformable(glm::vec3(50.0f),glm::vec3(0.0f),glm::vec3(1.0f)),glm::vec3(0, 0, 0));
	camera->SetName("Camera");
	renderEngine->AddSkybox("media/skybox/right.jpg"
		, "media/skybox/left.jpg"
		, "media/skybox/top.jpg"
		, "media/skybox/bottom.jpg"
		, "media/skybox/front.jpg"
		, "media/skybox/back.jpg");

	// Entities creation.
	meshes.push_back(make_unique<EMesh>(Transformable(glm::vec3(0), glm::vec3(0.0f), glm::vec3(5.0f,2.0f,5.0f)), "media/lathi.obj"));
	meshes[0]->SetName("Stick padre");
	meshes.push_back(make_unique<EMesh>(Transformable(glm::vec3(0), glm::vec3(90.0f,0.0f,0.0f), glm::vec3(1.0f)), "media/lathi.obj", meshes[0]->GetId()));
	meshes[1]->SetName("Stick hijo");
	AddEntities();
}

/// <summary>
/// StateExecution destructor.
/// </summary>
StateExecution::~StateExecution(){
}

/// <summary>
/// StateExecution initialization.
/// </summary>
void StateExecution::InitState(){
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
void StateExecution::Update(){
	imGuiManager->Begin("Entities transformables");
	for (const auto& mesh : meshes) {
		physicsEngine->UpdateEntity(mesh.get());
	}
	physicsEngine->UpdateCamera(camera.get(), glm::vec3(0));
	imGuiManager->End();
}

/// <summary>
/// StateExecution render.
/// </summary>
void StateExecution::Render(){
	renderEngine->BeginScene();
	renderEngine->DrawAll();
	imGuiManager->Render();
	renderEngine->EndScene();
}

/// <summary>
/// Add all the entities to the render engine.
/// </summary>
void StateExecution::AddEntities() {
	renderEngine->AddCamera(camera.get());
	for (const auto& mesh : meshes) {
		renderEngine->AddMesh(mesh.get());
	}
}
