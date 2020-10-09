#pragma once

#include <CLEngine.h>

class ECamera;
class EMesh;

/// <summary>
/// RenderEngine to use the CLEngine library.
/// </summary>
class RenderEngine{
	public:
		/// <summary>
		/// Creates or returns a RenderEngine instance.
		/// </summary>
		/// <returns> Static RenderEngine instance pointer. </returns>
		static RenderEngine* GetInstance();

		/// <summary>
		/// RenderEngine destructor.
		/// </summary>
		~RenderEngine();

		CLE::CLEngine* GetDevice() { return device; }

		/// <summary>
		/// Add mesh to the scene.
		/// </summary>
		/// <param name="mesh"> Mesh. </param>
		void AddMesh(EMesh* mesh);

		/// <summary>
		/// Add camera to the scene.
		/// </summary>
		/// <param name="camera"> Camera. </param>
		void AddCamera(ECamera* camera);

		/// <summary>
		/// Add a skybox to the scene
		/// </summary>
		/// <param name="right"> Right image path. </param>
		/// <param name="left"> Left image path. </param>
		/// <param name="top"> Top image path. </param>
		/// <param name="bottom"> Bottom image path. </param>
		/// <param name="front"> Front image path. </param>
		/// <param name="back"> Back image path. </param>
		void AddSkybox(const std::string right, const std::string left, const std::string top, const std::string bottom, const std::string front, const std::string back) const;

		/// <summary>
		/// Returns true if the windows stills open.
		/// </summary>
		/// <returns> True if windows stills open. </returns>
		bool IsOpen() const;

		/// <summary>
		/// Close the window. 
		/// </summary>
		void Terminate() const;

		/// <summary>
		/// Clean the screen and buffers.
		/// </summary>
		void BeginScene() const;

		/// <summary>
		/// Draw entity objects.
		/// </summary>
		void DrawAll() const;

		/// <summary>
		/// Swap buffers and release glfw events.
		/// </summary>
		void EndScene() const;
	
	private:
		/// <summary>
		/// Private constructor.
		/// </summary>
		RenderEngine();

		/// <summary>
		/// Static RenderEngine instance pointer for the singleton <see href="https://en.wikipedia.org/wiki/Singleton_pattern" />
		/// </summary>
		inline static RenderEngine* instance{ nullptr };

		/// <summary>
		/// CLEngine pointer.
		/// </summary>
		CLE::CLEngine* device{ nullptr };

		/// <summary>
		/// Scene manager.
		/// </summary>
		CLE::CLNode* smgr{ nullptr };

		/// <summary>
		/// Resource manager.
		/// </summary>
		CLE::CLResourceManager* resourceManager{ nullptr };
};

