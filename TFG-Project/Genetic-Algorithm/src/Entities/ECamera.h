#pragma once

#include "Entity.h"

class Transformable; 

/// <summary>
/// Camera class
/// </summary>
class ECamera : public Entity{
	public:

		/// <summary>
		/// Enumerations for the possible camera types.
		/// </summary>
		enum CameraType { BESTGENE, BESTLASTGENE, FREE };

		/// <summary>
		/// ECamera constructor.
		/// </summary>
		/// <param name="target"> Camera target. </param>
		/// <param name="parent"> Parent. </param>
		ECamera(const glm::vec3 target, Entity* parent = nullptr);

		/// <summary>
		/// ECamera constructor
		/// </summary>
		/// <param name="transform"> Transformable. </param>
		/// <param name="target"> Camera target. </param>
		/// <param name="parentId"> Parent Id. </param>
		ECamera(const Transformable transform, const glm::vec3 target, Entity* parent = nullptr);

		/// <summary>
		/// ECamera destructor.
		/// </summary>
		~ECamera() override;

		/// <summary>
		/// Returns camera target.
		/// </summary>
		/// <returns> Camera target. </returns>
		const glm::vec3 GetTarget() { return target; }

		/// <summary>
		/// Returns the entity target adress.
		/// </summary>
		/// <returns> Entity target adress. </returns>
		glm::vec3* GetTargetPtr() { return &target; }

		/// <summary>
		/// Returns the camera type.
		/// </summary>
		/// <returns> Camera type. </returns>
		CameraType& GetCameraType() { return cameraType; }

		/// <summary>
		/// Sets camera target.
		/// </summary>
		/// <param name="target"> Camera target. </param>
		void SetTarget(const glm::vec3 target) { this->target = target; }

		/// <summary>
		/// Sets camera type.
		/// </summary>
		/// <param name="type"> Type. </param>
		void SetCameraType(CameraType type) { cameraType = type; }
		/// <summary>
		/// Camera type.
		/// </summary>
		CameraType cameraType{ CameraType::FREE };

	private:
		/// <summary>
		/// Target camera.
		/// </summary>
		glm::vec3 target{ 0.0f };

};
