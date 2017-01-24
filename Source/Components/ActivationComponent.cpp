#include "stdafx.h"
#include "ActivationComponent.h"


CActivationComponent::CActivationComponent()
{
	myType = eComponentType::eActivationComponent;
}

CActivationComponent::~CActivationComponent()
{
}

void CActivationComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData)
{
	if (aMessageType == eComponentMessageType::eTakeDamage)
	{
		SComponentMessageData messageData;
		GetParent()->NotifyComponents(eComponentMessageType::eActivate, messageData);
	}
}

void CActivationComponent::Destroy()
{

}
