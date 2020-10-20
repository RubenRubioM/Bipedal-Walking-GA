#include "Program.h"

#include <Render/RenderEngine.h>
#include <State/StateExecution.h>
#include <Utils/Utils.h>

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
Program::Program() {
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
void Program::Start() {
	int frameCount = 0;
	int lastFPS = renderEngine->GetTime();

	while (renderEngine->IsOpen()) {
		state->InitFrame();
		state->Update();
		state->Render();

		int currentTime = renderEngine->GetTime();
		frameCount++;
		if (currentTime - lastFPS >= 1.0) {
			std::string title = std::string(renderEngine->GetTitle() + " " + std::to_string(frameCount));
			renderEngine->SetTitle(title);
			Utils::FPS = frameCount;
			Utils::deltaTime = 1.0 / Utils::FPS;
			frameCount = 0;
			lastFPS = currentTime;
		}
	}

}