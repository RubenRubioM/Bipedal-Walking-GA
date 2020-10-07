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
	std::unique_ptr<Entity> mesh = std::make_unique<EMesh>(Transformable(glm::vec3(10.0f), glm::vec3(0.0f), glm::vec3(20.0f)),"path1.jpg");
	std::unique_ptr<Entity> mesh1 = std::make_unique<EMesh>(Transformable(glm::vec3(10.0f), glm::vec3(0.0f), glm::vec3(20.0f)),"path2.jpg",5);
	std::unique_ptr<Entity> mesh2 = std::make_unique<EMesh>(Transformable(glm::vec3(10.0f), glm::vec3(0.0f), glm::vec3(20.0f)),"path3.jpg");
	std::unique_ptr<Entity> camera = std::make_unique<ECamera>(Transformable(glm::vec3(10.0f),glm::vec3(0.0f),glm::vec3(20.0f)), glm::vec3(10.0, 10.0, 10.0));
	
	program->SetState(State::States::EXECUTION);
	program->Start();

	return EXIT_SUCCESS;
}