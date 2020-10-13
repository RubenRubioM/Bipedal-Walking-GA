#pragma once

#include <CLEngine.h>

class Entity;
class EMesh;
class ImGuiManager;
class Transformable;

/// <summary>
/// Physics engine class.
/// </summary>
class PhysicsEngine{
	public:
		/// <summary>
		/// Creates or returns a PhysicsEngine instance.
		/// </summary>
		/// <returns> Static PhysicsEngine instance pointer. </returns>
		static PhysicsEngine* GetInstance();

		/// <summary>
		/// RenderEngine destructor.
		/// </summary>
		~PhysicsEngine();

		/// <summary>
		/// Updates entity physics values.
		/// </summary>
		/// <param name="entity"> Entity. </param>
		void UpdateEntity(Entity* entity);

		/// <summary>
		/// Updates entities physics values.
		/// </summary>
		/// <param name="entities"> Entities. </param>
		void UpdateEntities(std::vector<std::unique_ptr<EMesh>> entities);

		/// <summary>
		/// Updates entity camera.
		/// </summary>
		/// <param name="camera"> Camera entity. </param>
		/// <param name="target"> Target position. </param>
		void UpdateCamera(Entity* camera, glm::vec3 target);

	private:
		/// <summary>
		/// Private constructor.
		/// </summary>
		PhysicsEngine();

		/// <summary>
		/// Static PhysicsEngine instance pointer for the singleton <see href="https://en.wikipedia.org/wiki/Singleton_pattern" />
		/// </summary>
		inline static PhysicsEngine* instance{ nullptr };

		/// <summary>
		/// CLEngine pointer.
		/// </summary>
		CLE::CLEngine* device{ nullptr };

		/// <summary>
		/// Scene manager.
		/// </summary>
		CLE::CLNode* smgr{ nullptr };

		/// <summary>
		/// ImGuiManager pointer. 
		/// </summary>
		ImGuiManager* imGuiManager{ nullptr };
};

