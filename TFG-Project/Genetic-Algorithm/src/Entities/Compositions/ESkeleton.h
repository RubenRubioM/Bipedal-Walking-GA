#pragma once

#include <GLM/vec3.hpp>

#include <vector>

class EMesh;

/// <summary>
/// Skeleton auxiliar class
/// </summary>
class ESkeleton{
	public:
		/// <summary>
		/// Enumeration to set body flexibility level.
		/// </summary>
		enum Flexibility {LOW, MEDIUM, HIGH};

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
		/// Returns skeleton identifier.
		/// </summary>
		/// <returns> Skeleton Id. </returns>
		const int GetSkeletonId() { return skeletonId; }

		/// <summary>
		/// Returns number of joints.
		/// </summary>
		/// <returns> NumOfJoints. </returns>
		const int GetJointsNum() { return numOfJoints; }

		/// <summary>
		/// Returns the flexibility.
		/// </summary>
		/// <returns> Flexibility. </returns>
		const Flexibility GetFlexibility() { return flexibility; }

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

		/// <summary>
		/// Returns on air.
		/// </summary>
		/// <returns> On air. </returns>
		bool GetOnAir() { return onAir; };

		/// <summary>
		/// Returns leg1 on air.
		/// </summary>
		/// <returns> Leg1 on air. </returns>
		bool GetLeg1OnAir() { return leg1OnAir; };

		/// <summary>
		/// Returns leg2 on air.
		/// </summary>
		/// <returns> Leg1 on air. </returns>
		bool GetLeg2OnAir() { return leg2OnAir; };

		/// <summary>
		/// Returns the fitness value of the skeleton.
		/// </summary>
		/// <returns> Fitness value. </returns>
		const float GetFitness() { return fitnessValue; }

		/// <summary>
		/// Sets the skeleton flexibility.
		/// </summary>
		/// <param name="flexibility"> Flexibility. </param>
		void SetFlexibility(const Flexibility flexibility);

		/// <summary>
		/// Sets the skeleton on air.
		/// </summary>
		/// <param name="onAir"> On air. </param>
		void SetOnAir(const bool onAir) { this->onAir = onAir; }

		/// <summary>
		/// Sets the skeleton's leg1 on air.
		/// </summary>
		/// <param name="onAir"> On air. </param>
		void SetLeg1OnAir(const bool onAir) { this->leg1OnAir = onAir; }

		/// <summary>
		/// Sets the skeleton's leg2 on air.
		/// </summary>
		/// <param name="onAir"> On air. </param>
		void SetLeg2OnAir(const bool onAir) { this->leg2OnAir = onAir; }

		/// <summary>
		/// Kill the skeleton.
		/// </summary>
		/// <param name="dead"> Is dead </param>
		void SetIsDead(bool dead) { isDead = dead; }

		/// <summary>
		/// Sets new fitness value.
		/// </summary>
		/// <param name="newFitness"> New fitness value. </param>
		void SetFitness(const float newFitness) { fitnessValue = newFitness; }

		/// <summary>
		/// Returns if the skeleton is dead.
		/// </summary>
		/// <returns> IsDead. </returns>
		bool IsDead() { return isDead; }

		/// <summary>
		/// Updates the fitness value.
		/// </summary>
		void UpdateFitness();

	private:
		/// <summary>
		/// Skeleton identifier.
		/// </summary>
		int skeletonId = 0;

		/// <summary>
		/// Num of joints.
		/// </summary>
		int numOfJoints = 0;

		/// <summary>
		/// Skeleton flexibility.
		/// </summary>
		Flexibility flexibility{ Flexibility::MEDIUM };

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

		/// <summary>
		/// To verify if the skeleton is on air.
		/// </summary>
		bool onAir{ true };

		/// <summary>
		/// To verify if the skeleton's leg1 is on air.
		/// </summary>
		bool leg1OnAir{ true };

		/// <summary>
		/// To verify if the skeleton's leg2 is on air.
		/// </summary>
		bool leg2OnAir{ true };

		/// <summary>
		/// To check if skeleton is dead.
		/// </summary>
		bool isDead{ false };

		/// <summary>
		/// Fitness value.
		/// Basically the distance between te actual point and the beginning point.
		/// </summary>
		float fitnessValue{ 0 };

		/// <summary>
		/// Starting point.
		/// </summary>
		glm::vec3 startingPoint{ 0.0f };

		/// <summary>
		/// Static count for skeleton id's.
		/// </summary>
		inline static int skeletonNextId = 0;
};
