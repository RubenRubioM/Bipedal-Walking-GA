#include "ImGuiManager.h"

//#include <Render/RenderEngine.h>

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
/// Private constructor.
/// </summary>
ImGuiManager::ImGuiManager() {

}
