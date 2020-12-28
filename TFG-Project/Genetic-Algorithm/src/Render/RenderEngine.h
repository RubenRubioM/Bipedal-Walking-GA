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

		/// <summary>
		/// Returns the device.
		/// </summary>
		/// <returns> Device. </returns>
		CLE::CLEngine* GetDevice() { return device; }

		/// <summary>
		/// Returns window title
		/// </summary>
		/// <returns> Title. </returns>
		std::string GetTitle() { return title; }

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
		void AddSkybox(const std::string right
			, const std::string left
			, const std::string top
			, const std::string bottom
			, const std::string front
			, const std::string back) const;

		/// <summary>
		/// Draw mesh bounding box
		/// </summary>
		/// <param name="entity"> Entity to draw the bounding box. </param>
		void DrawBoundingBox(EMesh* entity);

		/// <summary>
		/// Draw distance lines.
		/// </summary>
		void DrawDistanceLines();

		/// <summary>
		/// Draw a line for the first skeleton.
		/// </summary>
		/// <param name="position"> Position. </param>
		void DrawFirstPlaceLine(glm::vec3 position);

		/// <summary>
		/// Returns true if the windows stills open.
		/// </summary>
		/// <returns> True if windows stills open. </returns>
		bool IsOpen() const;

		/// <summary>
		/// Returns time.
		/// </summary>
		/// <returns> Time. </returns>
		uint32_t GetTime() const;

		/// <summary>
		/// Set the title.
		/// </summary>
		/// <param name="title"> Window title. </param>
		void SetTitle(std::string& title);

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

		/// <summary>
		/// Sets entity visibility.
		/// </summary>
		/// <param name="entity"> Entity. </param>
		/// <param name="visiblity"> Visibility. </param>
		void SetVisible(EMesh* entity, bool visiblity);
	
	private:
		/// <summary>
		/// Private constructor.
		/// </summary>
		RenderEngine();

		/// <summary>
		/// Window title.
		/// </summary>
		std::string title = "Genethic-Algorithm";

		/// <summary>
		/// Draw 3D line.
		/// </summary>
		/// <param name="pos1"> Initial position. </param>
		/// <param name="pos2"> End position. </param>
		void Draw3DLine(const glm::vec3 pos1, const glm::vec3 pos2) const;

		/// <summary>
		/// Draw 3D line.
		/// </summary>
		/// <param name="pos1"> Initial position. </param>
		/// <param name="pos2"> End position. </param>
		/// <param name="r"> Red value. </param>
		/// <param name="g"> Green value. </param>
		/// <param name="b"> Blue value. </param>
		/// <param name="a"> Alpha value. </param>
		void Draw3DLine(const glm::vec3 pos1, const glm::vec3 pos2, const uint16_t r, const uint16_t g, const uint16_t b, const uint16_t a) const;

		/// <summary>
		/// Draw 3D line.
		/// </summary>
		/// <param name="pos1"> Initial position. </param>
		/// <param name="pos2"> End position. </param>
		/// <param name="color"> Color value. </param>
		void Draw3DLine(const glm::vec3 pos1, const glm::vec3 pos2,const CLE::CLColor color) const;

		/// <summary>
		/// Draw 3D line local.
		/// </summary>
		/// <param name="node"> Node of reference. </param>
		/// <param name="pos1"> Initial position. </param>
		/// <param name="pos2"> End position. </param>
		void Draw3DLineLocal(CLE::CLNode* node, const glm::vec3 pos1, const glm::vec3 pos2) const;

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
		void Draw3DLineLocal(CLE::CLNode* node, const glm::vec3 pos1, const glm::vec3 pos2, const uint16_t r, const uint16_t g, const uint16_t b, const uint16_t a) const;

		/// <summary>
		/// Draw 3D line local.
		/// </summary>
		/// <param name="node"> Node of reference. </param>
		/// <param name="pos1"> Initial position. </param>
		/// <param name="pos2"> End position. </param>
		/// <param name="color"> Color value. </param>
		void Draw3DLineLocal(CLE::CLNode* node, const glm::vec3 pos1, const glm::vec3 pos2, const CLE::CLColor color) const;

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

