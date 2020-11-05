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
		/// Returns the actual generation.
		/// </summary>
		/// <returns> Actual generation. </returns>
		int GetGeneration() { return actualGeneration; }

		/// <summary>
		/// Returns the population.
		/// </summary>
		/// <returns> Population. </returns>
		std::vector<std::shared_ptr<ESkeleton>> GetPopulation();

		/// <summary>
		/// Returns the best gene of the last generation.
		/// </summary>
		/// <returns> Best gene of the last generation. </returns>
		ESkeleton* GetBestGeneLastGeneration();

		/// <summary>
		/// Returns the best gene.
		/// </summary>
		/// <returns> Best gene. </returns>
		ESkeleton* GetBestGene();

	private:
		/// <summary>
		/// Auxiliar struct to storage multiple data about the generation.
		/// </summary>
		struct GenerationStats {
			public:
				GenerationStats() = default;
				~GenerationStats() = default;

				int generation = 0;
				int bestGeneId = 0;
				float deathPercentage = 0.0;
				float averageFitness = 0.0;
				float topFitness = 0.0;
				float minFitness = std::numeric_limits<float>::max();
				float averageHip1Velocity = 0.0;
				float averageKnee1Velocity = 0.0;
				float averageHip2Velocity = 0.0;
				float averageKnee2Velocity = 0.0;
				float topHip1Velocity = std::numeric_limits<float>::lowest();
				float topKnee1Velocity = std::numeric_limits<float>::lowest();
				float topHip2Velocity = std::numeric_limits<float>::lowest();
				float topKnee2Velocity = std::numeric_limits<float>::lowest();
				float minHip1Velocity = std::numeric_limits<float>::max();
				float minKnee1Velocity = std::numeric_limits<float>::max();
				float minHip2Velocity = std::numeric_limits<float>::max();
				float minKnee2Velocity = std::numeric_limits<float>::max();
				std::pair<float, float> averageHip1RotationBoundaries;
				std::pair<float, float> averageKnee1RotationBoundaries;
				std::pair<float, float> averageHip2RotationBoundaries;
				std::pair<float, float> averageKnee2RotationBoundaries;
				float topHip1Rotation = std::numeric_limits<float>::lowest();
				float topKnee1Rotation = std::numeric_limits<float>::lowest();
				float topHip2Rotation = std::numeric_limits<float>::lowest();
				float topKnee2Rotation = std::numeric_limits<float>::lowest();
				float minHip1Rotation = std::numeric_limits<float>::max();
				float minKnee1Rotation = std::numeric_limits<float>::max();
				float minHip2Rotation = std::numeric_limits<float>::max();
				float minKnee2Rotation = std::numeric_limits<float>::max();
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
		/// Mutate the population.
		/// </summary>
		void Mutation();

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
		/// Generates random skeleton values.
		/// </summary>
		/// <param name="skeleton"> Skeleton. </param>
		void GenerateRandomSkeletonValues(ESkeleton* skeleton);

		/// <summary>
		/// Population.
		/// </summary>
		std::vector<std::shared_ptr<ESkeleton>> population;

		/// <summary>
		/// ImGuiManager pointer.
		/// </summary>
		ImGuiManager* imGuiManager{ nullptr };

		/// <summary>
		/// Best gene id.
		/// </summary>
		int bestGeneId = 0;

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
