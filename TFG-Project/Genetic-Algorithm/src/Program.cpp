#include "Program.h"

#include <Render/RenderEngine.h>
#include <State/StateExecution.h>

#include <iostream>

/// <summary>
/// Creates or returns a Program instance.
/// </summary>
/// <returns> Static Program instance pointer. </returns>
Program* Program::GetInstance() {
	if (!instance) {
		instance = new Program();
	}
	return instance;
}

/// <summary>
/// Private constructor.
/// </summary>
Program::Program(){
	renderEngine = RenderEngine::GetInstance();
}

/// <summary>
/// Set the actual state.
/// </summary>
/// <param name="newState"> State to set. </param>
void Program::SetState(State::States newState) {
	switch (newState) {
		case State::States::EXECUTION:
			this->state = std::make_unique<StateExecution>();
			this->state->InitState();
			break;
		case State::States::MENU:
			break;
	}
}

/// <summary>
/// Start the program.
/// </summary>
void Program::Start(){
	while (renderEngine->IsOpen()) {
		state->Update();
		state->Render();
	}
}