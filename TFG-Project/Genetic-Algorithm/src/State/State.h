#pragma once

/// <summary>
/// Interface for states implementation.
/// </summary>
class State{
	public:
		/// <summary>
		/// Enumaration for the states.
		/// </summary>
		enum States{EXECUTION, MENU};

		/// <summary>
		/// State constructor.
		/// </summary>
		State() = default;

		/// <summary>
		/// State destructor.
		/// </summary>
		~State() = default;

		/// <summary>
		/// State initialization.
		/// </summary>
		virtual void InitState() = 0;

		/// <summary>
		/// Called at the start of every frame.
		/// </summary>
		virtual void InitFrame() = 0;

		/// <summary>
		/// State update.
		/// </summary>
		virtual void Update() = 0;

		/// <summary>
		/// State render.
		/// </summary>
		virtual void Render() = 0;

};
