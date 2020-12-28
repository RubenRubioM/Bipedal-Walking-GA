#include "OBBCollider.h"

#include <GLM/glm.hpp>

#include <iostream>
#include <cmath>
#include <limits>       // std::numeric_limits

/// <summary>
/// OBBCollider constructor.
/// </summary>
/// <param name="center"> OBB center. </param>
/// <param name="vertexs"> OBB vertexs. </param>
/// <param name="size"> OBB size. </param>
/// <param name="axes"> OBB axes. </param>
OBBCollider::OBBCollider(const glm::vec3 center, const std::vector<glm::vec3> vertexs, const glm::vec3 size, const glm::mat3 axes){
	this->center = center;
	this->vertexs.reserve(8);
	this->vertexs = vertexs;
	this->size = size;
	this->axes = axes;
}

/// <summary>
/// OBBCollider Destructor.
/// </summary>
OBBCollider::~OBBCollider(){

}

/// <summary>
/// Translate the OBB vertexs and center.
/// </summary>
/// <param name="translation"> Translation. </param>
void OBBCollider::TranslateOBB(const glm::vec3 translation) {
	this->center += translation;
	for (uint16_t i = 0; i < 8; i++) {
		this->vertexs[i] += translation;
	}
}

/// <summary>
/// Returns if an OBBCollider is colliding.
/// </summary>
/// <param name="obb"> OBBCollider to check</param>
/// <returns> True if colliding, false if not. </returns>
bool OBBCollider::IsColliding(const OBBCollider* obb)  const {
	// <href="https://stackoverflow.com/questions/47866571/simple-oriented-bounding-box-obb-collision-detection-explaining">
	static glm::vec3 rpos;
	rpos = obb->GetCenter() - this->center;

	return !(GetSeparatingPlane(rpos, this->axes[0], obb) || 
		GetSeparatingPlane(rpos, this->axes[1], obb) ||
		GetSeparatingPlane(rpos, this->axes[2], obb) ||
		GetSeparatingPlane(rpos, obb->GetAxes()[0], obb) || 
		GetSeparatingPlane(rpos, obb->GetAxes()[1], obb) ||
		GetSeparatingPlane(rpos, obb->GetAxes()[2], obb) ||
		GetSeparatingPlane(rpos, glm::cross(this->axes[0], obb->GetAxes()[0]), obb) ||
		GetSeparatingPlane(rpos, glm::cross(this->axes[0], obb->GetAxes()[1]), obb) ||
		GetSeparatingPlane(rpos, glm::cross(this->axes[0], obb->GetAxes()[2]), obb) ||
		GetSeparatingPlane(rpos, glm::cross(this->axes[1], obb->GetAxes()[0]), obb) ||
		GetSeparatingPlane(rpos, glm::cross(this->axes[1], obb->GetAxes()[1]), obb) ||
		GetSeparatingPlane(rpos, glm::cross(this->axes[1], obb->GetAxes()[2]), obb) ||
		GetSeparatingPlane(rpos, glm::cross(this->axes[2], obb->GetAxes()[0]), obb) ||
		GetSeparatingPlane(rpos, glm::cross(this->axes[2], obb->GetAxes()[1]), obb) ||
		GetSeparatingPlane(rpos, glm::cross(this->axes[2], obb->GetAxes()[2]), obb));
}

bool OBBCollider::GetSeparatingPlane(const glm::vec3& rpos, const glm::vec3& plane, const OBBCollider* obb2) const{
	if (plane.x == 0 && plane.y == 0 && plane.z == 0)
		return false;


	float aMin = std::numeric_limits<float>::lowest();
	float aMax = std::numeric_limits<float>::max();
	float bMin = std::numeric_limits<float>::lowest();
	float bMax = std::numeric_limits<float>::max();

	for (int i = 0; i < 8; i++) {
		float aDist = glm::dot(this->vertexs[i], plane);
		aMin = (aDist < aMin) ? aDist : aMin;
		aMax = (aDist > aMax) ? aDist : aMax;

		float bDist = glm::dot(obb2->GetVertexs()[i], plane);
		bMin = (bDist < bMin) ? bDist : bMin;
		bMax = (bDist > bMax) ? bDist : bMax;
	}

	float longSpan = std::max(aMax, bMax) - std::min(aMin, bMin);
	float sumSpan = aMax - aMin + bMax - bMin;
	return longSpan < sumSpan; // Change this to <= if you want the case were they are touching but not overlapping, to count as an intersection

	/*
		// Define two intervals, a and b. Calculate their min and max values
		for( int i = 0; i < 8; i++ ) {
			float aDist = Vector3.Dot( aCorn[i], axis );
			aMin = ( aDist < aMin ) ? aDist : aMin;
			aMax = ( aDist > aMax ) ? aDist : aMax;
			float bDist = Vector3.Dot( bCorn[i], axis );
			bMin = ( bDist < bMin ) ? bDist : bMin;
			bMax = ( bDist > bMax ) ? bDist : bMax;
		}

    // One-dimensional intersection test between a and b
    float longSpan = Mathf.Max( aMax, bMax ) - Mathf.Min( aMin, bMin );
    float sumSpan = aMax - aMin + bMax - bMin;
    return longSpan < sumSpan; // Change this to <= if you want the case were they are touching but not overlapping, to count as an intersection
	*/


	/*auto one = std::fabs(glm::dot(rpos, plane));
	auto two = std::fabs(glm::dot(this->axes[0] * (this->size.x / 2), plane));
	auto three = std::fabs(glm::dot(this->axes[1] * (this->size.y / 2), plane));
	auto four = std::fabs(glm::dot(glm::vec3(this->axes[2]) * (this->size.z / 2), plane));
	auto five = std::fabs(glm::dot(obb2->GetAxes()[0] * (obb2->GetSize().x / 2), plane));
	auto six = std::fabs(glm::dot(obb2->GetAxes()[1] * (obb2->GetSize().y / 2), plane));
	auto seven = std::fabs(glm::dot(obb2->GetAxes()[2] * (obb2->GetSize().z / 2), plane));

	return one > (two + three + four + five + six + seven);*/
}


/// <summary>
/// Assign operator overload.
/// </summary>
/// <param name="in"> OBBCollider to assign. </param>
void OBBCollider::operator=(const OBBCollider& in) {
	center = in.GetCenter();
	vertexs = in.GetVertexs();
	size = in.GetSize();
}