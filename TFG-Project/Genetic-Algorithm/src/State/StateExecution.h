#pragma once

#include "State.h"

#include <memory>
#include <vector>
#include <chrono>

class RenderEngine;
class PhysicsEngine;
class ImGuiManager;
class ECamera;
class ESkeleton;
class EMesh;
class Entity;
class GeneticAlgorithm;

using namespace std::chrono_literals;

class StateExecution : public State {
	public:
		/// <summary>
		/// StateExecution constructor.
		/// </summary>
		StateExecution();

		/// <summary>
		/// StateExecution destructor.
		/// </summary>
		~StateExecution();

		/// <summary>
		/// StateExecution initialization.
		/// </summary>
		void InitState() override;

		/// <summary>
		/// Called at the start of every frame.
		/// </summary>
		void InitFrame() override;

		/// <summary>
		/// StateExecution update.
		/// </summary>
		void Update() override;

		/// <summary>
		/// StateExecution render.
		/// </summary>
		void Render() override;

	private:
		/// <summary>
		/// Add all the entities to the render and physics engine.
		/// </summary>
		void AddEntities();

		/// <summary>
		/// Refactor imGui debug.
		/// </summary>
		void ImGuiDebug();

		/// <summary>
		/// RenderEngine pointer.
		/// </summary>
		RenderEngine* renderEngine{ nullptr };

		/// <summary>
		/// PhysicsEngine pointer.
		/// </summary>
		PhysicsEngine* physicsEngine{ nullptr };

		/// <summary>
		/// ImGuiManager pointer. 
		/// </summary>
		ImGuiManager* imGuiManager{ nullptr };

		/// <summary>
		/// Genetic algorithm pointer.
		/// </summary>
		std::unique_ptr<GeneticAlgorithm> geneticAlgorithm{ nullptr };

		/// <summary>
		/// Scene main camera.
		/// </summary>
		std::unique_ptr<ECamera> camera{ nullptr };

		/// <summary>
		/// Vector for all the skeletons.
		/// </summary>
		std::vector<std::shared_ptr<ESkeleton>> skeletons;

		/// <summary>
		/// Vector for all the meshes.
		/// </summary>
		std::vector<std::unique_ptr<EMesh>> terrain;

		/// <summary>
		/// Boolean to activate the bounding boxes render.
		/// </summary>
		bool showBoundingBoxes = false;

		/// <summary>
		/// Time to start the execution.
		/// </summary>
		std::chrono::milliseconds timeToStart = 3 * 1000ms;

		/// <summary>
		/// Clock.
		/// </summary>
		std::chrono::system_clock::time_point timeStart = std::chrono::system_clock::now();
};
