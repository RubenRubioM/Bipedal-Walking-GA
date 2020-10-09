#include "ImGuiManager.h"

#include <Entities/Entity.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/constants.hpp>


/// <summary>
/// Creates or returns a ImGuiManager instance.
/// </summary>
/// <returns> Static ImGuiManager instance pointer. </returns>
ImGuiManager* ImGuiManager::GetInstance(){
	if (!instance) {
		instance = new ImGuiManager();
	}
	return instance;
}

/// <summary>
/// ImGuiManager destructor.
/// </summary>
ImGuiManager::~ImGuiManager(){
}

/// <summary>
/// Sets the window context for ImGui.
/// </summary>
/// <param name="window"> CLE::CLEngine's glfw window. </param>
void ImGuiManager::SetContext(GLFWwindow* window) {
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

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
void ImGuiManager::Begin(const std::string title){
	ImGui::Begin(title.c_str());
}

/// <summary>
/// Calls ImGui::End.
/// </summary>
void ImGuiManager::End(){
	ImGui::End();
}

/// <summary>
/// Creates an entity transformable fields.
/// </summary>
/// <param name="entity"> Entity pointer. </param>
/// <param name="name"> Entity name. </param>
/// <param name="min"> Slider min value. </param>
/// <param name="max"> Slider max value. </param>
void ImGuiManager::EntityTransformable(Entity* entity, std::string name, const int min, const int max){
	if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_None)) {
		ImGui::SliderFloat3(std::string(name + " position").c_str(), reinterpret_cast<float*>(entity->GetPositionPtr()), min, max);
		ImGui::SliderFloat3(std::string(name + " rotation").c_str(), reinterpret_cast<float*>(entity->GetRotationPtr()), glm::degrees(-glm::pi<float>()), glm::degrees(glm::pi<float>()));
		ImGui::SliderFloat3(std::string(name + " scalation").c_str(), reinterpret_cast<float*>(entity->GetScalationPtr()), min, max);
	}
}

/// <summary>
/// Private constructor.
/// </summary>
ImGuiManager::ImGuiManager() {

}
