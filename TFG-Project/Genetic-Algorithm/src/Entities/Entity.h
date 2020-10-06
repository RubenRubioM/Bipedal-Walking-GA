#pragma once

#include <GLM/vec3.hpp>

#include <Utils/Shaders.h>
#include <DataTypes/Transformable.h>

/// <summary>
/// Generic entity class.
/// </summary>
class Entity{
	public:
		/// <summary>
		/// Entity constructor.
		/// </summary>
		Entity();

		/// <summary>
		/// Entity constructor with parent.
		/// </summary>
		/// <param name="parentId"> Parent Id. </param>
		Entity(const uint32_t parentId);

		/// <summary>
		/// Entity constructor with parent.
		/// </summary>
		/// <param name="parentId"> Parent Id. </param>
		/// <param name="transform"> Transformable </param>
		Entity(const Transformable transform, const uint32_t parentId);

		/// <summary>
		/// Entity destructor.
		/// </summary>
		~Entity();

		/// <summary>
		/// Returns the entity Id.
		/// </summary>
		/// <returns> Entity identifier. </returns>
		const uint32_t GetId() { return id; }

		/// <summary>
		/// Returns the entity parent id.
		/// </summary>
		/// <returns> Parent identifier. </returns>
		const uint32_t GetParentId() { return parentId; }

		/// <summary>
		/// Returns the entity transformable.
		/// </summary>
		/// <returns> Entity transformable. </returns>
		const Transformable GetTransformable() { return transformable; }

		/// <summary>
		/// Returns the entity position.
		/// </summary>
		/// <returns> Entity position. </returns>
		const glm::vec3 GetPosition() { return position; }

		/// <summary>
		/// Returns the entity rotation.
		/// </summary>
		/// <returns> Entity rotation. </returns>
		const glm::vec3 GetRotation() { return rotation; }

		/// <summary>
		/// Returns the entity scalation.
		/// </summary>
		/// <returns> Entity scalation. </returns>
		const glm::vec3 GetScalation() { return scalation; }

		/// <summary>
		/// Returns the entity shader.
		/// </summary>
		/// <returns> Entity shader. </returns>
		const Shader GetShader() { return shader; }

		/// <summary>
		/// Sets the entity transformable.
		/// </summary>
		/// <param name="transformable"> Entity transformable. </param>
		void SetTransformable(const Transformable transformable);

		/// <summary>
		/// Sets the entity position.
		/// </summary>
		/// <param name="position"> Entity position. </param>
		void SetPosition(glm::vec3 position) { this->position = position; }

		/// <summary>
		/// Sets the entity rotation.
		/// </summary>
		/// <param name="rotation"> Entity rotation. </param>
		void SetRotation(glm::vec3 rotation)  { this->rotation = rotation; }

		/// <summary>
		/// Sets the entity scalation.
		/// </summary>
		/// <param name="scalation"> Entity scalation. </param>
		void SetScalation(glm::vec3 scalation) { this->scalation = scalation; }

		/// <summary>
		/// Sets the entity shader.
		/// </summary>
		/// <param name="shader"> Entity shader. </param>
		void SetShader(Shader shader) { this->shader = shader; }

	protected:
		/// <summary>
		/// Entity Id.
		/// </summary>
		uint32_t id{ 0 };

		/// <summary>
		/// Parent Id.
		/// </summary>
		uint32_t parentId{ 0 };

		/// <summary>
		/// Entity transformable.
		/// </summary>
		Transformable transformable;

		/// <summary>
		/// Entity position.
		/// </summary>
		glm::vec3 position{ 0.0f };

		/// <summary>
		/// Entity rotation.
		/// </summary>
		glm::vec3 rotation{ 0.0f };

		/// <summary>
		/// Entity scalation.
		/// </summary>
		glm::vec3 scalation{ 1.0f };

		/// <summary>
		/// Entity shader.
		/// </summary>
		Shader shader = Shaders::BasicShader;

	private:
		/// <summary>
		/// Entity next Id.
		/// </summary>
		inline static uint32_t nextId{ 0 };
};
