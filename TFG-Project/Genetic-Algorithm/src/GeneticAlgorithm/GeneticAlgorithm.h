#pragma once

#include <memory>
#include <vector>

class ESkeleton;
class ImGuiManager;

/// <summary>
/// Class for the genetic algorithm
/// </summary>
class GeneticAlgorithm {
	public:
		/// <summary>
		/// GeneticAlgorithm constructor.
		/// </summary>
		GeneticAlgorithm();

		/// <summary>
		/// GeneticAlgorithm destructor.
		/// </summary>
		~GeneticAlgorithm();

		/// <summary>
		/// Updates all related to the genetic algorithm
		/// </summary>
		/// <param time="time"> Life of the generation. </param>
		void Update(long long time);

		/// <summary>
		/// Creates a new generation.
		/// </summary>
		void NewGeneration();

		/// <summary>
		/// Returns the population.
		/// </summary>
		/// <returns> Population. </returns>
		std::vector<std::shared_ptr<ESkeleton>> GetPopulation();

	private:

		/// <summary>
		/// Creates imgui stats for this gene.
		/// </summary>
		/// <param name="gene"> Gene. </param>
		void GeneStats(ESkeleton* gene);

		/// <summary>
		/// Population.
		/// </summary>
		std::vector<std::shared_ptr<ESkeleton>> population;

		/// <summary>
		/// ImGuiManager pointer.
		/// </summary>
		ImGuiManager* imGuiManager{ nullptr };

		/// <summary>
		/// Actual generation.
		/// </summary>
		int actualGeneration = 1;

		/// <summary>
		/// Death percentage of the population.
		/// </summary>
		float deathPercentage = 0.0;

		/// <summary>
		/// Fitness percentage of the population.
		/// </summary>
		float fitnessPercentage = 0.0f;

		/// <summary>
		/// Gene with more fitness.
		/// </summary>
		float topFitness = 0.0f;
};

