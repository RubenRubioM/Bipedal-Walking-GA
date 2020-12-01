#include "GeneticAlgorithm.h"

#include <Utils/Config.h>
#include <Utils/Utils.h>
#include <Entities/Compositions/ESkeleton.h>
#include <Entities/EMesh.h>
#include <Render/ImGuiManager.h>

#include <CSV/csv2.hpp>
#include <GLM/glm.hpp>
#include <RANDOM/random.hpp>
#include <IMGUI/implot.h>
#include <NUMCPP/NumCpp.hpp>

#include <iostream>
#include <algorithm>
#include <ctime>

using Random = effolkronium::random_static;

/// <summary>
/// GeneticAlgorithm constructor.
/// </summary>
GeneticAlgorithm::GeneticAlgorithm() {
	imGuiManager = ImGuiManager::GetInstance();

	glm::vec3 offset = Utils::defaultPosition;
	glm::vec3 offsetIncrese = Utils::positionOffset;

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
		
		GenerateRandomSkeletonValues(population[i].get());

		offset.x += offsetIncrese.x;
	}

	std::cout << "==============================\n";

	std::cout << "Population size: " << Config::populationSize << std::endl;
	std::cout << "Life span: " << Config::generationLifeSpan << std::endl;
	std::cout << "Max generations: " << Config::maxGenerations << std::endl;
	std::cout << "New genes probability: " << Config::newGenProbability * 100 << "%" << std::endl;
	std::cout << "Mutation rate: " << Config::mutationProbability * 100 << "%" <<std::endl;

	std::string selectionFunction;
	switch (Config::selectionFunction) {
		case Config::SelectionFunction::ROULETTE: {
			selectionFunction = "Roulette";
			break;
		}
		case Config::SelectionFunction::TOURNAMENT: {
			selectionFunction = "Tournament-" + std::to_string(Config::tournamentMembers);
			break;
		}
	}
	std::cout << "Selection function: " << selectionFunction << std::endl;
	
	std::string crossoverType;
	switch (Config::crossoverType) {
		case(Config::CrossoverType::HEURISTIC): {
			crossoverType = "Heuristic";
			break;
		}
		case(Config::CrossoverType::ARITHMETIC): {
			crossoverType = "Arithmetic";
			break;
		}
		case(Config::CrossoverType::AVERAGE): {
			crossoverType = "Average";
			break;
		}
		case(Config::CrossoverType::ONEPOINT): {
			crossoverType = "One point";
			break;
		}
	}
	std::cout << "Crossover type: " << crossoverType << std::endl;

	std::cout << "==============================\n";

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
		imGuiManager->BulletText(std::string("Generation " + std::to_string(actualGeneration) + " (" + std::to_string(Config::maxGenerations - actualGeneration) + " left)"));
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
			
			if (gene->GetFitness() > topGeneFitness) {
				topGeneFitness = gene->GetFitness();
				bestGeneId = gene->GetSkeletonId();
			}

			minGeneFitness = (gene->GetFitness() < minGeneFitness && !gene->IsDead()) ? gene->GetFitness() : minGeneFitness;
		}
	}

	deathPercentage = (deads / (float)Config::populationSize) * 100.0;
	averageFitness = (totalFitness / (float)Config::populationSize);
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

				imGuiManager->BulletText(std::string("Best gene hip1 rotation boundaries: <" + std::to_string(generation.bestHip1RotationBoundaries.first) + ", " + std::to_string(generation.bestHip1RotationBoundaries.second) + ">"));
				imGuiManager->BulletText(std::string("Best gene knee1 rotation boundaries: <" + std::to_string(generation.bestKnee1RotationBoundaries.first) + ", " + std::to_string(generation.bestKnee1RotationBoundaries.second) + ">"));
				imGuiManager->BulletText(std::string("Best gene hip2 rotation boundaries: <" + std::to_string(generation.bestHip2RotationBoundaries.first) + ", " + std::to_string(generation.bestHip2RotationBoundaries.second) + ">"));
				imGuiManager->BulletText(std::string("Best gene knee2 rotation boundaries: <" + std::to_string(generation.bestKnee2RotationBoundaries.first) + ", " + std::to_string(generation.bestKnee2RotationBoundaries.second) + ">"));

				imGuiManager->BulletText(std::string("Best gene hip1 velocity: " + std::to_string(generation.bestHip1Velocity)));
				imGuiManager->BulletText(std::string("Best gene knee1 velocity: " + std::to_string(generation.bestKnee1Velocity)));
				imGuiManager->BulletText(std::string("Best gene hip2 velocity: " + std::to_string(generation.bestHip2Velocity)));
				imGuiManager->BulletText(std::string("Best gene knee2 velocity: " + std::to_string(generation.bestKnee2Velocity)));

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
	auto gene = GetBestGene();
	// Genetic algorithm flow: Selection -> Crossover -> Mutation
	auto pairPopulation = Selection();
	Crossover(pairPopulation);
	Mutation();

	SetDefaultPopulationValues();
	ResetStats();
	actualGeneration++;
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
			// Sort the population based on the gene fitness. Worst = first
			std::sort(populationAux.begin(), populationAux.end(), [](std::shared_ptr<ESkeleton> skeleton1, std::shared_ptr<ESkeleton> skeleton2) {
				return skeleton1->GetFitness() < skeleton2->GetFitness();
			});

			// We have to recalculate rank table because the population change
			while (genesToNewGeneration != Config::populationSize - newGenes) {
				// Calculate the sum of all gene fitness
				float totalFitness = 0;
				int j = 0;
				for (auto gene : populationAux) {
					totalFitness += gene->GetFitness();
					j++;
				}
				
				// Create an array with probabilities from 0 to 1
				nc::NdArray<float> rankProbabilities = nc::zeros<float>(1, populationAux.size() + 1);
				float actualTotalFitness = 0;
				for (uint16_t i = 1; i < rankProbabilities.shape().cols; i++) {
					rankProbabilities[0, i] = (populationAux[i - 1]->GetFitness() / totalFitness) + actualTotalFitness;
					actualTotalFitness += populationAux[i - 1]->GetFitness() / totalFitness;
				}

				// Calculate the random value and check which gene is
				float probabilityValue = Random::get<float>(0, 1);
				int selectionIndx = 0;

				for (int i = 0; i < rankProbabilities.shape().cols - 1; i++) {
					if (rankProbabilities[0, i] <= probabilityValue && probabilityValue <= rankProbabilities[0, i + 1]) {
						newPopulation.push_back(populationAux[i].get());
						populationAux.erase(populationAux.begin() + i);
						genesToNewGeneration++;
						break;
					}
				}
			}

			break;
		}

		case Config::SelectionFunction::TOURNAMENT: {
			// NOTE: Since we have a sorted vector by fitness, the tournament winner will be always the minimum index
			//		 between all the Config::tournamentMembers but just for scalability we will not have this in count and
			//		 we will act as if the vector is not sorted.
			while (genesToNewGeneration != Config::populationSize - newGenes) {
				std::vector<int> membersIndexs;

				// If we have enought genes to set a tournament
				if (Config::tournamentMembers <= populationAux.size()) {
					while (membersIndexs.size() < Config::tournamentMembers) {
						int indx = Random::get<int>(0, populationAux.size() - 1);
						bool repeated = false;

						for (int memberIndex : membersIndexs) {
							if (memberIndex == indx) {
								repeated = true;
								break;
							}
						}

						// If the index is not repeated, add it to the vector
						if (!repeated)
							membersIndexs.push_back(indx);
					}
				}
				else {
					// If we have less genes than members to pick for the tournament just pick all the rest
					for (uint16_t i = 0; i < populationAux.size(); ++i) {
						membersIndexs.push_back(i);
					}
				}

				// Now, FIGHT!
				int winnerIndex = 0;
				float maxTournamentFitness = std::numeric_limits<float>::lowest();

				for (int index : membersIndexs) {
					if (populationAux[index]->GetFitness() > maxTournamentFitness) {
						winnerIndex = index;
						maxTournamentFitness = populationAux[index]->GetFitness();
					}
				}

				newPopulation.push_back(populationAux[winnerIndex].get());
				populationAux.erase(populationAux.begin() + winnerIndex);
				genesToNewGeneration++;
			}

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

		// Lambda functions to set the values based on the lambda functions by parameter
		auto setValues = [&gene,&parent1,&parent2](std::function<std::pair<float, float>(std::pair<float, float>, std::pair<float, float>)> pairLambda, std::function<glm::vec3(glm::vec3, glm::vec3)> vec3Lambda) {
			// 0. Hip1 rotation boundaries
			gene->GetLeg1()[0]->SetRotationBoundaries(pairLambda(parent1->GetLeg1()[0]->GetRotationBoundaries(), parent2->GetLeg1()[0]->GetRotationBoundaries()));

			// 1. Knee1 rotation boundaries
			gene->GetLeg1()[1]->SetRotationBoundaries(pairLambda(parent1->GetLeg1()[1]->GetRotationBoundaries(), parent2->GetLeg1()[1]->GetRotationBoundaries()));

			// 2. Hip2 rotation boundaries
			gene->GetLeg2()[0]->SetRotationBoundaries(pairLambda(parent1->GetLeg2()[0]->GetRotationBoundaries(), parent2->GetLeg2()[0]->GetRotationBoundaries()));

			// 3. Knee2 rotation boundaries
			gene->GetLeg2()[1]->SetRotationBoundaries(pairLambda(parent1->GetLeg2()[1]->GetRotationBoundaries(), parent2->GetLeg2()[1]->GetRotationBoundaries()));

			// 4. Hip1 velocity
			gene->GetLeg1()[0]->SetRotationVelocity(vec3Lambda(parent1->GetLeg1()[0]->GetRotationVelocity(), parent2->GetLeg1()[0]->GetRotationVelocity()));

			//// 5. Knee1 velocity
			gene->GetLeg1()[1]->SetRotationVelocity(vec3Lambda(parent1->GetLeg1()[1]->GetRotationVelocity(), parent2->GetLeg1()[1]->GetRotationVelocity()));

			//// 6. Hip2 velocity
			gene->GetLeg2()[0]->SetRotationVelocity(vec3Lambda(parent1->GetLeg2()[0]->GetRotationVelocity(), parent2->GetLeg2()[0]->GetRotationVelocity()));

			//// 7. Knee2 velocity
			gene->GetLeg2()[1]->SetRotationVelocity(vec3Lambda(parent1->GetLeg2()[1]->GetRotationVelocity(), parent2->GetLeg2()[1]->GetRotationVelocity()));

		};

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
				auto average = [](float value1, float value2) {
					return (value1 + value2) / 2;
				};
				auto pairAverage = [&average](std::pair<float, float> v1, std::pair<float, float> v2) {
					return std::pair<float, float>(average(v1.first, v2.first), average(v1.second, v2.second));
				};
				auto vec3Average = [&average](glm::vec3 v1, glm::vec3 v2) {
					return glm::vec3(average(v1.x, v2.x), average(v1.y, v2.y), average(v1.z, v2.z));
				};

				setValues(pairAverage, vec3Average);
				
				break;
			}
			case Config::CrossoverType::ARITHMETIC: {
				float alpha = Random::get<float>(0, 1);
				auto arithmetic = [&alpha](float bestValue, float worstValue) {
					return (alpha * bestValue) + (1 - alpha) * worstValue;
				};
				auto pairArithmetic = [&arithmetic](std::pair<float, float> bestPair, std::pair<float, float> worstPair) {
					return std::pair<float, float>(arithmetic(bestPair.first, worstPair.first), arithmetic(bestPair.second, worstPair.second));
				};
				auto vec3Arithmetic = [&arithmetic](glm::vec3 bestVec3, glm::vec3 worstVec3) {
					return glm::vec3(arithmetic(bestVec3.x, worstVec3.x), arithmetic(bestVec3.y, worstVec3.y), arithmetic(bestVec3.z, worstVec3.z));
				};

				setValues(pairArithmetic, vec3Arithmetic);
				break;
			}
			case Config::CrossoverType::HEURISTIC: {
				auto heuristic = [](float bestValue, float worstValue, float ratio) {
					return bestValue + (ratio * (bestValue - worstValue));
				};
				auto pairHeuristic = [&heuristic](std::pair<float, float> bestPair, std::pair<float, float> worstPair, float min, float max) {
					auto pair = std::pair<float, float>(worstPair.first,worstPair.second);

					for (uint16_t i = 0; i < Config::heuristicTries; ++i) {
						float ratio = Random::get<float>(0, 1);

						pair = std::pair<float, float>(heuristic(bestPair.first, worstPair.first,ratio), heuristic(bestPair.second, worstPair.second,ratio));
						if ((pair.first >= min && pair.first <= max) && (pair.second >= min && pair.second <= max)) {
							return pair;
						}
					}
					pair = std::pair<float, float>(worstPair.first, worstPair.second);

					return pair;
				};
				auto vec3Heuristic = [&heuristic](glm::vec3 bestVec3, glm::vec3 worstVec3, float min, float max) {
					auto vec3 = glm::vec3(worstVec3.x, worstVec3.y, worstVec3.z);

					for (uint16_t i = 0; i < Config::heuristicTries; i++) {
						float ratio = Random::get<float>(0, 1);

						vec3 = glm::vec3(heuristic(bestVec3.x, worstVec3.x,ratio), heuristic(bestVec3.y, worstVec3.y,ratio), heuristic(bestVec3.z, worstVec3.z,ratio));
						if (vec3.x >= min && vec3.x <= max) {
							return vec3;
						}
					}
					vec3 = glm::vec3(worstVec3.x, worstVec3.y, worstVec3.z);

					return vec3;
				};

				// TODO: Cambiar esto por un valor n para hacer pruebas y que no se salgan de los valores limites.
				
				float rotationVelocityMin = Config::rotationVelocityBoundaries.first.x;
				float rotationVelocityMax = Config::rotationVelocityBoundaries.second.x;

				// 0. Hip1 rotation boundaries
				gene->GetLeg1()[0]->SetRotationBoundaries(pairHeuristic(parent1->GetLeg1()[0]->GetRotationBoundaries(), parent2->GetLeg1()[0]->GetRotationBoundaries(),-160,160));
				// 1. Knee1 rotation boundaries
				gene->GetLeg1()[1]->SetRotationBoundaries(pairHeuristic(parent1->GetLeg1()[1]->GetRotationBoundaries(), parent2->GetLeg1()[1]->GetRotationBoundaries(), -160, 0));
				// 2. Hip2 rotation boundaries
				gene->GetLeg2()[0]->SetRotationBoundaries(pairHeuristic(parent1->GetLeg2()[0]->GetRotationBoundaries(), parent2->GetLeg2()[0]->GetRotationBoundaries(), -160, 160));
				// 3. Knee2 rotation boundaries
				gene->GetLeg2()[1]->SetRotationBoundaries(pairHeuristic(parent1->GetLeg2()[1]->GetRotationBoundaries(), parent2->GetLeg2()[1]->GetRotationBoundaries(), -160, 0));
				// 4. Hip1 velocity
				gene->GetLeg1()[0]->SetRotationVelocity(vec3Heuristic(parent1->GetLeg1()[0]->GetRotationVelocity(), parent2->GetLeg1()[0]->GetRotationVelocity(), rotationVelocityMin, rotationVelocityMax));
				// 5. Knee1 velocity
				gene->GetLeg1()[1]->SetRotationVelocity(vec3Heuristic(parent1->GetLeg1()[1]->GetRotationVelocity(), parent2->GetLeg1()[1]->GetRotationVelocity(), rotationVelocityMin, rotationVelocityMax));
				// 6. Hip2 velocity
				gene->GetLeg2()[0]->SetRotationVelocity(vec3Heuristic(parent1->GetLeg2()[0]->GetRotationVelocity(), parent2->GetLeg2()[0]->GetRotationVelocity(), rotationVelocityMin, rotationVelocityMax));
				// 7. Knee2 velocity
				gene->GetLeg2()[1]->SetRotationVelocity(vec3Heuristic(parent1->GetLeg2()[1]->GetRotationVelocity(), parent2->GetLeg2()[1]->GetRotationVelocity(), rotationVelocityMin, rotationVelocityMax));
				
				break;
			}
		}
	}
}

/// <summary>
/// Mutate population.
/// </summary>
void GeneticAlgorithm::Mutation() {
	for (auto gene : population) {
		float prob = Random::get<float>(0, 1);

		if (prob <= Config::mutationProbability) {
			GenerateRandomSkeletonValues(gene.get());
		}
	}
}

/// <summary>
/// Saves the generation stats.
/// </summary>
void GeneticAlgorithm::SaveGenerationStats() {
	GenerationStats generationStats;
	generationStats.generation = actualGeneration;
	generationStats.bestGeneId = bestGeneId;
	generationStats.deathPercentage = deathPercentage;
	generationStats.averageFitness = averageFitness;
	generationStats.topFitness = topFitness;
	generationStats.minFitness = minFitness;

	auto bestGene = GetBestGene();

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

		generationStats.bestHip1RotationBoundaries = bestGene->GetLeg1()[0]->GetRotationBoundaries();
		generationStats.bestKnee1RotationBoundaries = bestGene->GetLeg1()[1]->GetRotationBoundaries();
		generationStats.bestHip2RotationBoundaries = bestGene->GetLeg2()[0]->GetRotationBoundaries();
		generationStats.bestKnee2RotationBoundaries = bestGene->GetLeg2()[1]->GetRotationBoundaries();
		generationStats.bestHip1Velocity = std::abs(bestGene->GetLeg1()[0]->GetRotationVelocity().x);
		generationStats.bestKnee1Velocity = std::abs(bestGene->GetLeg1()[1]->GetRotationVelocity().x);
		generationStats.bestHip2Velocity = std::abs(bestGene->GetLeg2()[0]->GetRotationVelocity().x);
		generationStats.bestKnee2Velocity = std::abs(bestGene->GetLeg2()[1]->GetRotationVelocity().x);
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

/// <summary>
/// Returns the best gene of the last generation
/// </summary>
/// <returns> Best gene of the last generation. </returns>
ESkeleton* GeneticAlgorithm::GetBestGeneLastGeneration() {
	if (generationsStats.size() > 0) {
		int id = generationsStats[actualGeneration - 2].bestGeneId;

		for (auto gene : population)
			if (gene->GetSkeletonId() == id) return gene.get();
	}
	return nullptr;
}

/// <summary>
/// Returns the best gene.
/// </summary>
/// <returns> Best gene. </returns>
ESkeleton* GeneticAlgorithm::GetBestGene() {
	for (auto gene : population)
		if (gene->GetSkeletonId() == bestGeneId) return gene.get();

	return nullptr;
}

/// <summary>
/// Generates random skeleton values.
/// </summary>
/// <param name="skeleton"> Skeleton. </param>
void GeneticAlgorithm::GenerateRandomSkeletonValues(ESkeleton* skeleton) {
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

	// Set skeleton flexibility
	float flexibility = Random::get<float>(0, 1);
	if (flexibility >= 0 && flexibility <= Config::flexibilityProbability.x) {
		skeleton->SetFlexibility(ESkeleton::Flexibility::LOW);
	}
	else if (flexibility > Config::flexibilityProbability.x && flexibility <= Config::flexibilityProbability.x + Config::flexibilityProbability.y) {
		skeleton->SetFlexibility(ESkeleton::Flexibility::MEDIUM);
	}
	else if (flexibility > Config::flexibilityProbability.x + Config::flexibilityProbability.y) {
		skeleton->SetFlexibility(ESkeleton::Flexibility::HIGH);
	}
	else {
		std::cout << "No flexibility probability found, automatic set to Flexibility::HIGH" << std::endl;
		skeleton->SetFlexibility(ESkeleton::Flexibility::HIGH);
	}

	// Set legs boundaries.
	setLegBoundaries(skeleton->GetLeg1());
	setLegBoundaries(skeleton->GetLeg2());
}

/// <summary>
/// Exports the generation's data to a csv.
/// </summary>
void GeneticAlgorithm::WriteCSV() {
	int randomNumber = Random::get<int>(0, std::numeric_limits<int>::max());
	// current date/time based on current system
	std::time_t now = std::time(0);

	// convert now to string form
	std::string time = std::asctime(std::localtime(&now));
	std::string title("Simulations/Simulation" + std::to_string(randomNumber) +".csv");
	std::ofstream stream(title);
	csv2::Writer<csv2::delimiter<','>> writer(stream);

	std::string selectionFunction;
	switch (Config::selectionFunction) {
		case Config::SelectionFunction::ROULETTE: {
			selectionFunction = "Roulette";
			break;
		}
		case Config::SelectionFunction::TOURNAMENT: {
			selectionFunction = "Tournament";
			break;
		}
	}

	std::string crossoverType;
	switch (Config::crossoverType) {
		case(Config::CrossoverType::HEURISTIC): {
			crossoverType = "Heuristic";
			break;
		}
		case(Config::CrossoverType::ARITHMETIC): {
			crossoverType = "Arithmetic";
			break;
		}
		case(Config::CrossoverType::AVERAGE): {
			crossoverType = "Average";
			break;
		}
		case(Config::CrossoverType::ONEPOINT): {
			crossoverType = "One point";
			break;
		}
	}

	std::vector<std::vector<std::string>> rows =
	{
		{"Population size", "Life span (sec)", "Max generations", "New genes probability", "Mutation rate", "Selection function", "Crossover operator"},
		{std::to_string(Config::populationSize), std::to_string(Config::generationLifeSpan),std::to_string(Config::maxGenerations)
		, std::to_string(Config::newGenProbability), std::to_string(Config::mutationProbability), selectionFunction, crossoverType},
		{"", "", "", "", "Tournament members", std::to_string(Config::tournamentMembers), ""},
		{"", "", "", "", "", "", ""},

		{"Generation", "Death percentage", "Average fitness", "Top fitness", "Min fitness"
		, "Average hip1 velocity", "Average knee1 velocity", "Average hip2 velocity", "Average knee2 velocity"
		, "Top hip1 velocity", "Top knee1 velocity", "Top hip2 velocity", "Top knee2 velocity"
		, "Min hip1 velocity", "Min knee1 velocity", "Min hip2 velocity", "Min knee2 velocity"
		, "Average hip1 rotation", "Average knee1 rotation", "Average hip2 rotation", "Average knee2 rotation"
		, "Top hip1 rotation", "Top knee1 rotation", "Top hip2 rotation", "Top knee2 rotation"
		, "Min hip1 rotation", "Min knee1 rotation", "Min hip2 rotation", "Min knee2 rotation"
		, "Best gene hip1 rotation", "Best gene knee1 rotation", "Best gene hip2 rotation", "Best gene knee2 rotation"
	    , "Best gene hip1 velocity", "Best gene knee1 velocity", "Best gene hip2 velocity", "Best gene knee2 velocity", }
	};
	int actualRow = 5;
	float totalDeath = 0.0f;
	float totalFitness = 0.0f;
	float totalMaxFitness = 0.0f;
	float totalMinFitness = 0.0f;
	for (auto generation : generationsStats) {
		std::vector<std::string> row;
		row.push_back(std::to_string(generation.generation));
		totalDeath += generation.deathPercentage;
		row.push_back(std::to_string(generation.deathPercentage));
		totalFitness += generation.averageFitness;
		row.push_back(std::to_string(generation.averageFitness));
		totalMaxFitness += generation.topFitness;
		row.push_back(std::to_string(generation.topFitness));
		totalMinFitness += generation.minFitness;
		row.push_back(std::to_string(generation.minFitness));
		row.push_back(std::to_string(generation.averageHip1Velocity));
		row.push_back(std::to_string(generation.averageKnee1Velocity));
		row.push_back(std::to_string(generation.averageHip2Velocity));
		row.push_back(std::to_string(generation.averageKnee2Velocity));
		row.push_back(std::to_string(generation.topHip1Velocity));
		row.push_back(std::to_string(generation.topKnee1Velocity));
		row.push_back(std::to_string(generation.topHip2Velocity));
		row.push_back(std::to_string(generation.topKnee2Velocity));
		row.push_back(std::to_string(generation.minHip1Velocity));
		row.push_back(std::to_string(generation.minKnee1Velocity));
		row.push_back(std::to_string(generation.minHip2Velocity));
		row.push_back(std::to_string(generation.minKnee2Velocity));
		row.push_back(std::string("<" + std::to_string(generation.averageHip1RotationBoundaries.first) + " | " + std::to_string(generation.averageHip1RotationBoundaries.second) + ">"));
		row.push_back(std::string("<" + std::to_string(generation.averageKnee1RotationBoundaries.first) + " | " + std::to_string(generation.averageKnee1RotationBoundaries.second) + ">"));
		row.push_back(std::string("<" + std::to_string(generation.averageHip2RotationBoundaries.first) + " | " + std::to_string(generation.averageHip2RotationBoundaries.second) + ">"));
		row.push_back(std::string("<" + std::to_string(generation.averageKnee2RotationBoundaries.first) + " | " + std::to_string(generation.averageKnee2RotationBoundaries.second) + ">"));
		row.push_back(std::string("<" + std::to_string(generation.topHip1Rotation) + " | " + std::to_string(generation.topHip1Rotation) + ">"));
		row.push_back(std::string("<" + std::to_string(generation.topKnee1Rotation) + " | " + std::to_string(generation.topKnee1Rotation) + ">"));
		row.push_back(std::string("<" + std::to_string(generation.topHip2Rotation) + " | " + std::to_string(generation.topHip2Rotation) + ">"));
		row.push_back(std::string("<" + std::to_string(generation.topKnee2Rotation) + " | " + std::to_string(generation.topKnee2Rotation) + ">"));
		row.push_back(std::string("<" + std::to_string(generation.minHip1Rotation) + " | " + std::to_string(generation.minHip1Rotation) + ">"));
		row.push_back(std::string("<" + std::to_string(generation.minKnee1Rotation) + " | " + std::to_string(generation.minKnee1Rotation) + ">"));
		row.push_back(std::string("<" + std::to_string(generation.minHip2Rotation) + " | " + std::to_string(generation.minHip2Rotation) + ">"));
		row.push_back(std::string("<" + std::to_string(generation.minKnee2Rotation) + " | " + std::to_string(generation.minKnee2Rotation) + ">"));
		row.push_back(std::string("<" + std::to_string(generation.bestHip1RotationBoundaries.first) + " | " + std::to_string(generation.bestHip1RotationBoundaries.second) + ">"));
		row.push_back(std::string("<" + std::to_string(generation.bestKnee1RotationBoundaries.first) + " | " + std::to_string(generation.bestKnee1RotationBoundaries.second) + ">"));
		row.push_back(std::string("<" + std::to_string(generation.bestHip2RotationBoundaries.first) + " | " + std::to_string(generation.bestHip2RotationBoundaries.second) + ">"));
		row.push_back(std::string("<" + std::to_string(generation.bestKnee2RotationBoundaries.first) + " | " + std::to_string(generation.bestKnee2RotationBoundaries.second) + ">"));
		row.push_back(std::to_string(generation.bestHip1Velocity));
		row.push_back(std::to_string(generation.bestKnee1Velocity));
		row.push_back(std::to_string(generation.bestHip2Velocity));
		row.push_back(std::to_string(generation.bestKnee2Velocity));

		rows.insert(rows.begin() + actualRow, row);
		actualRow++;
	}
	float meanDeaths = totalDeath / (float)generationsStats.size();
	float meanFitness = totalFitness / (float)generationsStats.size();
	float meanTopFitness = totalMaxFitness / (float)generationsStats.size();
	float meanMinFitness = totalMinFitness / (float)generationsStats.size();
	std::vector<std::string> row;
	row.push_back("Average: ");
	row.push_back(std::to_string(meanDeaths));
	row.push_back(std::to_string(meanFitness));
	row.push_back(std::to_string(meanTopFitness));
	row.push_back(std::to_string(meanMinFitness));
	rows.insert(rows.begin() + actualRow, row);

	writer.write_rows(rows);
	stream.close();

	std::cout << title <<" exported\n";
}