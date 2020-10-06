#pragma once

#include "State.h"

#include <memory>

class RenderEngine;
class ECamera;

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
		/// Scene main camera.
		/// </summary>
		std::unique_ptr<ECamera> camera{ nullptr };
};

