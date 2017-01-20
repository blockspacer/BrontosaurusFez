#include "stdafx.h"
#include "MasterAI.h"
#include "PollingStation.h"
#include "../Components/PlayerData.h"


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
