#include "RenderEngine.h"

#include <Entities/ECamera.h>
#include <Entities/EMesh.h>
#include <Render/ImGuiManager.h>

/// <summary>
/// Creates or returns a RenderEngine instance.
/// </summary>
/// <returns> Static RenderEngine instance pointer. </returns>
RenderEngine* RenderEngine::GetInstance() {
	if (!instance) {
		instance = new RenderEngine();
	}
	return instance;
}

/// <summary>
/// Private constructor.
/// </summary>
RenderEngine::RenderEngine() {
	device = new CLE::CLEngine(1280, 720, "Genethic Algorithm");
	smgr = device->GetSceneManager();
	resourceManager = device->GetResourceManager();

	ImGuiManager::GetInstance()->SetContext(device->GetWindow());
}

/// <summary>
/// RenderEngine destructor.
/// </summary>
RenderEngine::~RenderEngine() {
}

/// <summary>
/// Add mesh to the scene.
/// </summary>
/// <param name="mesh"> Mesh. </param>
void RenderEngine::AddMesh(EMesh* mesh) {
	CLE::CLNode* node = nullptr;
	CLE::CLNode* father = smgr;

	if (mesh->GetParentId()) {
		father = device->GetNodeByID(mesh->GetParentId());
	}

	node = device->AddMesh(father, mesh->GetId());
	const auto& resourceMesh = resourceManager->GetResourceMesh(mesh->GetMeshPath(), true);
	static_cast<CLE::CLMesh*>(node->GetEntity())->SetMesh(resourceMesh);

	node->SetTranslation(mesh->GetPosition());
	node->SetRotation(mesh->GetRotation());
	node->SetScalation(mesh->GetScalation());

	mesh->SetDimensions(node->CalculateBoundingBoxOBB());

}

/// <summary>
/// Add camera to the scene
/// </summary>
/// <param name="camera"> Camera. </param>
void RenderEngine::AddCamera(ECamera* camera) {
	CLE::CLNode* node = nullptr;
	CLE::CLNode* father = smgr;

	if (camera->GetParentId()) {
		father = device->GetNodeByID(camera->GetParentId());
	}

	node = device->AddCamera(father, camera->GetId());
	node->SetTranslation(camera->GetPosition());
	node->SetRotation(camera->GetRotation());
	node->SetScalation(camera->GetScalation());
	static_cast<CLE::CLCamera*>(node->GetEntity())->SetCameraTarget(camera->GetTarget());
}

/// <summary>
/// Add a skybox to the scene
/// </summary>
/// <param name="right"> Right image path. </param>
/// <param name="left"> Left image path. </param>
/// <param name="top"> Top image path. </param>
/// <param name="bottom"> Bottom image path. </param>
/// <param name="front"> Front image path. </param>
/// <param name="back"> Back image path. </param>
void RenderEngine::AddSkybox(const std::string right, const std::string left, const std::string top, const std::string bottom, const std::string front, const std::string back) const {
	device->AddSkybox(right, left, top, bottom, front, back);
}

// <summary>
/// Returns true if the windows stills open.
/// </summary>
/// <returns> True if windows stills open. </returns>
bool RenderEngine::IsOpen() const {
	return device->Run();
}

/// <summary>
/// Close the window. 
/// </summary>
void RenderEngine::Terminate() const {
	device->CloseWindow();
}

/// <summary>
/// Clean the screen and buffers.
/// </summary>
void RenderEngine::BeginScene() const {
	device->UpdateViewport();
	device->BeginScene();
}

/// <summary>
/// Draw entity objects.
/// </summary>
void RenderEngine::DrawAll() const {
	device->DrawObjects();
}

/// <summary>
/// Swap buffers and release glfw events.
/// </summary>
void RenderEngine::EndScene() const {
	device->PollEvents();
	device->EndScene();
}
