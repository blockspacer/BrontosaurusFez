#include "stdafx.h"
#include "OpenShopListenerComponent.h"
#include "../PostMaster/PostMaster.h"
#include "../PostMaster/Message.h"
#include "../PostMaster/PushState.h"


COpenShopListenerComponent::COpenShopListenerComponent()
{
	myType = eComponentType::eOpenShopListener;
}


COpenShopListenerComponent::~COpenShopListenerComponent()
{
}

void COpenShopListenerComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
	case eComponentMessageType::eActivate:
		PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::eHatShop, -1)));
		break;
	}
}

void COpenShopListenerComponent::Destroy()
{
}
