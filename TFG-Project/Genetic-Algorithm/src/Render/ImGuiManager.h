#pragma once

#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw.h>
#include <IMGUI/imgui_impl_opengl3.h>

struct GLFWwindow;

/// <summary>
/// Class to manage ImGui.
/// </summary>
class ImGuiManager{
	public:
		/// <summary>
		/// Creates or returns a Program instance.
		/// </summary>
		/// <returns> Static Program instance pointer. </returns>
		static ImGuiManager* GetInstance();

		/// <summary>
		/// ImGuiManager destructor.
		/// </summary>
		~ImGuiManager();

		/// <summary>
		/// Sets the window context for ImGui.
		/// </summary>
		/// <param name="window"> CLE::CLEngine's glfw window. </param>
		void SetContext(GLFWwindow* window);

		/// <summary>
		/// Call at the begining of every frame.
		/// </summary>
		void FrameInit();

		/// <summary>
		/// Render all the ImGui stuff.
		/// </summary>
		void Render();

	private:
		/// <summary>
		/// Private constructor
		/// </summary>
		ImGuiManager();

		/// <summary>
		/// Static ImGuiManager instance pointer for the singleton <see href="https://en.wikipedia.org/wiki/Singleton_pattern" />
		/// </summary>
		inline static ImGuiManager* instance{ nullptr };
};

