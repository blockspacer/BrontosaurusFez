#pragma once
#include "../CommonUtilities/matrix33.h"
#include "../CommonUtilities/matrix44.h"
#include "StatStructs.h"

class CPath;
class ICollider;
class CComponent;
class CPickupComponent;
class CGameObject;
class Skill;
struct SNavmeshTriangle;
struct SHat;

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
enum class eComponentType : unsigned char;


enum class eComponentMessageType
{
	eAddComponent,
	ePlaySound,
	eMoving,
	eDied,
	eRespawned,
	eHeal,
	eHealPercent,
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
	eSetMaxManaFromStats,
	eAddHat,
	eAddStats,
	eAddToMaxHealth,
	eAddToMaxMana,
	eAddToMovementSpeed,
	ePickUp,
	eRestoreMana,
	eRestorePercentMana,
	eActivateBurningBasicAttack,
	eActivateManaRefund,
	eActivate,
	eCalledForHelp,
	eLookAt,
	eAddMessageSubscription,
	eEnterNewTriangle,
	eNotOnNavmesh,
	eSetPathPosition,
	eLength,
	eSetHighLight,
};

#define STATIC_SIZEOF(x) { char STATIC_SIZEOF_TEMP[(x)]; STATIC_SIZEOF_TEMP = 1; }

struct SComponentMessageData
{
	SComponentMessageData() : myVoidPointer(nullptr) {}

	union
	{
		void* myVoidPointer;
		CComponent* myComponent;
		CPickupComponent* myPickupComponent;
		CGameObject* myGameObject;
		struct SComponentMessageCallback* myComponentMessageCallback;
		int myInt;
		unsigned short myUShort;
		unsigned char myUChar;
		bool myBool;
		const char* myString;
		float myFloat;
		CU::Vector2f myVector2f;
		CU::Vector3f myVector3f;


		CPath* myPathPointer;
		eComponentType myComponentTypeAdded;
		Stats::STotalStats* myStatStruct;
		Stats::SBonusStats* myStatsToAdd;
		ICollider* myCollider;
		Skill* mySkill;
		SHat* myHat;
		SNavmeshTriangle* myTrianglePointer;
	};
};

