#include <iostream>

#include <Program.h>
#include <Entities/EMesh.h>
#include <Entities/ECamera.h>
#include <Utils/Shaders.h>
#include <GLM/glm.hpp>

#include <memory>

int main() {
	Program* program = Program::GetInstance();
	std::unique_ptr<Entity> mesh = std::make_unique<EMesh>("path1.jpg");
	std::unique_ptr<Entity> mesh1 = std::make_unique<EMesh>("path2.jpg",5);
	std::unique_ptr<Entity> mesh2 = std::make_unique<EMesh>("path3.jpg");
	std::unique_ptr<Entity> camera = std::make_unique<ECamera>(glm::vec3(10,10,10));
	mesh1->SetPosition(glm::vec3(10, 20, 30));

	std::cout << "Pos: " << static_cast<ECamera*>(camera.get())->GetTarget().x << std::endl;

	return EXIT_SUCCESS;
}