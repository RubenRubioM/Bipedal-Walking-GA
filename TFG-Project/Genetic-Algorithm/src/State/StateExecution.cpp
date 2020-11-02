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
	geneticAlgorithm = make_unique<GeneticAlgorithm>();
	camera = make_unique<ECamera>(Transformable(glm::vec3(-150.0f, 50.0f, 150.0f), glm::vec3(0.0f), glm::vec3(1.0f)), glm::vec3(0, 45, 0));
	camera->SetName("Camera");
	/*renderEngine->AddSkybox("media/skybox/right.jpg"
		, "media/skybox/left.jpg"
		, "media/skybox/top.jpg"
		, "media/skybox/bottom.jpg"
		, "media/skybox/front.jpg"
		, "media/skybox/back.jpg");*/

	// Field
	terrain.push_back(make_unique<EMesh>(Transformable(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(200.0f, 10.0f, 200.0f)), "media/Grass_Block.obj"));
	terrain[0]->SetName("Field");

	skeletons = geneticAlgorithm->GetPopulation();

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
	auto time = duration_cast<milliseconds>(std::chrono::system_clock::now() - timeStart).count() - timeToStart.count();

	imGuiManager->Begin("Entities transformables");
	physicsEngine->UpdateCamera(camera.get());
	ImGuiDebug();

	if (time >= std::chrono::milliseconds(0).count()) {
		for (const auto& skeleton : skeletons) {
			physicsEngine->UpdateSkeleton(skeleton.get());
		}

		for (const auto& mesh : terrain) {
			physicsEngine->UpdateEntity(mesh.get());
		}
	}

	geneticAlgorithm->Update(time);

	if (time >= Config::generationLifeSpan.count()) {
		timeStart = std::chrono::system_clock::now();
		geneticAlgorithm->NewGeneration();
	}
}

/// <summary>
/// StateExecution render.
/// </summary>
void StateExecution::Render() {
	renderEngine->BeginScene();

	imGuiManager->Begin("Debug");
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
	imGuiManager->End();

	renderEngine->DrawAll();
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