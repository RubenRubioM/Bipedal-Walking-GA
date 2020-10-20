#pragma once

#include <GLM/vec3.hpp>
#include <GLM/mat3x3.hpp>

#include <vector>

/*
		IMPORTANT: All pivots are in the bottom center of the mesh.
		We have to change every measure to LOCAL space. (EMesh dimensions are in WORLD space)

		^  +height
		|
		|   / -depth
		|  /
		| /
		|/------> +width

		   /3-------- 7
		  / |       / |
		 /  |      /  |
		1---------5   |
		|  /2- - -|- -6
		| /       |  /
		|/        | /
		0---------4
		a1  = 0->1
		a2  = 0->2
		a3  = 0->4
		a4  = 1->3
		a5  = 1->5
		a6  = 2->3
		a7  = 2->6
		a8  = 3->7
		a9  = 4->5
		a10 = 4->6
		a11 = 5->7
		a12 = 6->7
	*/

/// <summary>
/// Class to manage the OBB collider in the meshes
/// </summary>
class OBBCollider{
	public:
		/// <summary>
		/// OBBCollider constructor.
		/// </summary>
		/// <param name="center"> OBB center. </param>
		/// <param name="vertexs"> OBB vertexs. </param>
		/// <param name="size"> OBB size. </param>
		/// <param name="axes"> OBB axes. </param>
		OBBCollider(const glm::vec3 center, const std::vector<glm::vec3> vertexs, const glm::vec3 size, const glm::mat3 axes);

		/// <summary>
		/// OBBCollider Destructor.
		/// </summary>
		~OBBCollider();

		/// <summary>
		/// Returns if an OBBCollider is colliding.
		/// </summary>
		/// <param name="obb"> OBBCollider to check</param>
		/// <returns> True if colliding, false if not. </returns>
		bool IsColliding(const OBBCollider* obb) const;

		bool GetSeparatingPlane(const glm::vec3& rpos, const glm::vec3& plane, const OBBCollider* obb2) const;

		/// <summary>
		/// Assign operator overload.
		/// </summary>
		/// <param name="in"> OBBCollider to assign. </param>
		void operator=(const OBBCollider& in);

		/// <summary>
		/// Returns OBBCollider vertexs.
		/// </summary>
		/// <returns> OBBCollider vertexs. </returns>
		const std::vector<glm::vec3> GetVertexs() const { return vertexs; }

		/// <summary>
		/// Returns OBBCollider center.
		/// </summary>
		/// <returns> OBBCollider center. </returns>
		const glm::vec3 GetCenter() const { return center; }

		/// <summary>
		/// Returns OBBCollider size.
		/// </summary>
		/// <returns> OBBCollider size. </returns>
		const glm::vec3 GetSize() const { return size; }

		/// <summary>
		/// Returns OBBCollider axes.
		/// </summary>
		/// <returns> OBBCollider axes. </returns>
		const glm::mat3 GetAxes() const { return axes; }

		/// <summary>
		/// Sets OBBCollider vertexs. 
		/// </summary>
		/// <param name="vertexs"> OBBCollider vertexs. </param>
		void SetVertexs(const std::vector<glm::vec3> vertexs) { this->vertexs = vertexs; }

		/// <summary>
		/// Sets OBBCollider center.
		/// </summary>
		/// <param name="center"> OBBCollider center. </param>
		void SetCenter(const glm::vec3 center) { this->center = center; }

		/// <summary>
		/// Sets OBBCollider size.
		/// </summary>
		/// <param name="size"> OBBCollider size. </param>
		void SetSize(const glm::vec3 size) { this->size = size; }

		/// <summary>
		/// Sets OBBCollider axes.
		/// </summary>
		/// <param name="axes"> OBBCollider axes. </param>
		void SetAxes(const glm::mat3 axes) { this->axes = axes; }

	private:
		/// <summary>
		/// OBB collider vertexs positions.
		/// </summary>
		std::vector<glm::vec3> vertexs;

		/// <summary>
		/// OBB collider center.
		/// </summary>
		glm::vec3 center;

		/// <summary>
		/// Size.
		/// X = width.
		/// Y = height.
		/// Z = depth.
		/// </summary>
		glm::vec3 size{ 0.0f };

		/// <summary>
		/// OBB axes.
		/// </summary>
		glm::mat3 axes;
};

