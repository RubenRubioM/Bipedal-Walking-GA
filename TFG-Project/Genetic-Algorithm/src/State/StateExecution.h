#pragma once

#include "State.h"

class RenderEngine;

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
		/// RenderEngine pointer.
		/// </summary>
		RenderEngine* renderEngine{ nullptr };
};

