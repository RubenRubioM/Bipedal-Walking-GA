#include "GeneticAlgorithm.h"

#include <Utils/Config.h>
#include <Entities/Compositions/ESkeleton.h>
#include <Entities/EMesh.h>

#include <GLM/glm.hpp>
#include <RANDOM/random.hpp>

#include <iostream>

using Random = effolkronium::random_static;

/// <summary>
/// GeneticAlgorithm constructor.
/// </summary>
GeneticAlgorithm::GeneticAlgorithm() {
	float xOffset = 0;
	float xOffsetIncrease = 10;
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
		auto core = new EMesh(Transformable(glm::vec3(xOffset, 80.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0), glm::vec3(7.0f)), "media/Body.obj");
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
/// Returns the population.
/// </summary>
/// <returns> Population. </returns>
std::vector<std::shared_ptr<ESkeleton>> GeneticAlgorithm::GetPopulation() {
	return population;
}