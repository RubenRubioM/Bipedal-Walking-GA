#pragma once

#include "Entity.h"

#include <string>

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
		EMesh(const std::string meshPath, uint32_t parentId = 0);

		/// <summary>
		/// EMesh destructor.
		/// </summary>
		~EMesh();

		/// <summary>
		/// Returns meshPath.
		/// </summary>
		/// <returns> MeshPath. </returns>
		const std::string GetMeshPath() { return meshPath; }

	private:
		/// <summary>
		/// Mesh path.
		/// </summary>
		std::string meshPath;

};

