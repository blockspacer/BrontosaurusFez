#include "stdafx.h"
#include "AudioSourceComponent.h"
#include "GameObject.h"
#include "ComponentManager.h"
#include "../Audio/AudioInterface.h"

CAudioSourceComponent::CAudioSourceComponent()
{
	myIsActive = true;
	myAudioInterface = Audio::CAudioInterface::GetInstance();
}


CAudioSourceComponent::~CAudioSourceComponent()
{
	myAudioInterface = nullptr;
}

void CAudioSourceComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
	case eComponentMessageType::ePlaySound:
		PushEvent(aMessageData.myString);
		break;
	default:
		break;
	}
}

void CAudioSourceComponent::Update()
{
	if (myIsActive == true)
	{
		CU::Matrix44f& temp = GetParent()->GetToWorldTransform();
		myAudioInterface->SetGameObjectPosition(myGameObjectID, temp.GetPosition(), temp.myForwardVector);
	}
}

void CAudioSourceComponent::PushEvent(const char * aEvent)
{
	if (myIsActive == true)
	{
		myAudioInterface->PostEvent(aEvent, myGameObjectID);
	}
}

void CAudioSourceComponent::Destroy()
{
	GetParent()->GetComponents().RemoveCyclic(this);
	CComponentManager::GetInstance().RemoveComponent(myGameObjectID);
}
