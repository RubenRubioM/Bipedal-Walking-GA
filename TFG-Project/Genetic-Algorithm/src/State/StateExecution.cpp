#include "StateExecution.h"

#include <Render/RenderEngine.h>

/// <summary>
/// StateExecution constructor.
/// </summary>
StateExecution::StateExecution(){
	renderEngine = RenderEngine::GetInstance();
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
