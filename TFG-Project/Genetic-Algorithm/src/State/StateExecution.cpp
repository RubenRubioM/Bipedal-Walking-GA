#include "StateExecution.h"

#include <Render/RenderEngine.h>
#include <Entities/ECamera.h>
#include <Entities/EMesh.h>
#include <Entities/Entity.h>
#include <DataTypes/Transformable.h>

/// <summary>
/// StateExecution constructor.
/// </summary>
StateExecution::StateExecution(){
	renderEngine = RenderEngine::GetInstance();

	camera = make_unique<ECamera>(Transformable(glm::vec3(50.0f),glm::vec3(0.0f),glm::vec3(1.0f)),glm::vec3(0, 0, 0));

	// Entities creation.
	entities.push_back(make_unique<EMesh>(Transformable(glm::vec3(0), glm::vec3(0.0f), glm::vec3(5.0f)), "media/kart_physics.obj"));
	entities.push_back(make_unique<EMesh>(Transformable(glm::vec3(30.0f,10.0f,0.0f), glm::vec3(0.0f), glm::vec3(5.0f)), "media/telebanana.obj"));
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
/// StateExecution update.
/// </summary>
void StateExecution::Update(){
	std::cout << "Updating..." << std::endl;
}

/// <summary>
/// StateExecution render.
/// </summary>
void StateExecution::Render(){
	renderEngine->BeginScene();
	renderEngine->DrawAll();
	renderEngine->EndScene();
}

/// <summary>
/// Add all the entities to the render engine.
/// </summary>
void StateExecution::AddEntities() {
	renderEngine->AddCamera(camera.get());
	for (const auto& entity : entities) {
		renderEngine->AddMesh(entity.get());
	}
}
