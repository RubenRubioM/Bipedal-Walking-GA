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
		inline static glm::vec3 positionOffset{ 40.0f,0.0f,0.0f };

		/// <summary>
		/// Generates a roulette probabilities with a non linear function.
		/// </summary>
		/// <param name="nElements"> Number of sections. </param>
		/// <returns> Array with probabilities. </returns>
		static nc::NdArray<float> RouletteProbabilities(int nElements) {
			// href: https://en.wikipedia.org/wiki/Fitness_proportionate_selection
			int sumOfFitness = 10;
			float previousProbability = 0.0f;
			nc::NdArray<float> probabilities = nc::zeros<float>(1,nElements);

			for (int i = 0; i < nElements; i++) {
				previousProbability = previousProbability + (i+1 / sumOfFitness);
				probabilities[0,i] = previousProbability;
			}

			return nc::divide(probabilities, previousProbability);
		}

	private:
};