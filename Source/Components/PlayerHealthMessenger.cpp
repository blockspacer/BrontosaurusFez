#include "stdafx.h"
#include "PlayerHealthMessenger.h"
#include "..\PostMaster\PlayerHealthChanged.h"
#include "../Game/PollingStation.h"
#include "../Components/PlayerData.h"

CPlayerHealthMessenger::CPlayerHealthMessenger()
{
}

CPlayerHealthMessenger::~CPlayerHealthMessenger()
{
}

void CPlayerHealthMessenger::Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData)
{
	switch (aMessageType)
	{
	case eComponentMessageType::eDied:
		break;
	case eComponentMessageType::eRespawned:
		break;
	case eComponentMessageType::eHeal:
		break;
	case eComponentMessageType::ePercentHPLeft:
		PostMaster::GetInstance().SendLetter(eMessageType::ePlayerHealthChanged, CPlayerHealthChanged(static_cast<float>(aMessageData.myUChar) * 0.01f));
		PollingStation::playerData->myPercentHPLeft = static_cast<float>(aMessageData.myUChar);
		break;
	}
}

void CPlayerHealthMessenger::Destroy()
{
	throw std::logic_error("The method or operation is not implemented.");
}
