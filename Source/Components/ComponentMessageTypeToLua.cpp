#include "stdafx.h"

#include "ComponentMessageTypeToLua.h"

#define TO_STRING(something) #something

static std::map<std::string, eComponentMessageType> locComponentTypeNames;

void InitComponentTypes();

int ComponentMessage::GetType(const std::string& aTypeName)
{
	if (locComponentTypeNames.begin() == locComponentTypeNames.end())
	{
		InitComponentTypes();
	}

	auto it = locComponentTypeNames.find(aTypeName);
	if (it != locComponentTypeNames.end())
	{
		return static_cast<int>(it->second);
	}

	return -1;
}

void InitComponentTypes()
{
	locComponentTypeNames[TO_STRING(eAddComponent)]	= eComponentMessageType::eAddComponent;
	locComponentTypeNames[TO_STRING(ePlaySound)] = eComponentMessageType::ePlaySound;
	locComponentTypeNames[TO_STRING(eMoving)] = eComponentMessageType::eMoving;
	locComponentTypeNames[TO_STRING(eInit)] = eComponentMessageType::eInit;
	locComponentTypeNames[TO_STRING(eDied)] = eComponentMessageType::eDied;
	locComponentTypeNames[TO_STRING(eRespawned)] = eComponentMessageType::eRespawned;
	locComponentTypeNames[TO_STRING(eHeal)] = eComponentMessageType::eHeal;
	locComponentTypeNames[TO_STRING(eOnCollisionEnter)] = eComponentMessageType::eOnCollisionEnter;
	locComponentTypeNames[TO_STRING(eOnCollisionExit)] = eComponentMessageType::eOnCollisionExit;

		//eComponentMessageType::ePercentHPLeft:
		//eComponentMessageType::eLootDropped:
		//eComponentMessageType::eLootPickedUp:
		//eComponentMessageType::eCollectStats:
		//eComponentMessageType::eObjectiveCompleted:
		//eComponentMessageType::eSetDamage:
		//eComponentMessageType::eSetNavigationTarget:
		//eComponentMessageType::eSetPath:
		//eComponentMessageType::eStartedMoving:
		//eComponentMessageType::eStoppedMoving:
		//eComponentMessageType::eChangedDirection:
		//eComponentMessageType::eSetVisibility:
		//eComponentMessageType::eStatsUpdated:
		//eComponentMessageType::eSetMaxHealthFromStats:
		//eComponentMessageType::eSelectSkill:
		//eComponentMessageType::eSetSkillTargetPosition:
		//eComponentMessageType::eAddSkill:
		//eComponentMessageType::eTakeDamage:
		//eComponentMessageType::eStopMovement:
		//eComponentMessageType::eSetIsColliderActive:
		//eComponentMessageType::eBasicAttack:
		//eComponentMessageType::eActivateSkillCollider:
		//eComponentMessageType::eSetSkillTargetPositionWhileHoldingPosition:
		//eComponentMessageType::eObjectDone:
		//eComponentMessageType::eAddAIBehavior:
		//eComponentMessageType::eCheckIfCanUseSkill:
		//eComponentMessageType::eBurnMana:
		//eComponentMessageType::ePercentMPLeft:
		//eComponentMessageType::eSetMaxManaFromStats:
		//eComponentMessageType::eAddHat:
		//eComponentMessageType::eAddStats:
		//eComponentMessageType::eAddToMaxHealth:
		//eComponentMessageType::eAddToMaxMana:
		//eComponentMessageType::eAddToMovementSpeed:
		//eComponentMessageType::ePickUp:
		//eComponentMessageType::eRestoreMana:
		//eComponentMessageType::eActivateBurningBasicAttack:
		//eComponentMessageType::eActivateManaRefund:
		//eComponentMessageType::eActivate:
		//eComponentMessageType::eCalledForHelp:
		//eComponentMessageType::eLookAt:
		//eComponentMessageType::eAddMessageSubscription:
		//eComponentMessageType::eLength:
}