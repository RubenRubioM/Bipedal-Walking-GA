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
		/// <param name="parentId"> Parent Id.</param>
		EMesh(const std::string meshPath, const uint32_t parentId = 0);

		/// <summary>
		/// EMesh constructor.
		/// </summary>
		/// <param name="transform"> Transformable. </param>
		/// <param name="meshPath"> Mesh path. </param>
		/// <param name="parentId"> Parent Id. </param>
		EMesh(const Transformable transform, const std::string meshPath, const uint32_t parentId = 0);

		/// <summary>
		/// EMesh destructor.
		/// </summary>
		~EMesh();

		/// <summary>
		/// Returns mesh path.
		/// </summary>
		/// <returns> Mesh path. </returns>
		const std::string GetMeshPath() { return meshPath; }

	private:
		/// <summary>
		/// Mesh path.
		/// </summary>
		std::string meshPath;

};

