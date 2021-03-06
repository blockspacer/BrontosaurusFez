#include "stdafx.h"
#include "Component.h"

CComponent::CComponent()
	: myParent(nullptr)
	, myType(eComponentType::eNone)
	, myId(NULL_COMPONENT)
{
}

CComponent::~CComponent()
{
}

void CComponent::NotifyParent(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData)
{
	if (myParent != nullptr)
	{
		myParent->NotifyComponents(aMessageType, aMessageData);
	}
}

const eComponentType CComponent::GetType() const
{
	return myType;
}

void CComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData)
{
	aMessageType;
	aMessageData;
}

bool CComponent::IsGameObject()
{
	return false;
}
