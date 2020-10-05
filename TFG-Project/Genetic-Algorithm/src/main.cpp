#include <iostream>

#include <Program.h>
#include <Entities/EMesh.h>
#include <Utils/Shaders.h>
#include <GLM/glm.hpp>

#include <memory>

int main() {
	Program* program = Program::GetInstance();
	std::unique_ptr<Entity> mesh = std::make_unique<EMesh>("path1.jpg");
	std::unique_ptr<Entity> mesh1 = std::make_unique<EMesh>("path2.jpg",5);
	std::unique_ptr<Entity> mesh2 = std::make_unique<EMesh>("path3.jpg");
	//std::unique_ptr<Entity> e = std::make_unique<Entity>();
	mesh1->SetPosition(glm::vec3(10, 20, 30));

	std::cout << "ID: " << mesh->GetId() << std::endl;
	std::cout << "ID: " << mesh1->GetId() << std::endl;
	std::cout << "ID: " << mesh2->GetId() << std::endl;
	std::cout << "X: " << mesh1->GetPosition().z << std::endl;
	//std::cout << "ID: " << e->GetId() << std::endl;
	return 0;
}