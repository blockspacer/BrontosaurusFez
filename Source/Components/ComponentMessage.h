#pragma once
#include "../CommonUtilities/matrix33.h"
#include "../CommonUtilities/matrix44.h"
#include "StatStructs.h"

class ICollider;
class CComponent;
class CGameObject;
class Skill;

namespace CU
{
	template<typename ObjectType, typename SizeType = unsigned int, bool USE_SAFE_MODE = true>
	class GrowingArray;

	template<typename TYPE>
	class Matrix44;
	using Matrix44f = Matrix44<float>;

	template<typename TYPE>
	class Matrix33;
	using Matrix33f = Matrix33<float>;

	template<typename TYPE>
	class Vector3;
	using Vector3f = Vector3<float>;

	template<typename TYPE>
	class Vector2;
	using Vector2f = Vector2<float>;
}

enum class eComponentType : int;


enum class eComponentMessageType
{
	eAddComponent,
	eCollision,
	ePlaySound,
	eMoving,
	eDied,
	eRespawned,
	eHeal,
	ePercentHPLeft,
	eLootDropped,
	eLootPickedUp,
	eCollectStats,
	eObjectiveCompleted,
	eSetDamage,
	eSetNavigationTarget,
	eSetPath,
	eStartedMoving,
	eStoppedMoving,
	eChangedDirection,
	eSetVisibility,
	eStatsUpdated,
	eSetMaxHealthFromStats,
	eSelectSkill,
	eSetSkillTargetPosition,
	eSetSkillTargetObject,
	eAddSkill,
	eTakeDamage,
	eOnCollisionEnter,
	eOnCollisionExit,
	eStopMovement,
	eSetIsColliderActive,
	eBasicAttack,
	eActivateSkillCollider,
	eSetSkillTargetPositionWhileHoldingPosition,
	eObjectDone,
	eAddAIBehavior,
	eCheckIfCanUseSkill,
	eBurnMana,
	ePercentMPLeft,
	eSetMaxManaFromStats
};

struct SComponentMessageData
{
	SComponentMessageData() : myComponent(nullptr) {}

	union
	{
		CComponent* myComponent;
		CComponent* myComponents[2];
		CGameObject* myGameObject;
		int myInt;
		unsigned short myUShort;
		unsigned char myUChar;
		bool myBool;
		const char* myString;
		CU::Matrix33f myProjectileRotationMatrix;
		CU::Matrix44f myMatrix44;
		CU::Vector2f myVector2f;
		CU::Vector3f myVector3f;
		CU::GrowingArray<CU::Vector3f, unsigned int, false>* myVector3ListPointer;
		
		eComponentType myComponentTypeAdded;
		Stats::STotalStats myStatStruct;
		ICollider* myCollider;
		Skill* mySkill;
	};
};

