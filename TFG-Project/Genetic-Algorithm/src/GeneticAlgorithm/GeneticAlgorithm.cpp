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

		int size = generationsStats.size();
		float* xValues = new float[size];
		float* deathValues = new float[size];
		float* fitnessValues = new float[size];
		float* topFitness = new float[size];
		float* minFitness = new float[size];

		float* averageHip1Velocity = new float[size];
		float* minHip1Velocity = new float[size];
		float* topHip1Velocity = new float[size];

		float* averageKnee1Velocity = new float[size];
		float* minKnee1Velocity = new float[size];
		float* topKnee1Velocity = new float[size];

		float* averageHip2Velocity = new float[size];
		float* minHip2Velocity = new float[size];
		float* topHip2Velocity = new float[size];

		float* averageKnee2Velocity = new float[size];
		float* minKnee2Velocity = new float[size];
		float* topKnee2Velocity = new float[size];
		
		ImVec2* averageHip1Rotation = new ImVec2[size];
		float* minHip1Rotation = new float[size];
		float* topHip1Rotation = new float[size];

		ImVec2* averageKnee1Rotation = new ImVec2[size];
		float* minKnee1Rotation = new float[size];
		float* topKnee1Rotation = new float[size];

		ImVec2* averageHip2Rotation = new ImVec2[size];
		float* minHip2Rotation = new float[size];
		float* topHip2Rotation = new float[size];

		ImVec2* averageKnee2Rotation = new ImVec2[size];
		float* minKnee2Rotation = new float[size];
		float* topKnee2Rotation = new float[size];

		int i = 0;
		for (const auto& generation : generationsStats) {
			xValues[i] = generation.generation;
			deathValues[i] = generation.deathPercentage;
			fitnessValues[i] = generation.averageFitness;
			topFitness[i] = generation.topFitness;
			minFitness[i] = generation.minFitness;

			// Velocity
			averageHip1Velocity[i] = generation.averageHip1Velocity;
			minHip1Velocity[i] = averageHip1Velocity[i] - generation.minHip1Velocity;
			topHip1Velocity[i] = generation.topHip1Velocity - averageHip1Velocity[i];

			averageKnee1Velocity[i] = generation.averageKnee1Velocity;
			minKnee1Velocity[i] = averageKnee1Velocity[i] - generation.minKnee1Velocity;
			topKnee1Velocity[i] = generation.topKnee1Velocity - averageKnee1Velocity[i];

			averageHip2Velocity[i] = generation.averageHip2Velocity;
			minHip2Velocity[i] = averageHip2Velocity[i] - generation.minHip2Velocity;
			topHip2Velocity[i] = generation.topHip2Velocity - averageHip2Velocity[i];

			averageKnee2Velocity[i] = generation.averageKnee2Velocity;
			minKnee2Velocity[i] = averageKnee2Velocity[i] - generation.minKnee2Velocity;
			topKnee2Velocity[i] = generation.topKnee2Velocity - averageKnee2Velocity[i];

			// Rotations
			averageHip1Rotation[i].x = generation.averageHip1RotationBoundaries.first;
			averageHip1Rotation[i].y = generation.averageHip1RotationBoundaries.second;
			minHip1Rotation[i] = ((averageHip1Rotation[i].x + averageHip1Rotation[i].y) / 2) - generation.minHip1Rotation;
			topHip1Rotation[i] = generation.topHip1Rotation - ((averageHip1Rotation[i].x + averageHip1Rotation[i].y) / 2);

			averageKnee1Rotation[i].x = generation.averageKnee1RotationBoundaries.first;
			averageKnee1Rotation[i].y = generation.averageKnee1RotationBoundaries.second;
			minKnee1Rotation[i] = ((averageKnee1Rotation[i].x + averageKnee1Rotation[i].y) / 2) - generation.minKnee1Rotation;
			topKnee1Rotation[i] = generation.topKnee1Rotation - ((averageKnee1Rotation[i].x + averageKnee1Rotation[i].y) / 2);

			averageHip2Rotation[i].x = generation.averageHip2RotationBoundaries.first;
			averageHip2Rotation[i].y = generation.averageHip2RotationBoundaries.second;
			minHip2Rotation[i] = ((averageHip2Rotation[i].x + averageHip2Rotation[i].y) / 2) - generation.minHip2Rotation;
			topHip2Rotation[i] = generation.topHip2Rotation - ((averageHip2Rotation[i].x + averageHip2Rotation[i].y) / 2);

			averageKnee2Rotation[i].x = generation.averageKnee2RotationBoundaries.first;
			averageKnee2Rotation[i].y = generation.averageKnee2RotationBoundaries.second;
			minKnee2Rotation[i] = ((averageKnee2Rotation[i].x + averageKnee2Rotation[i].y) / 2) - generation.minKnee2Rotation;
			topKnee2Rotation[i] = generation.topKnee2Rotation - ((averageKnee2Rotation[i].x + averageKnee2Rotation[i].y) / 2);

			
			i++;
		}

		imGuiManager->Separator();

		if (imGuiManager->Header("Death percentage")) {
			ImPlot::SetNextPlotLimits(1, Config::maxGenerations, -10, 100);
			if (generationsStats.size() > 1 && ImPlot::BeginPlot("Death percentage", "Generations", "Death percentage")) {
				ImPlot::PlotLine("% Death percentage", xValues, deathValues, size);
				ImPlot::EndPlot();
			}
		}

		if (imGuiManager->Header("Average fitness")) {
			ImPlot::SetNextPlotLimits(1, Config::maxGenerations, 0, 200);
			if (generationsStats.size() > 1 && ImPlot::BeginPlot("Average fitness", "Generations", "Fitness")) {
				ImPlot::PlotLine("Fitness value", xValues, fitnessValues, size);
				ImPlot::EndPlot();
			}
		}

		if (imGuiManager->Header("Top fitness")) {
			ImPlot::SetNextPlotLimits(1, Config::maxGenerations, 0, 200);
			if (generationsStats.size() > 1 && ImPlot::BeginPlot("Top fitness", "Generations", "Fitness")) {
				ImPlot::PlotLine("Top fitness", xValues, topFitness, size);
				ImPlot::EndPlot();
			}
		}

		if (imGuiManager->Header("Min fitness")) {
			ImPlot::SetNextPlotLimits(1, Config::maxGenerations, 0, 200);
			if (generationsStats.size() > 1 && ImPlot::BeginPlot("Min fitness", "Generations", "Fitness")) {
				ImPlot::PlotLine("Min fitness", xValues, minFitness, size);
				ImPlot::EndPlot();
			}
		}

		if (imGuiManager->Header("Hip1 velocity")) {
			ImPlot::SetNextPlotLimits(0, Config::maxGenerations, Config::rotationVelocityBoundaries.first.x - 20, Config::rotationVelocityBoundaries.second.x + 20);
			if (generationsStats.size() >= 1 && ImPlot::BeginPlot("Hip1 velocity", "Generations", "Velocity")) {
				ImPlot::PlotErrorBars("Velocity", xValues, averageHip1Velocity, minHip1Velocity, topHip1Velocity,size);
				ImPlot::PlotScatter("Velocity", xValues, averageHip1Velocity, size);
				ImPlot::EndPlot();
			}
		}

		if (imGuiManager->Header("Knee1 velocity")) {
			ImPlot::SetNextPlotLimits(0, Config::maxGenerations, Config::rotationVelocityBoundaries.first.x - 20, Config::rotationVelocityBoundaries.second.x + 20);
			if (generationsStats.size() >= 1 && ImPlot::BeginPlot("Knee1 velocity", "Generations", "Velocity")) {
				ImPlot::PlotErrorBars("Velocity", xValues, averageKnee1Velocity, minKnee1Velocity, topKnee1Velocity, size);
				ImPlot::PlotScatter("Velocity", xValues, averageKnee1Velocity, size);
				ImPlot::EndPlot();
			}
		}

		if (imGuiManager->Header("Hip2 velocity")) {
			ImPlot::SetNextPlotLimits(0, Config::maxGenerations, Config::rotationVelocityBoundaries.first.x - 20, Config::rotationVelocityBoundaries.second.x + 20);
			if (generationsStats.size() >= 1 && ImPlot::BeginPlot("Hip2 velocity", "Generations", "Velocity")) {
				ImPlot::PlotErrorBars("Velocity", xValues, averageHip2Velocity, minHip2Velocity, topHip2Velocity, size);
				ImPlot::PlotScatter("Velocity", xValues, averageHip2Velocity, size);
				ImPlot::EndPlot();
			}
		}

		if (imGuiManager->Header("Knee2 velocity")) {
			ImPlot::SetNextPlotLimits(0, Config::maxGenerations, Config::rotationVelocityBoundaries.first.x - 20, Config::rotationVelocityBoundaries.second.x + 20);
			if (generationsStats.size() >= 1 && ImPlot::BeginPlot("Knee2 velocity", "Generations", "Velocity")) {
				ImPlot::PlotErrorBars("Velocity", xValues, averageKnee2Velocity, minKnee2Velocity, topKnee2Velocity, size);
				ImPlot::PlotScatter("Velocity", xValues, averageKnee2Velocity, size);
				ImPlot::EndPlot();
			}
		}

		auto rotationsPlots = [&](const std::string title, ImVec2* averageRotation, float* minRotation, float* topRotation) {
			if (imGuiManager->Header(title.c_str())) {
				ImPlot::SetNextPlotLimits(0, Config::maxGenerations, -100, 100);
				if (generationsStats.size() >= 1 && ImPlot::BeginPlot(std::string(title + " (average max & min)").c_str(), "Generations", "Angle(deg)")) {
					float* centers = new float[size];
					for (unsigned int j = 0; j < size; j++) {
						centers[j] = ((averageRotation[j].x + averageRotation[j].y) / 2);
						ImPlot::PushPlotClipRect();
						ImVec2 rmin = ImPlot::PlotToPixels(ImPlotPoint((j + 1) - 0.25f, averageRotation[j].x));
						ImVec2 rmax = ImPlot::PlotToPixels(ImPlotPoint((j + 1) + 0.25f, averageRotation[j].y));
						ImPlot::GetPlotDrawList()->AddRectFilled(rmin, rmax, IM_COL32(255, 0, 0, 255));

					}

					ImPlot::PopPlotClipRect();
					ImPlot::PlotErrorBars("Error", xValues, centers, minRotation, topRotation, size);
					ImPlot::PlotScatter("Error", xValues, centers, size);
					ImPlot::EndPlot();
					delete[] centers;
				}
			}
		};

		rotationsPlots("Hip1 rotations", averageHip1Rotation, minHip1Rotation, topHip1Rotation);
		rotationsPlots("Knee1 rotations", averageKnee1Rotation, minKnee1Rotation, topKnee1Rotation);
		rotationsPlots("Hip2 rotations", averageHip2Rotation, minHip2Rotation, topHip2Rotation);
		rotationsPlots("Knee2 rotations", averageKnee2Rotation, minKnee2Rotation, topKnee2Rotation);
		
		#if 1
				delete[] xValues;
				delete[] deathValues;
				delete[] fitnessValues;
				delete[] topFitness;
				delete[] minFitness;
				delete[] averageHip1Velocity;
				delete[] minHip1Velocity;
				delete[] topHip1Velocity;
				delete[] averageKnee1Velocity;
				delete[] minKnee1Velocity;
				delete[] topKnee1Velocity;
				delete[] averageHip2Velocity;
				delete[] minHip2Velocity;
				delete[] topHip2Velocity;
				delete[] averageKnee2Velocity;
				delete[] minKnee2Velocity;
				delete[] topKnee2Velocity;
				delete[] averageHip1Rotation;
				delete[] minHip1Rotation;
				delete[] topHip1Rotation;
				delete[] averageKnee1Rotation;
				delete[] minKnee1Rotation;
				delete[] topKnee1Rotation;
				delete[] averageHip2Rotation;
				delete[] minHip2Rotation;
				delete[] topHip2Rotation;
				delete[] averageKnee2Rotation;
				delete[] minKnee2Rotation;
				delete[] topKnee2Rotation;
		#endif


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

	if (imGuiManager->AddTab("Generations")) {

		for (auto generation : generationsStats) {
			if (imGuiManager->Header(std::string("Generation " + std::to_string(generation.generation)))) {
				imGuiManager->BulletText(std::string("Deaths percentage: " + std::to_string(generation.deathPercentage) + "%%"));
				imGuiManager->BulletText(std::string("Average fitness: " + std::to_string(generation.averageFitness)));
				imGuiManager->BulletText(std::string("Top fitness: " + std::to_string(generation.topFitness)));
				imGuiManager->BulletText(std::string("Min fitness: " + std::to_string((generation.minFitness != std::numeric_limits<float>::max()) ? generation.minFitness : 0.0)));
				
				imGuiManager->BulletText(std::string("Average hip1 velocity: " + std::to_string(generation.averageHip1Velocity)));
				imGuiManager->BulletText(std::string("Average knee1 velocity: " + std::to_string(generation.averageKnee1Velocity)));
				imGuiManager->BulletText(std::string("Average hip2 velocity: " + std::to_string(generation.averageHip2Velocity)));
				imGuiManager->BulletText(std::string("Average knee2 velocity: " + std::to_string(generation.averageKnee2Velocity)));
				
				imGuiManager->BulletText(std::string("Top hip1 velocity: " + std::to_string(generation.topHip1Velocity)));
				imGuiManager->BulletText(std::string("Top knee1 velocity: " + std::to_string(generation.topKnee1Velocity)));
				imGuiManager->BulletText(std::string("Top hip2 velocity: " + std::to_string(generation.topHip2Velocity)));
				imGuiManager->BulletText(std::string("Top knee2 velocity: " + std::to_string(generation.topKnee2Velocity)));
				imGuiManager->BulletText(std::string("Min hip1 velocity: " + std::to_string(generation.minHip1Velocity)));
				imGuiManager->BulletText(std::string("Min knee1 velocity: " + std::to_string(generation.minKnee1Velocity)));
				imGuiManager->BulletText(std::string("Min hip2 velocity: " + std::to_string(generation.minHip2Velocity)));
				imGuiManager->BulletText(std::string("Min knee2 velocity: " + std::to_string(generation.minKnee2Velocity)));

				imGuiManager->BulletText(std::string("Average hip1 rotation boundaries: <" + std::to_string(generation.averageHip1RotationBoundaries.first) + ", " + std::to_string(generation.averageHip1RotationBoundaries.second) + ">"));
				imGuiManager->BulletText(std::string("Average knee1 rotation boundaries: <" + std::to_string(generation.averageKnee1RotationBoundaries.first) + ", " + std::to_string(generation.averageKnee1RotationBoundaries.second) + ">"));
				imGuiManager->BulletText(std::string("Average hip2 rotation boundaries: <" + std::to_string(generation.averageHip2RotationBoundaries.first) + ", " + std::to_string(generation.averageHip2RotationBoundaries.second) + ">"));
				imGuiManager->BulletText(std::string("Average knee2 rotation boundaries: <" + std::to_string(generation.averageKnee2RotationBoundaries.first) + ", " + std::to_string(generation.averageKnee2RotationBoundaries.second) + ">"));

				imGuiManager->BulletText(std::string("Top hip1 rotation: " + std::to_string(generation.topHip1Rotation)));
				imGuiManager->BulletText(std::string("Top knee1 rotation: " + std::to_string(generation.topKnee1Rotation)));
				imGuiManager->BulletText(std::string("Top hip2 rotation: " + std::to_string(generation.topHip2Rotation)));
				imGuiManager->BulletText(std::string("Top knee2 rotation: " + std::to_string(generation.topKnee2Rotation)));

				imGuiManager->BulletText(std::string("Min hip1 rotation: " + std::to_string(generation.minHip1Rotation)));
				imGuiManager->BulletText(std::string("Min knee1 rotation: " + std::to_string(generation.minKnee1Rotation)));
				imGuiManager->BulletText(std::string("Min hip2 rotation: " + std::to_string(generation.minHip2Rotation)));
				imGuiManager->BulletText(std::string("Min knee2 rotation: " + std::to_string(generation.minKnee2Rotation)));

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
	Mutation();

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
	int numberOfParameters = 8;

	// Values to crossover:
	// -1. Flexibility. (always the same mechanism)
	// 0. Hip1 rotation boundaries
	// 1. Knee1 rotation boundaries
	// 2. Hip2 rotation boundaries
	// 3. Knee2 rotation boundaries
	// 4. Hip1 velocity
	// 5. Knee1 velocity
	// 6. Hip2 velocity
	// 7. Knee2 velocity

	for (auto gene : genesToUpdate) {
		// Parent1 will be the parent with best fitness
		ESkeleton* parent1 = nullptr;
		ESkeleton* parent2 = nullptr;

		while (parent1 == parent2) {
			parent1 = newPopulation[Random::get<int>(0, newPopulation.size() - 1)];
			parent2 = newPopulation[Random::get<int>(0, newPopulation.size() - 1)];
		}

		if (parent1->GetFitness() < parent2->GetFitness()) {
			auto aux = parent1;
			parent1 = parent2;
			parent2 = aux;
		}

		// Flexibility will be always the higher one
		gene->SetFlexibility(std::max(parent1->GetFlexibility(), parent2->GetFlexibility()));

		switch (Config::crossoverType) {
			// Select a random point and before this point all the values will be from parent1 and after from parent2
			case Config::CrossoverType::ONEPOINT: {
				int point = Random::get<int>(1, numberOfParameters - 1);
				int pivot = 0;
				auto updateSelectedParent = [&parent1, &parent2, &point, &pivot]() {
					auto parent = (pivot < point) ? parent1 : parent2;
					pivot++;
					return parent;
				};

				// 0. Hip1 rotation boundaries
				gene->GetLeg1()[0]->SetRotationBoundaries(updateSelectedParent()->GetLeg1()[0]->GetRotationBoundaries());
				// 1. Knee1 rotation boundaries
				gene->GetLeg1()[1]->SetRotationBoundaries(updateSelectedParent()->GetLeg1()[1]->GetRotationBoundaries());
				// 2. Hip2 rotation boundaries
				gene->GetLeg2()[0]->SetRotationBoundaries(updateSelectedParent()->GetLeg2()[0]->GetRotationBoundaries());
				// 3. Knee2 rotation boundaries
				gene->GetLeg2()[1]->SetRotationBoundaries(updateSelectedParent()->GetLeg2()[1]->GetRotationBoundaries());
				// 4. Hip1 velocity
				gene->GetLeg1()[0]->SetRotationVelocity(updateSelectedParent()->GetLeg1()[0]->GetRotationVelocity());
				// 5. Knee1 velocity
				gene->GetLeg1()[1]->SetRotationVelocity(updateSelectedParent()->GetLeg1()[1]->GetRotationVelocity());
				// 6. Hip2 velocity
				gene->GetLeg2()[0]->SetRotationVelocity(updateSelectedParent()->GetLeg2()[0]->GetRotationVelocity());
				// 7. Knee2 velocity
				gene->GetLeg2()[1]->SetRotationVelocity(updateSelectedParent()->GetLeg2()[1]->GetRotationVelocity());

				break;
			}
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
/// Mutate population.
/// </summary>
void GeneticAlgorithm::Mutation() {
	std::cout << "Mutation not implemented...\n";
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

	float size = (float)population.size();
	float hip1Velocity = 0.0;
	float knee1Velocity = 0.0;
	float hip2Velocity = 0.0;
	float knee2Velocity = 0.0;
	std::pair<float, float> totalHip1Rotation;
	std::pair<float, float> totalKnee1Rotation;
	std::pair<float, float> totalHip2Rotation;
	std::pair<float, float> totalKnee2Rotation;
	for (auto gene : population) {
		auto hip1 = gene->GetLeg1()[0];
		auto knee1 = gene->GetLeg1()[1];
		auto hip2 = gene->GetLeg2()[0];
		auto knee2 = gene->GetLeg2()[1];

		// Velocities values
		hip1Velocity += std::abs(hip1->GetRotationVelocity().x);
		generationStats.minHip1Velocity = (std::abs(hip1->GetRotationVelocity().x) < generationStats.minHip1Velocity) ? std::abs(hip1->GetRotationVelocity().x) : generationStats.minHip1Velocity;
		generationStats.topHip1Velocity = (std::abs(hip1->GetRotationVelocity().x) > generationStats.topHip1Velocity) ? std::abs(hip1->GetRotationVelocity().x) : generationStats.topHip1Velocity;
		
		knee1Velocity += std::abs(knee1->GetRotationVelocity().x);
		generationStats.minKnee1Velocity = (std::abs(knee1->GetRotationVelocity().x) < generationStats.minKnee1Velocity) ? std::abs(knee1->GetRotationVelocity().x) : generationStats.minKnee1Velocity;
		generationStats.topKnee1Velocity = (std::abs(knee1->GetRotationVelocity().x) > generationStats.topKnee1Velocity) ? std::abs(knee1->GetRotationVelocity().x) : generationStats.topKnee1Velocity;

		hip2Velocity += std::abs(hip2->GetRotationVelocity().x);
		generationStats.minHip2Velocity = (std::abs(hip2->GetRotationVelocity().x) < generationStats.minHip2Velocity) ? std::abs(hip2->GetRotationVelocity().x) : generationStats.minHip2Velocity;
		generationStats.topHip2Velocity = (std::abs(hip2->GetRotationVelocity().x) > generationStats.topHip2Velocity) ? std::abs(hip2->GetRotationVelocity().x) : generationStats.topHip2Velocity;

		knee2Velocity += std::abs(knee2->GetRotationVelocity().x);
		generationStats.minKnee2Velocity = (std::abs(knee2->GetRotationVelocity().x) < generationStats.minKnee2Velocity) ? std::abs(knee2->GetRotationVelocity().x) : generationStats.minKnee2Velocity;
		generationStats.topKnee2Velocity = (std::abs(knee2->GetRotationVelocity().x) > generationStats.topKnee2Velocity) ? std::abs(knee2->GetRotationVelocity().x) : generationStats.topKnee2Velocity;

		// Rotation values
		totalHip1Rotation.first += hip1->GetRotationBoundaries().first;
		totalHip1Rotation.second += hip1->GetRotationBoundaries().second;
		generationStats.minHip1Rotation = (hip1->GetRotationBoundaries().first < generationStats.minHip1Rotation) ? hip1->GetRotationBoundaries().first : generationStats.minHip1Rotation;
		generationStats.topHip1Rotation = (hip1->GetRotationBoundaries().second > generationStats.topHip1Rotation) ? hip1->GetRotationBoundaries().second : generationStats.topHip1Rotation;

		totalKnee1Rotation.first += knee1->GetRotationBoundaries().first;
		totalKnee1Rotation.second += knee1->GetRotationBoundaries().second;
		generationStats.minKnee1Rotation = (knee1->GetRotationBoundaries().first < generationStats.minKnee1Rotation) ? knee1->GetRotationBoundaries().first : generationStats.minKnee1Rotation;
		generationStats.topKnee1Rotation = (knee1->GetRotationBoundaries().second > generationStats.topKnee1Rotation) ? knee1->GetRotationBoundaries().second : generationStats.topKnee1Rotation;

		totalHip2Rotation.first += hip2->GetRotationBoundaries().first;
		totalHip2Rotation.second += hip2->GetRotationBoundaries().second;
		generationStats.minHip2Rotation = (hip2->GetRotationBoundaries().first < generationStats.minHip2Rotation) ? hip2->GetRotationBoundaries().first : generationStats.minHip2Rotation;
		generationStats.topHip2Rotation = (hip2->GetRotationBoundaries().second > generationStats.topHip2Rotation) ? hip2->GetRotationBoundaries().second : generationStats.topHip2Rotation;

		totalKnee2Rotation.first += knee2->GetRotationBoundaries().first;
		totalKnee2Rotation.second += knee2->GetRotationBoundaries().second;
		generationStats.minKnee2Rotation = (knee2->GetRotationBoundaries().first < generationStats.minKnee2Rotation) ? knee2->GetRotationBoundaries().first : generationStats.minKnee2Rotation;
		generationStats.topKnee2Rotation = (knee2->GetRotationBoundaries().second > generationStats.topKnee2Rotation) ? knee2->GetRotationBoundaries().second : generationStats.topKnee2Rotation;

	}

	generationStats.averageHip1Velocity = hip1Velocity / size;
	generationStats.averageKnee1Velocity = knee1Velocity / size;
	generationStats.averageHip2Velocity = hip2Velocity / size;
	generationStats.averageKnee2Velocity = knee2Velocity / size;

	generationStats.averageHip1RotationBoundaries = std::pair<float, float>(totalHip1Rotation.first / size, totalHip1Rotation.second / size);
	generationStats.averageKnee1RotationBoundaries = std::pair<float, float>(totalKnee1Rotation.first / size, totalKnee1Rotation.second / size);
	generationStats.averageHip2RotationBoundaries = std::pair<float, float>(totalHip2Rotation.first / size, totalHip2Rotation.second / size);
	generationStats.averageKnee2RotationBoundaries = std::pair<float, float>(totalKnee2Rotation.first / size, totalKnee2Rotation.second / size);

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
