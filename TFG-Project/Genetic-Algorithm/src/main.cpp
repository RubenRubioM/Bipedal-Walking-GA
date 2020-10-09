#include <iostream>

#include <Program.h>
#include <Entities/EMesh.h>
#include <Entities/ECamera.h>
#include <Utils/Shaders.h>
#include <Render/RenderEngine.h>
#include <State/State.h>
#include <DataTypes/Transformable.h>

#include <GLM/glm.hpp>

#include <memory>

int main() {
	Program* program = Program::GetInstance();

	program->SetState(State::States::EXECUTION);
	program->Start();

	return EXIT_SUCCESS;
}