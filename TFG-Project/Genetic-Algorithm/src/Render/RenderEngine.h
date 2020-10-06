#pragma once

#include <CLEngine.h>

class ECamera;

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

		/// <summary>
		/// Add camera to the scene
		/// </summary>
		/// <param name="camera"> Camera. </param>
		void AddCamera(ECamera* camera);

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

