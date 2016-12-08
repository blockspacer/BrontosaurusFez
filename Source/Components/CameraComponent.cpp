#include "stdafx.h"
#include "CameraComponent.h"
#include "GameObject.h"
#include "../CommonUtilities/matrix44.h"

CU::Matrix44f CCameraComponent::GetToWorldTransformation()
{
	return GetParent()->GetToWorldTransform();
}

void CCameraComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData)
{
	//if (aMessageType == eComponentMessageType::eInitController)
	//{
	//	ControllerComponent* controllerComponent = static_cast<ControllerComponent*>(aMessageData.myComponent);

	//	controllerComponent->SetCamera(GetParent());
	//}
}

void CCameraComponent::Destroy()
{
	DL_ASSERT("Not yet implemented");
}

CCameraComponent::CCameraComponent()
{
}


CCameraComponent::~CCameraComponent()
{
}
