#include "GeneticAlgorithm.h"

#include <Utils/Config.h>
#include <Utils/Utils.h>
#include <Entities/Compositions/ESkeleton.h>
#include <Entities/EMesh.h>
#include <Render/ImGuiManager.h>

#include <GLM/glm.hpp>
#include <RANDOM/random.hpp>
#include <IMGUI/implot.h>
#include <NUMCPP/NumCpp.hpp>

#include <iostream>
#include <algorithm>

using Random = effolkronium::random_static;

/// <summary>
/// GeneticAlgorithm constructor.
/// </summary>
GeneticAlgorithm::GeneticAlgorithm() {
	imGuiManager = ImGuiManager::GetInstance();

	glm::vec3 offset = Utils::defaultPosition;
	glm::vec3 offsetIncrese = Utils::positionOffset;
	auto setLegBoundaries = [](std::vector<EMesh*> leg) {
		auto hip = leg[0];
		auto knee = leg[1];

		// Knee boundaries
		std::pair<float, float> boundaries = { Random::get<float>(knee->GetRotationBoundaries().first, knee->GetRotationBoundaries().second) , Random::get<float>(knee->GetRotationBoundaries().first, knee->GetRotationBoundaries().second) };
		knee->SetRotationBoundaries(std::pair<float, float>(std::min(boundaries.first, boundaries.second), std::max(boundaries.first, boundaries.second)));
		
		glm::vec3 kneeVelocity = glm::vec3(Random::get<float>(Config::rotationVelocityBoundaries.first.x, Config::rotationVelocityBoundaries.second.x), 0.0f, 0.0f);
		knee->SetRotationVelocity(kneeVelocity);

		// Hip boundaries
		boundaries = { -1, -1 };
		while (boundaries.first < 0 && boundaries.second < 0) {
			boundaries = { Random::get<float>(hip->GetRotationBoundaries().first, hip->GetRotationBoundaries().second) , Random::get<float>(hip->GetRotationBoundaries().first, hip->GetRotationBoundaries().second) };
		}
		hip->SetRotationBoundaries(std::pair<float, float>(std::min(boundaries.first, boundaries.second), std::max(boundaries.first, boundaries.second)));
		
		glm::vec3 hipVelocity = glm::vec3(Random::get<float>(Config::rotationVelocityBoundaries.first.x, Config::rotationVelocityBoundaries.second.x), 0.0f, 0.0f);
		hip->SetRotationVelocity(hipVelocity);
	};

	// Create population
	for (uint16_t i = 0; i < Config::populationSize; ++i) {

		// Entities creation.
		auto core = new EMesh(Transformable(offset, glm::vec3(0.0f, 0.0f, 0), glm::vec3(0.1f)), "media/torso.obj");
		core->SetName("Core");
		auto hip1 = new EMesh(Transformable(glm::vec3(-28.0f, 0.0f, 0.0f), ESkeleton::hipDefaultRotation, glm::vec3(0.75f)), "media/cinturaD.obj", core);
		hip1->SetName("Hip1");
		auto knee1 = new EMesh(Transformable(glm::vec3(23.0f, 200.0f, 0.0f), ESkeleton::kneeDefaultRotation, glm::vec3(1.0f)), "media/rodilla.obj", hip1);
		knee1->SetName("Knee1");
		auto hip2 = new EMesh(Transformable(glm::vec3(24.0f, 0.0f, 0.0f), ESkeleton::hipDefaultRotation, glm::vec3(0.75f)), "media/cinturaI.obj", core);
		hip2->SetName("Hip2");
		auto knee2 = new EMesh(Transformable(glm::vec3(-18.0f, 200.0f, 0.0f), ESkeleton::kneeDefaultRotation, glm::vec3(1.0f)), "media/rodilla.obj", hip2);
		knee2->SetName("Knee2");
		auto shoulder1 = new EMesh(Transformable(glm::vec3(-50.0f, 150.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f)), "media/brazo arriba.obj", core);
		shoulder1->SetName("Shoulder1");
		auto elbow1 = new EMesh(Transformable(glm::vec3(-80.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f)), "media/brazo abajo.obj", shoulder1);
		elbow1->SetName("Elbow1");
		auto shoulder2 = new EMesh(Transformable(glm::vec3(40.0f, 150.0f, 0.0f), glm::vec3(0.0f, 180.0f, 0.0f), glm::vec3(1.0f)), "media/brazo arriba.obj", core);
		shoulder2->SetName("Shoulder2");
		auto elbow2 = new EMesh(Transformable(glm::vec3(-80.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f)), "media/brazo abajo.obj", shoulder2);
		elbow2->SetName("Elbow2");

		population.push_back(std::make_unique<ESkeleton>(core, hip1, knee1, hip2, knee2, shoulder1, elbow1, shoulder2, elbow2));
		
		// Set skeleton flexibility
		float flexibility = Random::get<float>(0, 1);
		if (flexibility >= 0 && flexibility <= Config::flexibilityProbability.x) {
			population[i]->SetFlexibility(ESkeleton::Flexibility::LOW);
		}else if (flexibility > Config::flexibilityProbability.x && flexibility <= Config::flexibilityProbability.x + Config::flexibilityProbability.y) {
			population[i]->SetFlexibility(ESkeleton::Flexibility::MEDIUM);
		}else if (flexibility > Config::flexibilityProbability.x + Config::flexibilityProbability.y) {
			population[i]->SetFlexibility(ESkeleton::Flexibility::HIGH);
		}else {
			std::cout << "No flexibility probability found, automatic set to Flexibility::HIGH" << std::endl;
			population[i]->SetFlexibility(ESkeleton::Flexibility::HIGH);
		}


		// Set legs boundaries.
		setLegBoundaries(population[i]->GetLeg1());
		setLegBoundaries(population[i]->GetLeg2());

		offset.x += offsetIncrese.x;
	}
}

/// <summary>
/// GeneticAlgorithm destructor.
/// </summary>
GeneticAlgorithm::~GeneticAlgorithm() {

}

/// <summary>
/// Updates all related to the genetic algorithm
/// </summary>
/// <param time="time"> Life of the generation. </param>
void GeneticAlgorithm::Update(long long time) {
	int deads = 0;
	int totalFitness = 0;
	float minGeneFitness = std::numeric_limits<float>::max();
	float topGeneFitness = 0.0f;

	// Sort the population based on the gene fitness.
	std::sort(population.begin(), population.end(), [](std::shared_ptr<ESkeleton> skeleton1, std::shared_ptr<ESkeleton> skeleton2) {
		return skeleton1->GetFitness() > skeleton2->GetFitness();
	});

	imGuiManager->Begin("Genetic algorithm debug");
	imGuiManager->BeginTabBar("Genetic algorithm");

	if (imGuiManager->AddTab("Resume")) {
		imGuiManager->BulletText(std::string("Generation " + std::to_string(actualGeneration)));
		imGuiManager->BulletText(std::string("Life time: " + std::to_string(time/1000.0) + "s"));
		imGuiManager->BulletText(std::string("Deaths percentage: " + std::to_string(deathPercentage) + "%%"));
		imGuiManager->BulletText(std::string("Average fitness: " + std::to_string(averageFitness)));
		imGuiManager->BulletText(std::string("Top fitness: " + std::to_string(topFitness)));
		imGuiManager->BulletText(std::string("Min fitness: " + std::to_string(	(minFitness != std::numeric_limits<float>::max()) ? minFitness : 0.0  )));

		imGuiManager->EndTab();
	}

	// Important genetic algorithm flow
	if (time > 0) {
		for (auto gene : population) {
			totalFitness += gene->UpdateFitness();

			if (gene->IsDead()) {
				deads++;
			}
		
			topGeneFitness = (gene->GetFitness() >= topGeneFitness) ? gene->GetFitness() : topGeneFitness;
			minGeneFitness = (gene->GetFitness() < minGeneFitness) ? gene->GetFitness() : minGeneFitness;
		}
	}

	deathPercentage = (deads / (float)Config::populationSize) * 100.0;
	averageFitness = (totalFitness / Config::populationSize);
	minFitness = minGeneFitness;
	topFitness = topGeneFitness;

	// ImGui debug
	if (imGuiManager->AddTab("Population")) {
		for (auto gene : population) {
			auto skeleton = gene->GetSkeleton();
			auto hip1 = gene->GetLeg1()[0];
			auto knee1 = gene->GetLeg1()[1];
			auto hip2 = gene->GetLeg2()[0];
			auto knee2 = gene->GetLeg2()[1];

			std::string dead = "";
			if (gene->IsDead()) {
				dead = "(dead)";
			}

			if (imGuiManager->Header(std::string("Skeleton " + std::to_string(gene->GetSkeletonId()) + dead))) {
				imGuiManager->BulletText(std::string("Fitness: " + std::to_string(gene->GetFitness())));
				imGuiManager->BulletText(std::string("Hip1 boundaries:  (" + std::to_string(hip1->GetRotationBoundaries().first) + ", " + std::to_string(hip1->GetRotationBoundaries().second) + ")"));
				imGuiManager->BulletText(std::string("Knee1 boundaries: (" + std::to_string(knee1->GetRotationBoundaries().first) + ", " + std::to_string(knee1->GetRotationBoundaries().second) + ")"));
				imGuiManager->BulletText(std::string("Hip2 boundaries:  (" + std::to_string(hip2->GetRotationBoundaries().first) + ", " + std::to_string(hip2->GetRotationBoundaries().second) + ")"));
				imGuiManager->BulletText(std::string("Knee2 boundaries: (" + std::to_string(knee2->GetRotationBoundaries().first) + ", " + std::to_string(knee2->GetRotationBoundaries().second) + ")"));

				imGuiManager->BulletText(std::string("Hip1 speed:  " + std::to_string(std::abs(hip1->GetRotationVelocity().x)) + " deg/frame"));
				imGuiManager->BulletText(std::string("Knee1 speed: " + std::to_string(std::abs(knee1->GetRotationVelocity().x)) + " deg/frame"));
				imGuiManager->BulletText(std::string("Hip2 speed:  " + std::to_string(std::abs(hip2->GetRotationVelocity().x)) + " deg/frame"));
				imGuiManager->BulletText(std::string("Knee2 speed: " + std::to_string(std::abs(knee2->GetRotationVelocity().x)) + " deg/frame"));
			}
		}
		imGuiManager->EndTab();
	}

	imGuiManager->EndTabBar();
	imGuiManager->End();
}

/// <summary>
/// Creates a new generation.
/// </summary>
void GeneticAlgorithm::NewGeneration() {
	SaveGenerationStats();

	// Genetic algorithm flow: Selection -> Crossover -> Mutation
	auto pairPopulation = Selection();
	Crossover(pairPopulation);

	SetDefaultPopulationValues();
	ResetStats();
	actualGeneration++;
	std::cout << "---- Generation " << actualGeneration << " begins ----\n";
}

/// <summary>
/// Selection function.
/// pair.first  = vector with genes to be crossovered.
/// pair.second = vector with genes that passed to the next generation.
/// </summary>
/// <returns> Pair with genes to change and genes that passed to the next generation. </returns>
std::pair<std::vector<ESkeleton*>, std::vector<ESkeleton*>> GeneticAlgorithm::Selection() {
	int newGenes = std::ceil(Config::populationSize * Config::newGenProbability); // Number of genes who will be new (from crossover)
	int genesToNewGeneration = 0;
	auto populationAux = population;
	std::vector<ESkeleton*> newPopulation;

	switch (Config::selectionFunction){
		case Config::SelectionFunction::ROULETTE: {
			int rouletteSections = Config::rouletteSections;
			nc::NdArray<float> fitnessSections = nc::linspace<float>(minFitness, topFitness+0.001, rouletteSections);
			nc::NdArray<float> rouletteProbabilities = Utils::RouletteProbabilities(rouletteSections); // Generate the roulette probabilities array.

			// We have newGenes which are the number of genes that have to be crossovered
			// So, Config::populationSize - newGenes is the number of genes that survived throw the next generation.
			while (genesToNewGeneration != Config::populationSize - newGenes) {
				float probabilityValue = Random::get<float>(0, 1);
				int sectionIndx = rouletteProbabilities.shape().cols - 2;

				for (int i = 0; i < rouletteProbabilities.shape().cols - 1; i++) {
					if (rouletteProbabilities[0, i] <= probabilityValue && probabilityValue <= rouletteProbabilities[0, i + 1]) {
						sectionIndx = i;
						break;
					}
				}

				for (int j = 0; j < populationAux.size(); j++) {
					if (fitnessSections[0, sectionIndx] <= populationAux[j]->GetFitness() && populationAux[j]->GetFitness() <= fitnessSections[0, sectionIndx + 1]) {
						newPopulation.push_back(populationAux[j].get());
						populationAux.erase(populationAux.begin() + j);
						genesToNewGeneration++;
						break;
					}
				}

			}
			break;
		}

		case Config::SelectionFunction::TOURNAMENT: {
			break;
		}

		case Config::SelectionFunction::LINEAR: {
			break;
		}
	}

	std::vector<ESkeleton*> populationToChange;
	for (auto gene : populationAux) {
		populationToChange.push_back(gene.get());
	}
	return std::pair(populationToChange,newPopulation);
}

/// <summary>
/// Crossover function.
/// </summary>
/// <param name="pairPopulation"> pair.first = genes to be crossovered | pair.second = genes to crossover</param>
void GeneticAlgorithm::Crossover(std::pair<std::vector<ESkeleton*>, std::vector<ESkeleton*>> pairPopulation) {
	auto genesToUpdate = pairPopulation.first;
	auto newPopulation = pairPopulation.second;

	for (auto gene : genesToUpdate) {
		switch (Config::crossoverType) {
			case Config::CrossoverType::AVERAGE: {

				break;
			}
			case Config::CrossoverType::ARITHMETIC: {

				break;
			}
			case Config::CrossoverType::HEURISTIC: {
				break;
			}
		}
	}
}

/// <summary>
/// Saves the generation stats.
/// </summary>
void GeneticAlgorithm::SaveGenerationStats() {
	GenerationStats generationStats;
	generationStats.generation = actualGeneration;
	generationStats.deathPercentage = deathPercentage;
	generationStats.averageFitness = averageFitness;
	generationStats.topFitness = topFitness;
	generationStats.minFitness = minFitness;

	generationsStats.push_back(generationStats);
}

/// <summary>
/// Resets all the generation values.
/// </summary>
void GeneticAlgorithm::ResetStats() {
	deathPercentage = 0.0;
	averageFitness = 0.0;
	topFitness = 0.0;
	minFitness = std::numeric_limits<float>::max();
}

/// <summary>
/// Sets default values for position and live.
/// </summary>
void GeneticAlgorithm::SetDefaultPopulationValues() {
	glm::vec3 offset = Utils::defaultPosition;
	glm::vec3 offsetIncrese = Utils::positionOffset;

	for (auto gene : population) {
		gene->SetIsDead(false);
		gene->GetCore()->SetPosition(offset);
		gene->GetLeg1()[0]->SetRotation(ESkeleton::hipDefaultRotation);
		gene->GetLeg2()[0]->SetRotation(ESkeleton::hipDefaultRotation);
		gene->GetLeg1()[1]->SetRotation(ESkeleton::kneeDefaultRotation);
		gene->GetLeg2()[1]->SetRotation(ESkeleton::kneeDefaultRotation);

		gene->SetStartingPoint(gene->GetCore()->GetPosition());
		gene->UpdateFitness();
		offset.x += offsetIncrese.x;
	}
}

/// <summary>
/// Returns the population.
/// </summary>
/// <returns> Population. </returns>
std::vector<std::shared_ptr<ESkeleton>> GeneticAlgorithm::GetPopulation() {
	return population;
}
