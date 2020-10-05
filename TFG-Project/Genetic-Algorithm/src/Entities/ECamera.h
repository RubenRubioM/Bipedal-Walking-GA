#pragma once

#include "Entity.h"

class ECamera : public Entity{
	public:
		/// <summary>
		/// ECamera constructor.
		/// </summary>
		/// <param name="target"> Camera target. </param>
		/// <param name="parentId"> Parent Id.</param>
		ECamera(glm::vec3 target, uint32_t parentId = 0);

		/// <summary>
		/// ECamera destructor.
		/// </summary>
		~ECamera();

		/// <summary>
		/// Returns camera target.
		/// </summary>
		/// <returns> Camera target. </returns>
		const glm::vec3 GetTarget() { return target; }

		/// <summary>
		/// Sets camera target.
		/// </summary>
		/// <param name="target"> Camera target. </param>
		void SetTarget(const glm::vec3 target) { this->target = target; }

	private:
		/// <summary>
		/// Target camera.
		/// </summary>
		glm::vec3 target{ 0.0f };
};
