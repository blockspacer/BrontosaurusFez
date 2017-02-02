#include "stdafx.h"
#include "CameraComponent.h"
#include "GameObject.h"
#include "../CommonUtilities/matrix44.h"
#include "../CommonUtilities/Camera.h"

void CCameraComponent::InitOffsetPosition()
{
	myOffsetPosition = myCamera->GetPosition();
}

void CCameraComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData)
{
	if (myCamera == nullptr)
	{
		return;
	}

	switch (aMessageType)
	{
	case eComponentMessageType::eObjectDone:
	case eComponentMessageType::eMoving:
		myCamera->SetPosition(myOffsetPosition + GetParent()->GetWorldPosition());
		break;
	}
}

void CCameraComponent::Destroy()
{
	DL_ASSERT("Not yet implemented");
}

CCameraComponent::CCameraComponent()
{
	myType = eComponentType::eCamera;
}

CCameraComponent::~CCameraComponent()
{
}
