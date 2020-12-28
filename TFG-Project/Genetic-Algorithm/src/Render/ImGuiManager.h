#pragma once

#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw.h>
#include <IMGUI/imgui_impl_opengl3.h>
#include <IMGUI/imgui_internal.h>
//#include <IMGUI/implot.h>
//#include <IMGUI/implot_internal.h>
#include <GLM/vec3.hpp>

#include <string>

struct GLFWwindow;
class Entity;

/// <summary>
/// Class to manage ImGui.
/// </summary>
class ImGuiManager {
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
		/// Creates a collapsable header.
		/// </summary>
		/// <param name="title"> Header title. </param>
		/// <returns> Bool. </returns>
		bool Header(const std::string title);

		/// <summary>
		/// Creates a tree node.
		/// </summary>
		/// <param name="title"> Title. </param>
		/// <returns> Bool. </returns>
		bool TreeNode(const std::string title);

		/// <summary>
		/// Pop the tree.
		/// </summary>
		void TreePop();

		/// <summary>
		/// Creates a slider for integer value.
		/// </summary>
		/// <param name="title"> Title. </param>
		/// <param name="value"> Variable. </param>
		/// <param name="min"> Min value. </param>
		/// <param name="max"> Max value. </param>
		void IntSlider(const std::string title, int* value, int min, int max);

		/// <summary>
		/// Creates a slider for float value.
		/// </summary>
		/// <param name="title"> Title. </param>
		/// <param name="value"> Variable. </param>
		/// <param name="min"> Min value. </param>
		/// <param name="max"> Max value. </param>
		void FloatSlider(const std::string title, float* value, float min, float max);

		/// <summary>
		/// Creates a vec3 slider.
		/// </summary>
		/// <param name="vec3"> Vec3 value. </param>
		/// <param name="name"> Field name. </param>
		/// <param name="min"> Min value. </param>
		/// <param name="max"> Max value. </param>
		void Vec3Slider(glm::vec3* vec3, const std::string name, const int min, const int max);

		/// <summary>
		/// Creates a radio button.
		/// </summary>
		/// <param name="title"> Title. </param>
		/// <param name="value"> Value. </param>
		/// <param name="idButton"> Button id. </param>
		void RadioButton(const std::string title, int* value, int idButton);

		/// <summary>
		/// Creates an entity transformable fields.
		/// </summary>
		/// <param name="entity"> Entity pointer. </param>
		/// <param name="name"> Entity name. </param>
		void EntityTransformable(Entity* entity, const std::string name);

		/// <summary>
		/// Creates a button.
		/// </summary>
		/// <param name="title"> Title. </param>
		/// <returns> If has been clicked. </returns>
		bool Button(const std::string title);

		/// <summary>
		/// Creates a checkbox.
		/// </summary>
		/// <param name="title"> Field name. </param>
		/// <param name="value"> Value. </param>
		void Checkbox(const std::string title, bool* value);

		/// <summary>
		/// Display text.
		/// </summary>
		/// <param name="text"> Text. </param>
		void Text(const std::string text);

		/// <summary>
		/// Display bullet text.
		/// </summary>
		/// <param name="text"> Text. </param>
		void BulletText(const std::string text);

		/// <summary>
		/// Creates a tab bar.
		/// </summary>
		/// <param name="title"> Title. </param>
		void BeginTabBar(const std::string title);

		/// <summary>
		/// Creates a tab.
		/// </summary>
		/// <param name="title"> Title. </param>
		bool AddTab(const std::string title);

		/// <summary>
		/// Ends tab bar.
		/// </summary>
		void EndTabBar();

		/// <summary>
		/// Ends tab.
		/// </summary>
		void EndTab();

		/// <summary>
		/// Adds a separator.
		/// </summary>
		void Separator();

		/// <summary>
		/// Display in the same line the next ImGui element.
		/// </summary>
		void SameLine();

		/// <summary>
		/// Returns if the last item is hovered.
		/// </summary>
		/// <returns> If the item is hovered. </returns>
		bool IsHovered();

		/// <summary>
		/// Begins a tooltip.
		/// </summary>
		void BeginTooltip();

		/// <summary>
		/// Sets a tooltip.
		/// </summary>
		/// <param name="title"> Text. </param>
		void SetTooltip(const std::string title);

		/// <summary>
		/// Ends a tooltip.
		/// </summary>
		void EndTooltip();

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
