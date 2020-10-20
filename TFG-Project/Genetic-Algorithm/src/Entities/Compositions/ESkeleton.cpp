#include "ESkeleton.h"

/// <summary>
/// ESkeleton constructor.
/// </summary>
/// <param name="core"> Skeleton core. </param>
/// <param name="hip1"> Skeleton leg1Joints[0]. </param>
/// <param name="knee1"> Skeleton leg1Joints[1]. </param>
/// <param name="hip2"> Skeleton leg1Joints[0]. </param>
/// <param name="knee2"> Skeleton leg1Joints[1]. </param>
ESkeleton::ESkeleton(EMesh* core, EMesh* hip1, EMesh* knee1, EMesh* hip2, EMesh* knee2){
	this->core = core; numOfJoints++;
	this->leg1Joints[0] = hip1; numOfJoints++;
	this->leg1Joints[1] = knee1; numOfJoints++;
	this->leg2Joints[0] = hip2; numOfJoints++;
	this->leg2Joints[1] = knee2; numOfJoints++;
}

/// <summary>
/// ESkeleton destructor.
/// </summary>
ESkeleton::~ESkeleton(){

}
