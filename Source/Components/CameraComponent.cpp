#include "stdafx.h"
#include "CameraComponent.h"
#include "GameObject.h"
#include "../CommonUtilities/matrix44.h"
#include "../CommonUtilities/Camera.h"

//CU::Matrix44f CCameraComponent::GetToWorldTransformation()
//{
//	return myCamera->GetTransformation();
//}

void CCameraComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData)
{
	if (myCamera == nullptr)
	{
		return;
	}

	switch (aMessageType)
	{
	case eComponentMessageType::eMoving:
		myCamera->SetPosition(myCamera->GetPosition() + aMessageData.myVector3f /*GetParent()->GetLocalTransform().GetPosition()*/); //local?
		break;
	}
}

void CCameraComponent::Destroy()
{
	DL_ASSERT("Not yet implemented");
}

void CCameraComponent::SetCamera(CU::Camera& aCamera)
{
	myCamera = &aCamera;
}

CCameraComponent::CCameraComponent()
{
}

CCameraComponent::~CCameraComponent()
{
}
