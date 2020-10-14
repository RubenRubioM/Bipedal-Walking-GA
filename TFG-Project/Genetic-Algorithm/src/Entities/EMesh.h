#pragma once

#include "Entity.h"

#include <string>

class Transformable;

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
		~EMesh();

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
		/// Sets dimensions
		/// </summary>
		/// <param name="dimensions"> Entity dimensions. </param>
		void SetDimensions(const glm::vec3 dimensions) { this->dimensions = dimensions; }

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

};

