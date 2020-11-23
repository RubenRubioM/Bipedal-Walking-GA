#pragma once

#include <NUMCPP/NumCpp.hpp>
#include <GLM/vec3.hpp>

#include <iostream>

/// <summary>
/// Class for utilities.
/// </summary>
class Utils {
	public:
		/// <summary>
		/// FPS of the game.
		/// </summary>
		inline static int FPS = 60;

		/// <summary>
		/// Time between frames.
		/// </summary>
		inline static float deltaTime = 1.0/FPS;

		/// <summary>
		/// Default start position for the first skeleton.
		/// </summary>
		inline static glm::vec3 defaultPosition{ -30.0f, 50.0f, 0.0f };

		/// <summary>
		/// Position increase for every skeleton.
		/// </summary>
		inline static glm::vec3 positionOffset{ 60.0f,0.0f,0.0f };

	private:
};