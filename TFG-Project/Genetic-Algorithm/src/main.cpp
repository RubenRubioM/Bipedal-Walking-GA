#include <iostream>

#include <Program.h>
#include <Entities/EMesh.h>
#include <Utils/Shaders.h>

#include <memory>

int main() {
	Program* program = Program::GetInstance();
	std::unique_ptr<Entity> mesh = std::make_unique<EMesh>("path1.jpg");
	std::unique_ptr<Entity> mesh1 = std::make_unique<EMesh>("path2.jpg",5);
	std::unique_ptr<Entity> mesh2 = std::make_unique<EMesh>("path3.jpg");
	std::unique_ptr<Entity> mesh3 = std::make_unique<EMesh>("path4.jpg");

	std::cout << mesh->GetShader() << std::endl;
	mesh->SetShader(Shaders::CartoonShader);
	std::cout << mesh->GetShader() << std::endl;
	return 0;
}