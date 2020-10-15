#pragma once

#include <GLM/vec3.hpp>
#include <GLM/gtc/constants.hpp>

#include <Utils/Shaders.h>
#include <DataTypes/Transformable.h>

#include <utility>

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
		/// <param name="parent"> Parent. </param>
		Entity(Entity* parent);

		/// <summary>
		/// Entity constructor with parent.
		/// </summary>
		/// <param name="transform"> Transformable </param>
		/// <param name="parent"> Parent. </param>
		Entity(const Transformable transform, Entity* parent);

		/// <summary>
		/// Entity destructor.
		/// </summary>
		virtual ~Entity() = default;

		/// <summary>
		/// Returns the entity Id.
		/// </summary>
		/// <returns> Entity identifier. </returns>
		const uint32_t GetId() const { return id; }

		/// <summary>
		/// Returns parent pointer.
		/// </summary>
		/// <returns> Parent poiner. </returns>
		const Entity* GetParent() const { return parent; }

		/// <summary>
		/// Returns the entity parent id.
		/// </summary>
		/// <returns> Parent identifier. </returns>
		const uint32_t GetParentId() const { return parent ? parent->GetId() : 0; }

		/// <summary>
		/// Returns the entity transformable.
		/// </summary>
		/// <returns> Entity transformable. </returns>
		const Transformable GetTransformable() const { return transformable; }

		/// <summary>
		/// Returns the entity position.
		/// </summary>
		/// <returns> Entity position. </returns>
		const glm::vec3 GetPosition() const { return position; }

		/// <summary>
		/// Returns the entity position adress.
		/// </summary>
		/// <returns> Entity position adress. </returns>
		glm::vec3* GetPositionPtr() { return &position; }

		/// <summary>
		/// Returns the entity rotation.
		/// </summary>
		/// <returns> Entity rotation. </returns>
		const glm::vec3 GetRotation() const { return rotation; }

		/// <summary>
		/// Returns the entity rotation adress.
		/// </summary>
		/// <returns> Entity rotation adress. </returns>
		glm::vec3* GetRotationPtr() { return &rotation; }

		/// <summary>
		/// Returns the entity scalation.
		/// </summary>
		/// <returns> Entity scalation. </returns>
		const glm::vec3 GetScalation() const { return scalation; }

		/// <summary>
		/// Returns the entity scalation adress.
		/// </summary>
		/// <returns> Entity scalation adress. </returns>
		glm::vec3* GetScalationPtr() { return &scalation; }

		/// <summary>
		/// Returns the entity name.
		/// </summary>
		/// <returns> Entity name. </returns>
		const std::string GetName() const { return name; }

		/// <summary>
		/// Returns the entity position boundaries.
		/// </summary>
		/// <returns> Entity position boundaries. </returns>
		const std::pair<float, float> GetPositionBoundaries() const { return positionBoundaries; } 

		/// <summary>
		/// Returns the entity rotation boundaries.
		/// </summary>
		/// <returns> Entity rotation boundaries. </returns>
		const std::pair<float, float> GetRotationBoundaries() const { return rotationBoundaries; }

		/// <summary>
		/// Returns the entity scalation boundaries.
		/// </summary>
		/// <returns> Entity scalation boundaries. </returns>
		const std::pair<float, float> GetScalationBoundaries() const { return scalationBoundaries; }

		/// <summary>
		/// Returns the entity shader.
		/// </summary>
		/// <returns> Entity shader. </returns>
		const Shader GetShader() const { return shader; }

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
		/// Sets the entity name.
		/// </summary>
		/// <param name="name"> Entity name. </param>
		void SetName(const std::string name) { this->name = name; }

		/// <summary>
		/// Sets the entity position boundaries
		/// </summary>
		/// <param name="boundaries"> Position boundaries. </param>
		void SetPositionBoundaries(const std::pair<float, float> boundaries) { positionBoundaries = boundaries; }

		/// <summary>
		/// Sets the entity rotation boundaries
		/// </summary>
		/// <param name="boundaries"> Rotation boundaries. </param>
		void SetRotationBoundaries(const std::pair<float, float> boundaries) { rotationBoundaries = boundaries; }

		/// <summary>
		/// Sets the entity scalation boundaries
		/// </summary>
		/// <param name="boundaries"> Scalation boundaries. </param>
		void SetScalationBoundaries(const std::pair<float, float> boundaries) { scalationBoundaries = boundaries; }

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
		/// Parent.
		/// </summary>
		Entity* parent{ nullptr };

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
		/// Entity name.
		/// </summary>
		std::string name{ "Entity" };

		/// <summary>
		/// Entity position boundaries
		/// </summary>
		std::pair<float, float> positionBoundaries{ -500.0f, 500.0f };

		/// <summary>
		/// Entity rotation boundaries
		/// </summary>
		std::pair<float, float> rotationBoundaries{ -180.0f, 180.0f };

		/// <summary>
		/// Entity scalation boundaries
		/// </summary>
		std::pair<float, float> scalationBoundaries{ -500.0f, 500.0f };

		/// <summary>
		/// Entity shader.
		/// </summary>
		Shader shader = Shaders::BasicShader;

	private:
		/// <summary>
		/// Entity next Id.
		/// </summary>
		inline static uint32_t nextId{ 1 };
};
