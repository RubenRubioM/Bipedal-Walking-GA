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
		void Update();

		/// <summary>
		/// Returns the population.
		/// </summary>
		/// <returns> Population. </returns>
		std::vector<std::shared_ptr<ESkeleton>> GetPopulation();

	private:
		/// <summary>
		/// Population.
		/// </summary>
		std::vector<std::shared_ptr<ESkeleton>> population;

		/// <summary>
		/// Actual generation.
		/// </summary>
		int actualGeneration = 1;

		/// <summary>
		/// ImGuiManager pointer.
		/// </summary>
		ImGuiManager* imGuiManager{ nullptr };
};

