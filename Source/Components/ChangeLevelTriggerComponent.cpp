#include "stdafx.h"
#include "ChangeLevelTriggerComponent.h"
#include "PollingStation.h"
#include "../Game/LevelManager.h"


ChangeLevelTriggerComponent::ChangeLevelTriggerComponent(const unsigned char aLevelToGoTo, const unsigned short aPortalID)
{
	myLevelToGoTo = aLevelToGoTo;
	myPortalID = aPortalID;
	myType = eComponentType::eChangeLevelTrigger;
}


ChangeLevelTriggerComponent::~ChangeLevelTriggerComponent()
{
}

void ChangeLevelTriggerComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
	case(eComponentMessageType::eActivate):
		if (myPortalID == 0)
		{
			CLevelManager::GetInstance()->GoToLevel(myLevelToGoTo);
		}
		else
		{
			for (int i = 0; i < PollingStation::OpenPortals.Size(); i++)
			{
				if (PollingStation::OpenPortals[i] == myPortalID)
				{
					CLevelManager::GetInstance()->GoToLevel(myLevelToGoTo);
				}
			}
		}
		break;
	default:
		break;
	}
}

void ChangeLevelTriggerComponent::Destroy()
{
}
