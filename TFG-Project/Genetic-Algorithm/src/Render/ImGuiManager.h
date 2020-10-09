#pragma once

#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw.h>
#include <IMGUI/imgui_impl_opengl3.h>

#include <string>

struct GLFWwindow;
class Entity;

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

		/// <summary>
		/// Calls ImGui::Begin.
		/// </summary>
		/// <param name="title"> Window title. </param>
		void Begin(const std::string title);

		/// <summary>
		/// Calls ImGui::End.
		/// </summary>
		void End();

		/// <summary>
		/// Creates an entity transformable fields.
		/// </summary>
		/// <param name="entity"> Entity pointer. </param>
		/// <param name="name"> Entity name. </param>
		/// /// <param name="min"> Slider min value. </param>
		/// <param name="max"> Slider max value. </param>
		void EntityTransformable(Entity* entity, const std::string name, const int min, const int max);

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

