#include <iostream>

#include <Program.h>

int main() {
	Program* program = Program::GetInstance();
	program->SetState(State::States::EXECUTION);
	program->Start();

	return EXIT_SUCCESS;
}