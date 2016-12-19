#pragma once
#include "../CommonUtilities/matrix33.h"
#include "../CommonUtilities/matrix44.h"
#include "StatStructs.h"
class Collider;
class CComponent;

template <typename T>
class ValueObserver;

namespace CU
{

	// Forward declarations
	//
	template<typename TYPE>
	class Matrix44;


	template<typename TYPE>
	class Matrix33;

	template<typename TYPE>
	class Vector2;

	
}

enum class eComponentType
{
	eModel,
	eDebugSphere,
	eCamera,
	eInventory,
	eNone,
};


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
};

struct SComponentMessageData
{
	SComponentMessageData();

	union
	{
		CComponent* myComponent;
		CComponent* myComponents[2];
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
