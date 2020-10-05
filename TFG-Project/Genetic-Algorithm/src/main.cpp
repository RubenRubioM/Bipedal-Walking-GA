#include <iostream>

#include <Game.h>
#include <NUMCPP/NumCpp.hpp>
int main() {
	Game* game = Game::GetInstance();
	nc::NdArray<int> a = { {1, 2}, {3, 4}, {5, 6} };
	std::cout << a << std::endl;
	return 0;
}