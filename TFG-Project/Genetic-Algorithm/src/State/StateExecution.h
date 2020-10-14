#pragma once

#include "State.h"

#include <memory>
#include <vector>

class RenderEngine;
class PhysicsEngine;
class ImGuiManager;
class ECamera;
class EMesh;
class Entity;

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
		/// Add all the entities to the render engine.
		/// </summary>
		void AddEntities();

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
		/// Scene main camera.
		/// </summary>
		std::unique_ptr<ECamera> camera{ nullptr };

		/// <summary>
		/// Vector for all the meshes.
		/// </summary>
		std::vector<std::unique_ptr<EMesh>> skeleton;

		/// <summary>
		/// Vector for all the meshes.
		/// </summary>
		std::vector<std::unique_ptr<EMesh>> terrain;

		/// <summary>
		/// Boolean to activate the bounding boxes render.
		/// </summary>
		bool showBoundingBoxes = true;

};
