#pragma once

#include <GLM/vec3.hpp>

#include <chrono>

using namespace std::chrono_literals;

#define MSTOSECONDS 1000

/// <summary>
/// Program configuration.
/// </summary>
class Config {
	public:
		/// <summary>
		/// Enumeration of selections functions avaliables.
		/// </summary>
		enum SelectionFunction {ROULETTE, TOURNAMENT};

		/// <summary>
		/// Enumeration of the crossover types.
		/// </summary>
		enum CrossoverType {HEURISTIC, ARITHMETIC, AVERAGE, ONEPOINT};

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
		inline static float newGenProbability{ 0.2f };

		/// <summary>
		/// Probability to get a mutation.
		/// </summary>
		inline static float mutationProbability{ 0.01f };

		/// <summary>
		/// Selection function.
		/// </summary>
		inline static SelectionFunction selectionFunction{ SelectionFunction::ROULETTE };

		/// <summary>
		/// Crossover type.
		/// </summary>
		inline static CrossoverType crossoverType{ CrossoverType::ONEPOINT };

		/// <summary>
		/// Tournament members to fight.
		/// </summary>
		inline static int tournamentMembers{ 2 };

		/// <summary>
		/// RotationVelocity for joints boundaries.
		/// </summary>
		inline static std::pair<glm::vec3, glm::vec3> rotationVelocityBoundaries{ glm::vec3(30.0f,0.0f,0.0f),glm::vec3(80.0f,0.0f,0.0f) };
		
		/// <summary>
		/// Generation life span in seconds.
		/// </summary>
		inline static int generationLifeSpan = 10;
};