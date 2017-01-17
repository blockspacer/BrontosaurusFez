#include "stdafx.h"
#include "HatActivator.h"
#include "../Game/PollingStation.h"
#include "InventoryComponent.h"
HatActivator::HatActivator(bool aIsActive, HatActivatorData* someData, SHat* aHatPointer)
{
	myIsActive = aIsActive;
	myData = someData;
	myHatStructPointer = aHatPointer;

	if(myIsActive == true)
	{
		Activate();
	}
}


HatActivator::~HatActivator()
{
}

void HatActivator::CheckHealthLimit(float aHealthPercentage)
{
	if(myData->healthLimitActivator >= 1)
	{
		return;
	}
	if (myData->isLimitActivationUnder == true)
	{
		if (myIsActive == true)
		{
			if (myData->healthLimitActivator > aHealthPercentage)
			{
				Deactivate();
			}
		}
		else
		{
			if (myData->healthLimitActivator < aHealthPercentage)
			{
				Activate();
			}
		}
	}
	else
	{
		if (myIsActive == true)
		{
			if (myData->healthLimitActivator < aHealthPercentage)
			{
				Deactivate();
			}
		}
		else
		{
			if (myData->healthLimitActivator > aHealthPercentage)
			{
				Activate();
			}
		}
	}
}

void HatActivator::CheckManaLimit(float aManaPercentage)
{
	if (myData->manaLimitActivator >= 1)
	{
		return;
	}
	if(myData->isLimitActivationUnder == true)
	{
		if(myIsActive == true)
		{
			if(myData->manaLimitActivator > aManaPercentage)
			{
				Deactivate();
			}
		}
		else
		{
			if (myData->manaLimitActivator < aManaPercentage)
			{
				Activate();
			}
		}
	}
	else
	{
		if (myIsActive == true)
		{
			if (myData->manaLimitActivator < aManaPercentage)
			{
				Deactivate();
			}
		}
		else
		{
			if (myData->manaLimitActivator > aManaPercentage)
			{
				Activate();
			}
		}
	}
}

void HatActivator::Activate()
{
	myIsActive = true;
	SComponentMessageData data;
	data.myStatsToAdd = myHatStructPointer->stat;
	PollingStation::playerObject->NotifyComponents(eComponentMessageType::eAddStats, data);
	
}

void HatActivator::Deactivate()
{
	myIsActive = false;
	SComponentMessageData data;
	data.myStatsToAdd = myHatStructPointer->stat;
	PollingStation::playerObject->NotifyComponents(eComponentMessageType::eAddStats, data);
}
