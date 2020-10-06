#include "Transformable.h"

/// <summary>
/// Transformable constructor.
/// </summary>
/// <param name="position"> Position. </param>
/// <param name="rotation"> Rotation. </param>
/// <param name="scalation"> Scalation. </param>
Transformable::Transformable(glm::vec3 position, glm::vec3 rotation, glm::vec3 scalation) : 
	position(position), 
	rotation(rotation), 
	scalation(scalation) 
{
}

/// <summary>
/// Assign operator overload.
/// </summary>
/// <param name="in"> Transformable to assign. </param>
void Transformable::operator=(const Transformable& in){
	position = in.position;
	rotation = in.rotation;
	scalation = in.scalation;
}

/// <summary>
/// Output operator overload.
/// </summary>
/// <param name="out"> Output. </param>
/// <param name="in"> Transformable to output. </param>
/// <returns> Output. </returns>
std::ostream& operator<<(std::ostream& out, const Transformable& in){
	out << "[" << std::endl;
	out << "  (" << in.position.x << ", " << in.position.y << ", " << in.position.z << ")" << std::endl;
	out << "  (" << in.rotation.x << ", " << in.rotation.y << ", " << in.rotation.z << ")" << std::endl;
	out << "  (" << in.scalation.x << ", " << in.scalation.y << ", " << in.scalation.z << ")" << std::endl;
	out << "]" << std::endl;
	return out;
}
