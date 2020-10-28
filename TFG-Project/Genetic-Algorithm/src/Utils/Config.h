#pragma once

#include <GLM/vec3.hpp>

/// <summary>
/// Program configuration.
/// </summary>
class Config {
	public:
		/// <summary>
		/// Enumeration of selections functions avaliables.
		/// </summary>
		enum SelectionFunction {ROULETTE, TOURNAMENT, LINEAR};

		/// <summary>
		/// Population size.
		/// </summary>
		inline static int populationSize{ 5 };
		
		/// <summary>
		/// Flexibility probability distribution.
		/// X = LOW; Y = MEDIUM; Z = HIGH
		/// X + Y + Z = 1.0
		/// </summary>
		inline static glm::vec3 flexibilityProbability{ 0.33f };

		/// <summary>
		/// Maximum number of generations.
		/// </summary>
		inline static int maxGenerations{ 50 };

		/// <summary>
		/// Percentage of new genes in the new generation.
		/// </summary>
		inline static double newGenProbability{ 0.1 };

		/// <summary>
		/// Probability to get a mutation.
		/// </summary>
		inline static double mutationProbability{ 0.01 };

		/// <summary>
		/// Selection function.
		/// </summary>
		inline static SelectionFunction selectionFunction{ SelectionFunction::ROULETTE };

		/// <summary>
		/// RotationVelocity for joints boundaries.
		/// </summary>
		inline static std::pair<glm::vec3, glm::vec3> rotationVelocityBoundaries{ glm::vec3(30.0f,0.0f,0.0f),glm::vec3(80.0f,0.0f,0.0f) };
};