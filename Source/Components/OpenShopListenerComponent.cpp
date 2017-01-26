#include "stdafx.h"
#include "OpenShopListenerComponent.h"
#include "../PostMaster/PostMaster.h"
#include "../PostMaster/Message.h"
#include "../PostMaster/PushState.h"

COpenShopListenerComponent::COpenShopListenerComponent()
{
	myType = eComponentType::eOpenShopListener;
	myIsActive = true;
	PostMaster::GetInstance().Subscribe(this, eMessageType::eShopClosed);
}


COpenShopListenerComponent::~COpenShopListenerComponent()
{
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eShopClosed);
}

void COpenShopListenerComponent::Reactivate()
{
	if (myIsActive == false)
	{
		myIsActive = true;
		SComponentMessageData message;
		message.myBool = true;
		GetParent()->NotifyComponents(eComponentMessageType::eSetIsColliderActive, message);
	}
}

void COpenShopListenerComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
	case eComponentMessageType::eActivate:
		if (myIsActive == true)
		{
			PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, PushState(PushState::eState::eHatShop, -1)));
			SComponentMessageData message;
			myIsActive = false;
			message.myBool = false;
			GetParent()->NotifyComponents(eComponentMessageType::eSetIsColliderActive, message);
			break;
		}
	}
}

void COpenShopListenerComponent::Destroy()
{
}

eMessageReturn COpenShopListenerComponent::Recieve(const Message & aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}
