#include "stdafx.h"
#include "RespawnComponent.h"


RespawnComponent::RespawnComponent()
{
	myRespawnTime = 1;
	myElapsedTime = 0;
	myType = eComponentType::eRespawn;
}


RespawnComponent::~RespawnComponent()
{
}

void RespawnComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
	case eComponentMessageType::eDied:
		
		myHasDied = true;

		break;
	}

}

void RespawnComponent::Destroy()
{
}

void RespawnComponent::Update(const float aDeltaTime)
{
	if (myHasDied == true)
	{
		myElapsedTime += aDeltaTime;
		if (myElapsedTime >= myRespawnTime)
		{
			GetParent()->NotifyComponents(eComponentMessageType::eRespawned, SComponentMessageData());
			myHasDied = false;
			myElapsedTime = 0;
		}
	}
}
