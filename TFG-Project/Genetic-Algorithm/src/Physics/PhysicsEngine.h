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
		/// Check if a mesh is colliding with any collidingMeshes.
		/// </summary>
		/// <param name="Mesh"> Mesh. </param>
		void CheckCollision(EMesh* mesh);

		/// <summary>
		/// Updates entity camera.
		/// </summary>
		/// <param name="camera"> Camera entity. </param>
		/// <param name="target"> Target position. </param>
		void UpdateCamera(Entity* camera, glm::vec3 target);

		/// <summary>
		/// Adds a colliding object to the physics engine
		/// </summary>
		/// <param name="entity"> Colliding object. </param>
		void AddCollidingMesh(EMesh* entity) { collidingMeshes.push_back(entity); }

	private:
		/// <summary>
		/// Private constructor.
		/// </summary>
		PhysicsEngine();

		/// <summary>
		/// Calculate and assign OBB collider to the entity
		/// </summary>
		/// <param name="mesh"> Mesh. </param>
		/// <param name="node"> Node. </param>
		void CalculateOBB(EMesh* mesh, CLE::CLNode* node);

		/// <summary>
		/// All the colliding meshes.
		/// </summary>
		std::vector<EMesh*> collidingMeshes;

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

