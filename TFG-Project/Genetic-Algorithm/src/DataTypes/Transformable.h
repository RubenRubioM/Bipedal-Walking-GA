#pragma once

#include <GLM/vec3.hpp>

#include <ostream>

/// <summary>
/// Class to storage position, rotation, scalation.
/// </summary>
class Transformable {
	public:
		/// <summary>
		/// Transformable constructor.
		/// </summary>
		Transformable() = default;

		/// <summary>
		/// Transformable constructor.
		/// </summary>
		/// <param name="position"> Position. </param>
		/// <param name="rotation"> Rotation. </param>
		/// <param name="scalation"> Scalation. </param>
		Transformable(glm::vec3 position, glm::vec3 rotation, glm::vec3 scalation);

		/// <summary>
		/// Transformable destructor.
		/// </summary>
		~Transformable() = default;

		/// <summary>
		/// Assign operator overload.
		/// </summary>
		/// <param name="in"> Transformable to assign. </param>
		void operator=(const Transformable& in);

		/// <summary>
		/// Output operator overload.
		/// </summary>
		/// <param name="out"> Output. </param>
		/// <param name="in"> Transformable to output. </param>
		/// <returns> Output. </returns>
		friend std::ostream& operator<<(std::ostream& out, const Transformable& in);

		/// <summary>
		/// Position.
		/// </summary>
		glm::vec3 position{ 0.0f };

		/// <summary>
		/// Rotation.
		/// </summary>
		glm::vec3 rotation{ 0.0f };

		/// <summary>
		/// Scalation.
		/// </summary>
		glm::vec3 scalation{ 1.0f };

	private:
};