#pragma once

#include <CLEngine.h>

class Entity;
class ESkeleton;
class EMesh;
class ImGuiManager;
class Transformable;
class OBBCollider;

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
		/// Set entity physics values.
		/// </summary>
		/// <param name="entity"> Entity. </param>
		void SetEntityValues(Entity* entity);

		/// <summary>
		/// Updates entity physics values.
		/// </summary>
		/// <param name="entity"> Entity. </param>
		void UpdateEntity(Entity* entity);

		/// <summary>
		/// Updates skeleton physics values.
		/// </summary>
		/// <param name="skeleton"> skeleton. </param>
		void UpdateSkeleton(ESkeleton* skeleton);

		/// <summary>
		/// Updates entity camera.
		/// </summary>
		/// <param name="camera"> Camera entity. </param>
		void UpdateCamera(Entity* camera);

		/// <summary>
		/// Updates camera with target.
		/// </summary>
		/// <param name="camera"> Camera entity. </param>
		/// <param name="skeleton"> Skeleton target. </param>
		void UpdateCamera(Entity* camera, ESkeleton* skeleton);

		/// <summary>
		/// Adds a colliding object to the physics engine
		/// </summary>
		/// <param name="entity"> Colliding object. </param>
		void AddCollidingMesh(EMesh* entity);

		//////////////////////////// DEBUG ////////////////////////////

		/// <summary>
		/// Returns a reference to gravityActivated variable.
		/// </summary>
		/// <returns></returns>
		bool& GetGravityActivated() { return gravityActivated; }

	private:
		/// <summary>
		/// Private constructor.
		/// </summary>
		PhysicsEngine();

		/// <summary>
		/// Apply gravity
		/// </summary>
		/// <param name="skeleton"> Skeleton to apply gravity. </param>
		void ApplyGravity(ESkeleton* skeleton) const;

		/// <summary>
		/// Apply the movement to the skeleton
		/// </summary>
		/// <param name="skeleton"> Skeleton to move. </param>
		void ApplySkeletonMovement(ESkeleton* skeleton) const;

		/// <summary>
		/// Fix position.
		/// </summary>
		/// <param name="skeleton"> Skeleton entity to fix. </param>
		/// <returns> If the position has been fixed. </return>
		bool FixPosition(ESkeleton* skeleton) const;

		/// <summary>
		/// Check if the skeleton has die.
		/// </summary>
		/// <param name="skeleton"> Skeleton to check. </param>
		/// <returns> If is dead or not. </returns>
		bool SkeletonDead(ESkeleton* skeleton) const;

		/// <summary>
		/// Calculate and assign OBB collider to the entity
		/// </summary>
		/// <param name="mesh"> Mesh. </param>
		/// <param name="node"> Node. </param>
		OBBCollider CalculateOBB(EMesh* mesh, CLE::CLNode* node);

		/// <summary>
		/// Constant gravity value.
		/// </summary>
		const glm::vec3 gravity{0.0f, -50.0f, 0.0f};

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

		/// <summary>
		/// Debug to toggle gravity.
		/// </summary>
		bool gravityActivated = true;
};

