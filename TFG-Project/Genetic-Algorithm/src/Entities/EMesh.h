#pragma once

#include "Entity.h"

#include <string>

class Transformable;
class OBBCollider;

/// <summary>
/// Mesh class.
/// </summary>
class EMesh : public Entity {
	public:
		/// <summary>
		/// EMesh constructor.
		/// </summary>
		/// <param name="meshPath"> Mesh path.</param>
		/// <param name="parent"> Parent.</param>
		EMesh(const std::string meshPath, Entity* parent = nullptr);

		/// <summary>
		/// EMesh constructor.
		/// </summary>
		/// <param name="transform"> Transformable. </param>
		/// <param name="meshPath"> Mesh path. </param>
		/// <param name="parent"> Parent. </param>
		EMesh(const Transformable transform, const std::string meshPath, Entity* parent = nullptr);

		/// <summary>
		/// EMesh destructor.
		/// </summary>
		~EMesh() override;

		/// <summary>
		/// Returns mesh path.
		/// </summary>
		/// <returns> Mesh path. </returns>
		const std::string GetMeshPath() { return meshPath; }

		/// <summary>
		/// Returns mesh dimensions.
		/// </summary>
		/// <returns> Mesh dimensions. </returns>
		const glm::vec3 GetDimensions() { return dimensions; }

		/// <summary>
		/// Returns OBBCollider.
		/// </summary>
		/// <returns> OBBCollider. </returns>
		const OBBCollider* GetCollider() { return collider.get(); }

		/// <summary>
		/// Sets dimensions
		/// </summary>
		/// <param name="dimensions"> Entity dimensions. </param>
		void SetDimensions(const glm::vec3 dimensions) { this->dimensions = dimensions; }

		/// <summary>
		/// Sets OBBCollider.
		/// </summary>
		/// <param name="collider"> Entity collider. </param>
		void SetCollider(OBBCollider collider);

	private:
		/// <summary>
		/// Mesh path.
		/// </summary>
		std::string meshPath;

		/// <summary>
		/// Mesh dimensions.
		/// X = width.
		/// Y = height.
		/// Z = depth.
		/// </summary>
		glm::vec3 dimensions{ 0.0f };

		/// <summary>
		/// OBBCollider pointer.
		/// </summary>
		std::unique_ptr<OBBCollider> collider{ nullptr };
};

