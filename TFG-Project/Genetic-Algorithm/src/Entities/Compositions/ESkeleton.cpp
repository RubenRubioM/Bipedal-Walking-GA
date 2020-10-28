#include "ESkeleton.h"

#include <Entities/EMesh.h>

#include <iostream>

/// <summary>
/// ESkeleton constructor.
/// </summary>
/// <param name="core"> Skeleton core. </param>
/// <param name="hip1"> Skeleton leg1Joints[0]. </param>
/// <param name="knee1">Skeleton leg1Joints[1]. </param>
/// <param name="hip2"> Skeleton leg1Joints[0]. </param>
/// <param name="knee2"> Skeleton leg1Joints[1]. </param>
/// <param name="shoulder1"> Skeleton arm1Joints[0]. </param>
/// <param name="elbow1"> Skeleton arm1Joints[1]. </param>
/// <param name="shoulder2"> Skeleton arm2Joints[0]. </param>
/// <param name="elbow2"> Skeleton arm2Joints[1]. </param>
ESkeleton::ESkeleton(EMesh* core, EMesh* hip1, EMesh* knee1, EMesh* hip2, EMesh* knee2, EMesh* shoulder1, EMesh* elbow1, EMesh* shoulder2, EMesh* elbow2){
	this->core = core; numOfJoints++;
	this->leg1Joints[0] = hip1; numOfJoints++;
	this->leg1Joints[1] = knee1; numOfJoints++;
	this->leg2Joints[0] = hip2; numOfJoints++;
	this->leg2Joints[1] = knee2; numOfJoints++;
	this->arm1Joints[0] = shoulder1; numOfJoints;
	this->arm1Joints[1] = elbow1; numOfJoints;
	this->arm2Joints[0] = shoulder2; numOfJoints;
	this->arm2Joints[1] = elbow2; numOfJoints;

	SetFlexibility(Flexibility::MEDIUM);

	skeletonId = ++skeletonNextId;
}

/// <summary>
/// ESkeleton destructor.
/// </summary>
ESkeleton::~ESkeleton(){
	delete core;
	delete leg1Joints[0];
	delete leg1Joints[1];
	delete leg2Joints[0];
	delete leg2Joints[1];
	delete arm1Joints[0];
	delete arm1Joints[1];
	delete arm2Joints[0];
	delete arm2Joints[1];
}

/// <summary>
/// Returns all the skeleton parts.
/// [0] = core
/// [1] [2] = leg1
/// [3] [4] = leg2
/// [5] [6] = arm1
/// [7] [8] = arm2
/// </summary>
/// <returns> All the skeleton parts. </returns>
std::vector<EMesh*> ESkeleton::GetSkeleton() {
	return std::vector<EMesh*> {core, leg1Joints[0], leg1Joints[1], leg2Joints[0], leg2Joints[1], arm1Joints[0], arm1Joints[1], arm2Joints[0], arm2Joints[1]};
}

/// <summary>
/// Sets the skeleton flexibility.
/// </summary>
/// <param name="flexibility"> Flexibility. </param>
void ESkeleton::SetFlexibility(const Flexibility flexibility) {
	this->flexibility = flexibility;

	switch (flexibility) {
		case Flexibility::LOW: {
			leg1Joints[0]->SetRotationBoundaries(std::pair<float, float>(-40.0f, 40.0f));
			leg2Joints[0]->SetRotationBoundaries(std::pair<float, float>(-40.0f, 40.0f));
			leg1Joints[1]->SetRotationBoundaries(std::pair<float, float>(-50.0f, 0.0f));
			leg2Joints[1]->SetRotationBoundaries(std::pair<float, float>(-50.0f, 0.0f));

			break;
		}
		case Flexibility::MEDIUM: {
			leg1Joints[0]->SetRotationBoundaries(std::pair<float, float>(-105.0f, 105.0f));
			leg2Joints[0]->SetRotationBoundaries(std::pair<float, float>(-105.0f, 105.0f));
			leg1Joints[1]->SetRotationBoundaries(std::pair<float, float>(-120.0f, 0.0f));
			leg2Joints[1]->SetRotationBoundaries(std::pair<float, float>(-120.0f, 0.0f));

			break;
		}
		case Flexibility::HIGH: {
			leg1Joints[0]->SetRotationBoundaries(std::pair<float, float>(-160.0f, 160.0f));
			leg2Joints[0]->SetRotationBoundaries(std::pair<float, float>(-160.0f, 160.0f));
			leg1Joints[1]->SetRotationBoundaries(std::pair<float, float>(-160.0f, 0.0f));
			leg2Joints[1]->SetRotationBoundaries(std::pair<float, float>(-160.0f, 0.0f));

			break;
		}
		
		default:
			std::cout << "No valid Flexibility value.\n";
	}
}