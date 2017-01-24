#include "stdafx.h"
#include "MasterAI.h"
#include "PollingStation.h"
#include "../Components/PlayerData.h"
#include "GameObject.h"
#include "ComponentMessage.h"

CMasterAI* CMasterAI::ourInstance = nullptr;

CMasterAI::CMasterAI()
{

}

CMasterAI::~CMasterAI()
{
}

void CMasterAI::Create()
{
	if (ourInstance == nullptr)
	{
		ourInstance = new CMasterAI();
	}
}

void CMasterAI::Destroy()
{
	SAFE_DELETE(ourInstance);
}

CMasterAI & CMasterAI::GetInstance()
{
	return *ourInstance;
}

const float CMasterAI::DetermineHealthDrop()
{

	float result = 75 - PollingStation::playerData->myPercentHPLeft;

	if (result < 0)
	{
		return 0;
	}
	return result;
}

void CMasterAI::DetermineAmountOfMinionsToSpawn()
{

}

void CMasterAI::CallForHelp(CGameObject* aEnemyObject, float aCallForHelpRadius)
{
	unsigned short formationIndex = 0;
	for(unsigned short i = 0; i < PollingStation::myThingsEnemiesShouldAvoid.Size(); i++)
	{
		float distance2 = CU::Vector3f(PollingStation::myThingsEnemiesShouldAvoid[i]->GetWorldPosition() - aEnemyObject->GetWorldPosition()).Length2();
		if(distance2 < aCallForHelpRadius * aCallForHelpRadius)
		{
			SComponentMessageData data;
			data.myUShort = formationIndex;
			PollingStation::myThingsEnemiesShouldAvoid[i]->NotifyComponents(eComponentMessageType::eCalledForHelp, data);
			formationIndex++;
		}
	}
}