#pragma once

#include <vector>

class EMesh;

/// <summary>
/// Skeleton auxiliar class
/// </summary>
class ESkeleton{
	public:
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
		ESkeleton(EMesh* core, EMesh* hip1, EMesh* knee1, EMesh* hip2, EMesh* knee2, EMesh* shoulder1, EMesh* elbow1, EMesh* shoulder2, EMesh* elbow2);

		/// <summary>
		/// ESkeleton destructor.
		/// </summary>
		~ESkeleton();

		/// <summary>
		/// Returns number of joints.
		/// </summary>
		/// <returns> NumOfJoints. </returns>
		const int GetJointsNum() { return numOfJoints; }

		/// <summary>
		/// Returns all the skeleton parts.
		/// [0] = core
		/// [1] [2] = leg1
		/// [3] [4] = leg2
		/// [5] [6] = arm1
		/// [7] [8] = arm2
		/// </summary>
		/// <returns> All the skeleton parts. </returns>
		std::vector<EMesh*> GetSkeleton();

		/// <summary>
		/// Returns the core of the skeleton.
		/// </summary>
		/// <returns> Skeleton Core. </returns>
		EMesh* GetCore() const { return core; };

		/// <summary>
		/// Returns leg1 of the skeleton
		/// </summary>
		/// <returns> Skeleton leg1. </returns>
		std::vector<EMesh*> GetLeg1() { return leg1Joints; };

		/// <summary>
		/// Returns leg2 of the skeleton.
		/// </summary>
		/// <returns> Skeleton leg2. </returns>
		std::vector<EMesh*> GetLeg2() { return leg2Joints; };

		/// <summary>
		/// Returns arm1 of the skeleton.
		/// </summary>
		/// <returns> Skeleton arm1. </returns>
		std::vector<EMesh*> GetArm1() { return arm1Joints; };

		/// <summary>
		/// Returns arm2 of the skeleton.
		/// </summary>
		/// <returns> Skeleton arm2. </returns>
		std::vector<EMesh*> GetArm2() { return arm2Joints; };

	private:
		/// <summary>
		/// Num of joints.
		/// </summary>
		int numOfJoints = 0;

		/// <summary>
		/// Pointer to the core of the skeleton. The one you have to move in order to move the rest.
		/// </summary>
		EMesh* core{ nullptr };

		/// <summary>
		/// Array for the 2 joints of leg 1.
		/// leg1Joints[1] = Hip.
		/// leg1Joints[2] = Knee.
		/// </summary>
		std::vector<EMesh*> leg1Joints = { nullptr,nullptr };

		/// <summary>
		/// Array for the 2 joints of leg 2.
		/// leg1Joints[1] = Hip.
		/// leg1Joints[2] = Knee.
		/// </summary>
		std::vector<EMesh*> leg2Joints = { nullptr,nullptr };

		/// <summary>
		/// Array for the 2 joints of arm 1.
		/// arm1Joints[1] = shoulder.
		/// arm1Joints[2] = elbow.
		/// </summary>
		std::vector<EMesh*> arm1Joints = { nullptr,nullptr };

		/// <summary>
		/// Array for the 2 joints of arm 2.
		/// arm2Joints[1] = shoulder.
		/// arm2Joints[2] = elbow.
		/// </summary>
		std::vector<EMesh*> arm2Joints = { nullptr,nullptr };
};

