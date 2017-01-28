#include "stdafx.h"
#include "PlayerManaMessenger.h"
#include "..\PostMaster\PlayerManaChanged.h"

CPlayerManaMessenger::CPlayerManaMessenger()
{
	myType = eComponentType::ePlayerManaMessenger;
}

CPlayerManaMessenger::~CPlayerManaMessenger()
{
}

void CPlayerManaMessenger::Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData)
{
	switch (aMessageType)
	{
	case eComponentMessageType::eCheckIfCanUseSkill:
		break;
	case eComponentMessageType::eBurnMana:
		break;
	case eComponentMessageType::ePercentMPLeft:
		PostMaster::GetInstance().SendLetter(eMessageType::ePlayerManaChanged, CPlayerManaChanged(static_cast<float>(aMessageData.myUChar) * 0.01f));
		break;
	case eComponentMessageType::eSetMaxManaFromStats:
		break;
	default:
		break;
	}
}

void CPlayerManaMessenger::Destroy()
{
}
