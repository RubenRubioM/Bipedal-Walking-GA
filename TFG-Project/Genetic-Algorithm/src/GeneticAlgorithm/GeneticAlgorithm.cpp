#include "GeneticAlgorithm.h"

#include <Utils/Config.h>
#include <Entities/Compositions/ESkeleton.h>
#include <Entities/EMesh.h>
#include <Render/ImGuiManager.h>

#include <GLM/glm.hpp>
#include <RANDOM/random.hpp>

#include <iostream>
#include <algorithm>

using Random = effolkronium::random_static;

/// <summary>
/// GeneticAlgorithm constructor.
/// </summary>
GeneticAlgorithm::GeneticAlgorithm() {
	imGuiManager = ImGuiManager::GetInstance();

	float xOffset = -30;
	float xOffsetIncrease = 20;
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
		auto core = new EMesh(Transformable(glm::vec3(xOffset, 50.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0), glm::vec3(7.0f)), "media/Body.obj");
		core->SetName("Core");
		auto hip1 = new EMesh(Transformable(glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -180.0f), glm::vec3(0.25f)), "media/lathi.obj", core);
		hip1->SetName("Hip1");
		auto knee1 = new EMesh(Transformable(glm::vec3(0.0f, 9.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f)), "media/lathi.obj", hip1);
		knee1->SetName("Knee1");
		auto hip2 = new EMesh(Transformable(glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -180.0f), glm::vec3(0.25f)), "media/lathi.obj", core);
		hip2->SetName("Hip2");
		auto knee2 = new EMesh(Transformable(glm::vec3(0.0f, 9.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f)), "media/lathi.obj", hip2);
		knee2->SetName("Knee2");
		auto shoulder1 = new EMesh(Transformable(glm::vec3(-0.5f, 1.5f, 0.0f), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(0.25f)), "media/lathi.obj", core);
		shoulder1->SetName("Shoulder1");
		auto elbow1 = new EMesh(Transformable(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -180.0f), glm::vec3(0.25f)), "media/lathi.obj", shoulder1);
		elbow1->SetName("Elbow1");
		auto shoulder2 = new EMesh(Transformable(glm::vec3(0.5f, 1.5f, 0.0f), glm::vec3(0.0f, 0.0f, -90.0f), glm::vec3(0.25f)), "media/lathi.obj", core);
		shoulder2->SetName("Shoulder2");
		auto elbow2 = new EMesh(Transformable(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -180.0f), glm::vec3(0.25f)), "media/lathi.obj", shoulder2);
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

		xOffset += xOffsetIncrease;
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
	imGuiManager->Begin("Genetic algorithm debug");
	imGuiManager->BeginTabBar("Genetic algorithm");

	if (imGuiManager->AddTab("Resume")) {
		imGuiManager->BulletText(std::string("Generation " + std::to_string(actualGeneration)));
		imGuiManager->BulletText(std::string("Life time: " + std::to_string(time/1000.0) + "s"));
		imGuiManager->BulletText(std::string("Deaths percentage: " + std::to_string(deathPercentage) + "%%"));
		imGuiManager->BulletText(std::string("Average fitness: " + std::to_string(fitnessPercentage)));
		imGuiManager->BulletText(std::string("Top fitness: " + std::to_string(topFitness)));

		imGuiManager->EndTab();
	}
	int deads = 0;
	int totalFitness = 0;

	// Sort the population based on the gene fitness.
	std::sort(population.begin(), population.end(), [](std::shared_ptr<ESkeleton> skeleton1, std::shared_ptr<ESkeleton> skeleton2) {
		return skeleton1->GetFitness() > skeleton2->GetFitness();
	});

	// Important genetic algorithm flow
	for (auto gene : population) {
		gene->UpdateFitness();

		totalFitness += gene->GetFitness();
		if (gene->IsDead()) {
			deads++;
		}

		topFitness = (gene->GetFitness() >= topFitness) ? gene->GetFitness() : topFitness;
	}

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
				deads++;
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

	deathPercentage = (deads / (float)Config::populationSize) * 100.0;
	fitnessPercentage = (totalFitness / Config::populationSize);

	imGuiManager->EndTabBar();
	imGuiManager->End();
}

/// <summary>
/// Creates a new generation.
/// </summary>
void GeneticAlgorithm::NewGeneration() {
	// Reset all values and save the generation data.
	actualGeneration++;
}

/// <summary>
/// Returns the population.
/// </summary>
/// <returns> Population. </returns>
std::vector<std::shared_ptr<ESkeleton>> GeneticAlgorithm::GetPopulation() {
	return population;
}

/// <summary>
/// Creates imgui stats for this gene.
/// </summary>
/// <param name="gene"> Gene. </param>
void GeneticAlgorithm::GeneStats(ESkeleton* gene) {

}