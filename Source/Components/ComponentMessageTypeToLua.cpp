#include "stdafx.h"

#include "ComponentMessageTypeToLua.h"

#define TO_STRING(something) #something

static std::map<std::string, eComponentMessageType> locComponentTypeNames;
static std::string locFilePath;

void InitComponentTypes();
void InternalPrintComponentInfo();

int ComponentMessage::GetType(const std::string& aTypeName)
{
	if (locComponentTypeNames.begin() == locComponentTypeNames.end())
	{
		InitComponentTypes();
		InternalPrintComponentInfo();
	}

	auto it = locComponentTypeNames.find(aTypeName);
	if (it != locComponentTypeNames.end())
	{
		return static_cast<int>(it->second);
	}

	return -1;
}

bool ComponentMessage::GetTypeName(const eComponentMessageType aType, std::string& aNameOut)
{
	for (auto it = locComponentTypeNames.begin(); it != locComponentTypeNames.end(); ++it)
	{
		if (it->second == aType)
		{
			aNameOut = it->first;
			return true;
		}
	}

	return false;
}

void InitComponentTypes()
{
	locComponentTypeNames[TO_STRING(eAddComponent)]	= eComponentMessageType::eAddComponent;
	locComponentTypeNames[TO_STRING(ePlaySound)] = eComponentMessageType::ePlaySound;
	locComponentTypeNames[TO_STRING(eMoving)] = eComponentMessageType::eMoving;
	locComponentTypeNames[TO_STRING(eDied)] = eComponentMessageType::eDied;
	locComponentTypeNames[TO_STRING(eRespawned)] = eComponentMessageType::eRespawned;
	locComponentTypeNames[TO_STRING(eHeal)] = eComponentMessageType::eHeal;
	locComponentTypeNames[TO_STRING(eOnCollisionEnter)] = eComponentMessageType::eOnCollisionEnter;
	locComponentTypeNames[TO_STRING(eOnCollisionExit)] = eComponentMessageType::eOnCollisionExit;
	locComponentTypeNames[TO_STRING(ePercentHPLeft)] = eComponentMessageType::ePercentHPLeft;
	locComponentTypeNames[TO_STRING(eActivate)] = eComponentMessageType::eActivate;
	locComponentTypeNames[TO_STRING(ePickupVisual)] = eComponentMessageType::ePickupVisual;

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
		//eComponentMessageType::eCalledForHelp:
		//eComponentMessageType::eLookAt:
		//eComponentMessageType::eAddMessageSubscription:
		//eComponentMessageType::eLength:
}

void InternalPrintComponentInfo()
{
	std::ofstream file(locFilePath);
	int index = 1;
	for (auto it = locComponentTypeNames.begin(); it != locComponentTypeNames.end(); ++it)
	{
		file << index++ << ". " << it->first << ": " << static_cast<int>(it->second) << std::endl;
	}
}

void ComponentMessage::PrintComponentsToFile(const std::string& aFilePath)
{
	locFilePath = aFilePath;
}
