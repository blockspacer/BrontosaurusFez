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

void CComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData)
{
	aMessageType;
	aMessageData;
}
