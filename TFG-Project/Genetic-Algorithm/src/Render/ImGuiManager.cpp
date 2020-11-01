#include "ImGuiManager.h"

#include <Entities/Entity.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/constants.hpp>


/// <summary>
/// Creates or returns a ImGuiManager instance.
/// </summary>
/// <returns> Static ImGuiManager instance pointer. </returns>
ImGuiManager* ImGuiManager::GetInstance() {
	if (!instance) {
		instance = new ImGuiManager();
	}
	return instance;
}

/// <summary>
/// ImGuiManager destructor.
/// </summary>
ImGuiManager::~ImGuiManager() {
}

/// <summary>
/// Sets the window context for ImGui.
/// </summary>
/// <param name="window"> CLE::CLEngine's glfw window. </param>
void ImGuiManager::SetContext(GLFWwindow* window) {
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 450");
	ImGui::StyleColorsDark();
}

/// <summary>
/// Call at the begining of every frame.
/// </summary>
void ImGuiManager::FrameInit() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

/// <summary>
/// Render all the ImGui stuff.
/// </summary>
void ImGuiManager::Render() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/// <summary>
/// Calls ImGui::Begin.
/// </summary>
/// <param name="title"> Window title. </param>		
void ImGuiManager::Begin(const std::string title) {
	ImGui::Begin(title.c_str());
}

/// <summary>
/// Calls ImGui::End.
/// </summary>
void ImGuiManager::End() {
	ImGui::End();
}

/// <summary>
/// Creates a collapsable header.
/// </summary>
/// <param name="title"> Header title. </param>
/// <returns> Bool. </returns>
bool ImGuiManager::Header(const std::string title) {
	return ImGui::CollapsingHeader(title.c_str(), ImGuiTabBarFlags_None);
}

/// <summary>
/// Creates a tree node.
/// </summary>
/// <param name="title"> Title. </param>
/// <returns> Bool. </returns>
bool ImGuiManager::TreeNode(const std::string title) {
	return ImGui::TreeNode(title.c_str());
}

/// <summary>
/// Pop the tree.
/// </summary>
void ImGuiManager::TreePop() {
	ImGui::TreePop();
}

/// <summary>
/// Creates a vec3 slider.
/// </summary>
/// <param name="vec3"> Vec3 value. </param>
/// <param name="name"> Field name. </param>
/// <param name="min"> Min value. </param>
/// <param name="max"> Max value. </param>
void ImGuiManager::Vec3Slider(glm::vec3* vec3, const std::string name, const int min, const int max) {
	ImGui::SliderFloat3(name.c_str(), reinterpret_cast<float*>(vec3), min, max);
}

/// <summary>
/// Creates an entity transformable fields.
/// </summary>
/// <param name="entity"> Entity pointer. </param>
/// <param name="name"> Entity name. </param>
void ImGuiManager::EntityTransformable(Entity* entity, std::string name) {
	Vec3Slider(entity->GetPositionPtr(), std::string(name + " position"), entity->GetPositionBoundaries().first, entity->GetPositionBoundaries().second);
	Vec3Slider(entity->GetRotationPtr(), std::string(name + " rotation"), entity->GetRotationBoundaries().first, entity->GetRotationBoundaries().second);
	Vec3Slider(entity->GetScalationPtr(), std::string(name + " scalation"), entity->GetScalationBoundaries().first, entity->GetScalationBoundaries().second);
}

/// <summary>
/// Creates a checkbox.
/// </summary>
/// <param name="title"> Field name. </param>
/// <param name="value"> Value. </param>
void ImGuiManager::Checkbox(const std::string title, bool* value) {
	ImGui::Checkbox(title.c_str(), value);
}

/// <summary>
/// Display text.
/// </summary>
/// <param name="text"> Text. </param>
void ImGuiManager::Text(const std::string text) {
	ImGui::Text(text.c_str());
}

/// <summary>
/// Display bullet text.
/// </summary>
/// <param name="text"> Text. </param>
void ImGuiManager::BulletText(const std::string text) {
	ImGui::BulletText(text.c_str());
}

/// <summary>
/// Creates a tab bar.
/// </summary>
/// <param name="title"> Title. </param>
void ImGuiManager::BeginTabBar(const std::string title) {
	ImGui::BeginTabBar(title.c_str());
}

/// <summary>
/// Creates a tab.
/// </summary>
/// <param name="title"> Title. </param>
bool ImGuiManager::AddTab(const std::string title) {
	return ImGui::BeginTabItem(title.c_str());
}

/// <summary>
/// Ends tab bar.
/// </summary>
void ImGuiManager::EndTabBar() {
	ImGui::EndTabBar();
}

/// <summary>
/// Ends tab.
/// </summary>
void ImGuiManager::EndTab() {
	ImGui::EndTabItem();
}

/// <summary>
/// Private constructor.
/// </summary>
ImGuiManager::ImGuiManager() {

}
