#pragma once

#include <memory>
#include <vector>
#include <utility>
#include <limits>

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
		/// Auxiliar struct to storage multiple data about the generation.
		/// </summary>
		struct GenerationStats {
			int generation = 0;
			float deathPercentage = 0.0;
			float averageFitness = 0.0;
			float topFitness = 0.0;
			float minFitness = std::numeric_limits<float>::max();
		};

		/// <summary>
		/// Selection function.
		/// pair.first  = vector with genes to be crossovered.
		/// pair.second = vector with genes that passed to the next generation.
		/// </summary>
		/// <returns> Pair with genes to change and genes that passed to the next generation. </returns>
		std::pair<std::vector<ESkeleton*>, std::vector<ESkeleton*>> Selection();

		/// <summary>
		/// Crossover function.
		/// </summary>
		/// <param name="pairPopulation"> pair.first = genes to be crossovered | pair.second = genes to crossover</param>
		void Crossover(std::pair<std::vector<ESkeleton*>, std::vector<ESkeleton*>> pairPopulation);

		/// <summary>
		/// Saves the generation stats.
		/// </summary>
		void SaveGenerationStats();

		/// <summary>
		/// Resets all the generation values.
		/// </summary>
		void ResetStats();

		/// <summary>
		/// Sets default values for position and live.
		/// </summary>
		void SetDefaultPopulationValues();

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
		float averageFitness = 0.0f;

		/// <summary>
		/// Gene with more fitness.
		/// </summary>
		float topFitness = 0.0f;

		/// <summary>
		/// Gene with less fitness.
		/// </summary>
		float minFitness = std::numeric_limits<float>::max();

		/// <summary>
		/// Generations stats.
		/// </summary>
		std::vector<GenerationStats> generationsStats;

};
