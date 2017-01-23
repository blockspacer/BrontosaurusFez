#include "stdafx.h"
#include "ChangeLevelTriggerComponent.h"
#include "../Game/LevelManager.h"


ChangeLevelTriggerComponent::ChangeLevelTriggerComponent(unsigned char aLevelToGoTo)
{
	myLevelToGoTo = aLevelToGoTo;
}


ChangeLevelTriggerComponent::~ChangeLevelTriggerComponent()
{
}

void ChangeLevelTriggerComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
	case(eComponentMessageType::eActivate):
		CLevelManager::GetInstance()->GoToLevel(myLevelToGoTo);
		break;
	default:
		break;
	}
}

void ChangeLevelTriggerComponent::Destroy()
{
}
