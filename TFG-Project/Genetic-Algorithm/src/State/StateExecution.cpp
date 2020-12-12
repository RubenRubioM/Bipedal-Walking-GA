#include "StateExecution.h"

#include <Render/RenderEngine.h>
#include <Physics/PhysicsEngine.h>
#include <Render/ImGuiManager.h>
#include <Entities/ECamera.h>
#include <Entities/EMesh.h>
#include <Entities/Compositions/ESkeleton.h>
#include <Entities/Entity.h>
#include <DataTypes/Transformable.h>
#include <GeneticAlgorithm/GeneticAlgorithm.h>
#include <Utils/Config.h>
#include <Utils/Utils.h>

#include <IMGUI/imgui.h>
#include <IMGUI/implot.h>
#include <GLM/gtc/type_ptr.hpp>

/// <summary>
/// StateExecution constructor.
/// </summary>
StateExecution::StateExecution() {
	renderEngine = RenderEngine::GetInstance();
	physicsEngine = PhysicsEngine::GetInstance();
	imGuiManager = ImGuiManager::GetInstance();
	camera = make_unique<ECamera>(Transformable(glm::vec3(-150.0f, 100.0f, 150.0f), glm::vec3(0.0f), glm::vec3(1.0f)), glm::vec3(-30.0f, 45, 0));
	camera->SetName("Camera");
	camera->SetPositionBoundaries(std::pair<float, float>(-200,1000));
	renderEngine->AddSkybox("media/skybox/right.jpg"
		, "media/skybox/left.jpg"
		, "media/skybox/top.jpg"
		, "media/skybox/bottom.jpg"
		, "media/skybox/front.jpg"
		, "media/skybox/back.jpg");

	// Field
	terrain.push_back(make_unique<EMesh>(Transformable(glm::vec3(500.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2000.0f, 10.0f, 2000.0f)), "media/Grass_Block.obj"));
	terrain[0]->SetName("Field");


	AddEntities();
}

/// <summary>
/// StateExecution destructor.
/// </summary>
StateExecution::~StateExecution() {
}

/// <summary>
/// StateExecution initialization.
/// </summary>
void StateExecution::InitState() {

}

/// <summary>
/// Called at the start of every frame.
/// </summary>
void StateExecution::InitFrame() {
	imGuiManager->FrameInit();
}

/// <summary>
/// StateExecution update.
/// </summary>
void StateExecution::Update() {
	if (executionStarted) {
		auto setVisible = [&](ESkeleton* skeleton, bool visible) {
			for (auto joint : skeleton->GetSkeleton())
				renderEngine->SetVisible(joint, visible);
		};

		auto time = duration_cast<milliseconds>(std::chrono::system_clock::now() - timeStart).count() - timeToStart.count();
		ImGuiDebug();

		ESkeleton* target = nullptr;
		if (onlyTarget) {
			switch (camera->cameraType) {
				case ECamera::CameraType::BESTGENE: {
					target = geneticAlgorithm->GetBestGene();
					break;
				}
				case ECamera::CameraType::BESTLASTGENE: {
					target = geneticAlgorithm->GetBestGeneLastGeneration();
					break;
				}
			}
			if (target) {
				for (const auto skeleton : skeletons) {
					(skeleton->GetSkeletonId() != target->GetSkeletonId()) ? setVisible(skeleton.get(), false) : setVisible(skeleton.get(), true);
				}
			}
		}else {
			for (const auto skeleton : skeletons)
				setVisible(skeleton.get(), true);
		}

		if (time >= std::chrono::milliseconds(0).count() && geneticAlgorithm->GetGeneration() < Config::maxGenerations + 1) {
			for (const auto& skeleton : skeletons) {
				physicsEngine->UpdateSkeleton(skeleton.get());
			}

			for (const auto& mesh : terrain) {
				physicsEngine->UpdateEntity(mesh.get());
			}
		}

		geneticAlgorithm->Update(time);

		if (time >= Config::generationLifeSpan * MSTOSECONDS && geneticAlgorithm->GetGeneration() < Config::maxGenerations + 1) {
			timeStart = std::chrono::system_clock::now();
			geneticAlgorithm->NewGeneration();
		}

	}else {
		imGuiManager->Begin("Simulation configuration");
		ShowConfigurationWindow();
		imGuiManager->End();
	}
	
}

/// <summary>
/// StateExecution render.
/// </summary>
void StateExecution::Render() {
	renderEngine->BeginScene();
	renderEngine->DrawAll();

	// Now we can render primiteves like lines
	if (executionStarted) {
		imGuiManager->Begin("Debug");

		imGuiManager->BulletText("Camera type");
		if (geneticAlgorithm->GetBestGene()) {
			imGuiManager->RadioButton("Best", (int*)&camera->cameraType, 0); imGuiManager->SameLine();
		}
		if (geneticAlgorithm->GetBestGeneLastGeneration()) {
			imGuiManager->RadioButton("Last generation best", (int*)&camera->cameraType, 1); imGuiManager->SameLine();
		}
		imGuiManager->RadioButton("Free", (int*)&camera->cameraType, 2);

		if (camera->cameraType == ECamera::CameraType::BESTGENE || camera->cameraType == ECamera::CameraType::BESTLASTGENE) {
			imGuiManager->Checkbox("Show only gene target", &onlyTarget);

		}else {
			onlyTarget = false;
		}

		imGuiManager->Separator();

		imGuiManager->Checkbox("Show bounding boxes", &showBoundingBoxes);
		if (showBoundingBoxes) {
			for (const auto& skeleton : skeletons) {
				for (const auto& joint : skeleton->GetSkeleton()) {
					renderEngine->DrawBoundingBox(joint);
				}
			}

			for (const auto& mesh : terrain) {
				renderEngine->DrawBoundingBox(mesh.get());
			}
		}
		imGuiManager->Checkbox("Gravity", &physicsEngine->GetGravityActivated());

		if (imGuiManager->Button("Export dataset")) {
			geneticAlgorithm->WriteCSV();
		}
		imGuiManager->End();
		
		renderEngine->DrawDistanceLines();
		if (auto gene = geneticAlgorithm->GetBestGene()) {
			renderEngine->DrawFirstPlaceLine(gene->GetCore()->GetPosition());
		}
	}
	
	imGuiManager->Render();
	renderEngine->EndScene();
}

/// <summary>
/// Add all the entities to the render engine.
/// </summary>
void StateExecution::AddEntities() {
	renderEngine->AddCamera(camera.get());
	for (const auto& skeleton : skeletons) {
		for (const auto& joint : skeleton->GetSkeleton()) {
			renderEngine->AddMesh(joint);
			physicsEngine->SetEntityValues(joint);
		}
	}

	for (const auto& mesh : terrain) {
		renderEngine->AddMesh(mesh.get());
		physicsEngine->AddCollidingMesh(mesh.get());
		physicsEngine->SetEntityValues(mesh.get());
	}

}

/// <summary>
/// Refactor imGui debug.
/// </summary>
void StateExecution::ImGuiDebug() {
	imGuiManager->Begin("Entities transformables");
	switch (camera->GetCameraType()) {
		case ECamera::CameraType::BESTGENE: {
			physicsEngine->UpdateCamera(camera.get(), geneticAlgorithm->GetBestGene());
			break;
		}
		case ECamera::CameraType::BESTLASTGENE: {
			physicsEngine->UpdateCamera(camera.get(), geneticAlgorithm->GetBestGeneLastGeneration());
			break;
		}
		case ECamera::CameraType::FREE: {
			physicsEngine->UpdateCamera(camera.get());
			break;
		}
	}

	for (const auto& skeleton : skeletons) {
		if (imGuiManager->Header(std::string("Skeleton " + std::to_string(skeleton->GetSkeletonId())))) {
			for (auto joint : skeleton->GetSkeleton()) {
				if (imGuiManager->Header(std::string("Skeleton " + std::to_string(skeleton->GetSkeletonId()) + ". " + joint->GetName()))) {
					imGuiManager->EntityTransformable(joint, std::string(joint->GetName()));
				}
			}
		}
	}
	for (const auto& mesh : terrain) {
		if (imGuiManager->Header(std::string(std::to_string(mesh->GetId()) + ". " + mesh->GetName()))) {
			imGuiManager->EntityTransformable(mesh.get(), std::string(mesh->GetName()));
		}
	}
	imGuiManager->End();
}

/// <summary>
/// Shows configaration window, refactor method.
/// </summary>
void StateExecution::ShowConfigurationWindow() {
	imGuiManager->BulletText("Population size");
	imGuiManager->IntSlider("Number of genes", &Config::populationSize, 2, 200);
	if (imGuiManager->IsHovered()) {
		imGuiManager->BeginTooltip();
		imGuiManager->Text("High values may occuor performance issues");
		imGuiManager->EndTooltip();
	}

	imGuiManager->BulletText("Life span");
	imGuiManager->IntSlider("Seconds of life", &Config::generationLifeSpan, 10, 500);

	imGuiManager->BulletText("Maximum generations");
	imGuiManager->IntSlider("Number of generations", &Config::maxGenerations, 2, 500);

	imGuiManager->BulletText("New genes probability");
	imGuiManager->FloatSlider("Percentage of new genes", &Config::newGenProbability, 0, 1);

	imGuiManager->BulletText("Mutation probability");
	imGuiManager->FloatSlider("Percentage of mutations", &Config::mutationProbability, 0, 1);

	imGuiManager->Separator();

	imGuiManager->BulletText("Selection function");
	imGuiManager->RadioButton("Roulette", (int*)&Config::selectionFunction, 0); imGuiManager->SameLine();
	imGuiManager->RadioButton("Tournament", (int*)&Config::selectionFunction, 1); 

	if (Config::selectionFunction == Config::SelectionFunction::TOURNAMENT) {
		// Shows tournament members.
		imGuiManager->IntSlider("Number of members", &Config::tournamentMembers, 2, Config::populationSize);
		if (imGuiManager->IsHovered()) {
			imGuiManager->BeginTooltip();
			imGuiManager->Text("High number of members will occour in less diversity");
			imGuiManager->EndTooltip();
		}
	}

	imGuiManager->Separator();

	imGuiManager->BulletText("Crossover operator");
	imGuiManager->RadioButton("Heuristic", (int*)&Config::crossoverType, 0); imGuiManager->SameLine();
	imGuiManager->RadioButton("Arithmetic", (int*)&Config::crossoverType, 1); imGuiManager->SameLine();
	imGuiManager->RadioButton("Average", (int*)&Config::crossoverType, 2); imGuiManager->SameLine();
	imGuiManager->RadioButton("One point", (int*)&Config::crossoverType, 3);

	if (Config::crossoverType == Config::CrossoverType::HEURISTIC) {
		// Shows heuristic tries.
		imGuiManager->IntSlider("Number of tries", &Config::heuristicTries, 2, 100);
	}

	imGuiManager->Separator();

	if (imGuiManager->Button("Start simulation")) {
		executionStarted = true;
		timeStart = std::chrono::system_clock::now();
		geneticAlgorithm = make_unique<GeneticAlgorithm>();
		skeletons = geneticAlgorithm->GetPopulation();
		AddEntities();
	}
}