#include "stdafx.h"
#include "Component.h"


CComponent::CComponent() : myParent(nullptr)
{
}


CComponent::~CComponent()
{

}

void CComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{

}
