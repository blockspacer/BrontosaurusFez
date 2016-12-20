#pragma once
#include "../CommonUtilities/matrix33.h"
#include "../CommonUtilities/matrix44.h"
#include "StatStructs.h"
//#include "../CommonUtilities/GrowingArray.h"
class Collider;
class CComponent;
class CGameObject;

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
	eTookDamage,
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
	eAddSkill,
	eTakeDamage
};

struct SComponentMessageData
{
	SComponentMessageData();

	union
	{
		CComponent* myComponent;
		CComponent* myComponents[2];
		CGameObject* myGameObject;
		bool myBool;
		const char* myString;
		int myInt;
		CU::Matrix33f myProjectileRotationMatrix;
		CU::Matrix44f myMatrix44;
		CU::Vector2f myVector2f;
		CU::Vector3f myVector3f;
		CU::GrowingArray<CU::Vector3f>* myVector3ListPointer;
		
		eComponentType myComponentTypeAdded;
		unsigned short myUnsingedShort;
		Stats::STotalStats myStatStruct;
	};
};

inline SComponentMessageData::SComponentMessageData() : myComponent(nullptr)
{
}
