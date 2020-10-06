#include "RenderEngine.h"

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
RenderEngine::RenderEngine(){
	device = new CLE::CLEngine(1280, 720, "Genethic Algorithm");
	smgr = device->GetSceneManager();
	resourceManager = device->GetResourceManager();
}

/// <summary>
/// RenderEngine destructor.
/// </summary>
RenderEngine::~RenderEngine(){
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
