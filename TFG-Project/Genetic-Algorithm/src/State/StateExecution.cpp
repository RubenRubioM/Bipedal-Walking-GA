#include "StateExecution.h"

#include <Render/RenderEngine.h>
#include <Render/ImGuiManager.h>
#include <Entities/ECamera.h>
#include <Entities/EMesh.h>
#include <Entities/Entity.h>
#include <DataTypes/Transformable.h>

#include <IMGUI/imgui.h>

/// <summary>
/// StateExecution constructor.
/// </summary>
StateExecution::StateExecution(){
	renderEngine = RenderEngine::GetInstance();

	camera = make_unique<ECamera>(Transformable(glm::vec3(50.0f),glm::vec3(0.0f),glm::vec3(1.0f)),glm::vec3(0, 0, 0));

	// Entities creation.
	meshes.push_back(make_unique<EMesh>(Transformable(glm::vec3(0), glm::vec3(0.0f), glm::vec3(5.0f)), "media/kart_physics.obj"));
	meshes.push_back(make_unique<EMesh>(Transformable(glm::vec3(30.0f,10.0f,0.0f), glm::vec3(0.0f), glm::vec3(5.0f)), "media/telebanana.obj"));
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
	ImGuiManager::GetInstance()->FrameInit();
}

/// <summary>
/// StateExecution update.
/// </summary>
void StateExecution::Update(){
	ImGui::Begin("Ventana 1");
	ImGui::Checkbox("Checkbox", &check);
	ImGui::Text("Esta es la ventana 1");
	ImGui::Button("Boton 1");
	ImGui::End();

	ImGui::Begin("Ventana 2");
	ImGui::Text("Esta es la ventana 2");
	ImGui::End();
}

/// <summary>
/// StateExecution render.
/// </summary>
void StateExecution::Render(){
	renderEngine->BeginScene();
	renderEngine->DrawAll();
	ImGuiManager::GetInstance()->Render();
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
