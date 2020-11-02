#pragma once

#include <GLM/vec3.hpp>

#include <chrono>

using namespace std::chrono_literals;

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
		inline static int populationSize{ 10 };
		
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
		inline static double newGenProbability{ 0.2 };

		/// <summary>
		/// Probability to get a mutation.
		/// </summary>
		inline static double mutationProbability{ 0.01 };

		/// <summary>
		/// Selection function.
		/// </summary>
		inline static SelectionFunction selectionFunction{ SelectionFunction::ROULETTE };

		/// <summary>
		/// Roulette sections for probability.
		/// </summary>
		inline static int rouletteSections{ 5 };

		/// <summary>
		/// RotationVelocity for joints boundaries.
		/// </summary>
		inline static std::pair<glm::vec3, glm::vec3> rotationVelocityBoundaries{ glm::vec3(30.0f,0.0f,0.0f),glm::vec3(80.0f,0.0f,0.0f) };

		/// <summary>
		/// Generation life span.
		/// </summary>
		inline static std::chrono::milliseconds generationLifeSpan = 10 * 1000ms;
};