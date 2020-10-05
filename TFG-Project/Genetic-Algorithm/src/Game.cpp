#include "Game.h"

#include <iostream>

/// <summary>
/// Creates or returns a Game instance.
/// </summary>
/// <returns> Static Game instance pointer. </returns>
Game* Game::GetInstance() {
	if (!instance) {
		instance = new Game();
	}
	return instance;
}

/// <summary>
/// Private constructor.
/// </summary>
Game::Game(){
	
}
