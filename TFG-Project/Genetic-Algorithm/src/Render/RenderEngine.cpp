#include "RenderEngine.h"

#include <Entities/ECamera.h>
#include <Entities/Entity.h>
#include <Entities/EMesh.h>
#include <Render/ImGuiManager.h>
#include <DataTypes/OBBCollider.h>

#include <GLM/gtx/string_cast.hpp>

#include <cmath>

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
	delete instance;
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

	auto dimensions = node->CalculateBoundingBoxOBB();

	mesh->SetDimensions(glm::vec3(dimensions.x, dimensions.y, dimensions.z));
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
void RenderEngine::AddSkybox(const std::string right
	, const std::string left
	, const std::string top
	, const std::string bottom
	, const std::string front
	, const std::string back) const {
	device->AddSkybox(right, left, top, bottom, front, back);
}

/// <summary>
/// Draw mesh bounding box
/// </summary>
/// <param name="entity"> Entity to draw the bounding box. </param>
void RenderEngine::DrawBoundingBox(EMesh* entity) {
	/*
		   /3-------- 7
		  / |       / |
		 /  |      /  |
		1---------5   |
		|  /2- - -|- -6
		| /       |  /
		|/        | /
		0---------4
		a1  = 0->1
		a2  = 0->2
		a3  = 0->4
		a4  = 1->3
		a5  = 1->5
		a6  = 2->3
		a7  = 2->6
		a8  = 3->7
		a9  = 4->5
		a10 = 4->6
		a11 = 5->7
		a12 = 6->7
	*/
	const auto& vertexs = entity->GetCollider()->GetVertexs();
	const auto& center = entity->GetCollider()->GetCenter();
	device->SetDrawLineWidth(2);

	Draw3DLine(vertexs[0], vertexs[1]);
	Draw3DLine(vertexs[0], vertexs[2]);
	Draw3DLine(vertexs[0], vertexs[4]);
	Draw3DLine(vertexs[1], vertexs[3]);
	Draw3DLine(vertexs[1], vertexs[5]);
	Draw3DLine(vertexs[2], vertexs[3]);
	Draw3DLine(vertexs[2], vertexs[6]);
	Draw3DLine(vertexs[3], vertexs[7]);
	Draw3DLine(vertexs[4], vertexs[5]);
	Draw3DLine(vertexs[4], vertexs[6]);
	Draw3DLine(vertexs[5], vertexs[7]);
	Draw3DLine(vertexs[6], vertexs[7]);
	//Draw3DLine(center, glm::vec3(200, 10, -200), CLE::CLColor(0,200,0,255));
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

/// <summary>
/// Draw 3D line.
/// </summary>
/// <param name="pos1"> Initial position. </param>
/// <param name="pos2"> End position. </param>
void RenderEngine::Draw3DLine(const glm::vec3 pos1, const glm::vec3 pos2) const {
	Draw3DLine(pos1, pos2, 255.0, 0.0, 0.0, 255.0);
}

/// <summary>
/// Draw 3D line.
/// </summary>
/// <param name="pos1"> Initial position. </param>
/// <param name="pos2"> End position. </param>
/// <param name="r"> Red value. </param>
/// <param name="g"> Green value. </param>
/// <param name="b"> Blue value. </param>
/// <param name="a"> Alpha value. </param>
void RenderEngine::Draw3DLine(const glm::vec3 pos1, const glm::vec3 pos2, const uint16_t r, const uint16_t g, const uint16_t b, const uint16_t a) const {
	Draw3DLine(pos1, pos2, CLE::CLColor(r, g, b, a));
}

/// <summary>
/// Draw 3D line.
/// </summary>
/// <param name="pos1"> Initial position. </param>
/// <param name="pos2"> End position. </param>
/// <param name="color"> Color value. </param>
void RenderEngine::Draw3DLine(const glm::vec3 pos1, const glm::vec3 pos2, const CLE::CLColor color) const {
	device->Draw3DLine(pos1.x, pos1.y, pos1.z, pos2.x, pos2.y, pos2.z, color);
}

/// <summary>
/// Draw 3D line local.
/// </summary>
/// <param name="node"> Node of reference. </param>
/// <param name="pos1"> Initial position. </param>
/// <param name="pos2"> End position. </param>
void RenderEngine::Draw3DLineLocal(CLE::CLNode* node, const glm::vec3 pos1, const glm::vec3 pos2) const {
	Draw3DLineLocal(node, pos1, pos2, 255.0, 0.0, 0.0, 255.0);
}

/// <summary>
/// Draw 3D line local.
/// </summary>
/// <param name="node"> Node of reference. </param>
/// <param name="pos1"> Initial position. </param>
/// <param name="pos2"> End position. </param>
/// <param name="r"> Red value. </param>
/// <param name="g"> Green value. </param>
/// <param name="b"> Blue value. </param>
/// <param name="a"> Alpha value. </param>
void RenderEngine::Draw3DLineLocal(CLE::CLNode* node, const glm::vec3 pos1, const glm::vec3 pos2, const uint16_t r, const uint16_t g, const uint16_t b, const uint16_t a) const {
	Draw3DLineLocal(node, pos1, pos2, CLE::CLColor(r, g, b, a));
}

/// <summary>
/// Draw 3D line local.
/// </summary>
/// <param name="node"> Node of reference. </param>
/// <param name="pos1"> Initial position. </param>
/// <param name="pos2"> End position. </param>
/// <param name="color"> Color value. </param>
void RenderEngine::Draw3DLineLocal(CLE::CLNode* node, const glm::vec3 pos1, const glm::vec3 pos2, const CLE::CLColor color) const {
	device->Draw3DLineLocal(node, pos1.x, pos1.y, pos1.z, pos2.x, pos2.y, pos2.z, color);
}