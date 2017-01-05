#include "stdafx.h"
#include "MouseComponent.h"
#include "..\BrontosaurusEngine\Engine.h"
#include "..\CommonUtilities\Camera.h"

CMouseComponent::CMouseComponent(const CU::Camera& aPlayerCamera)
	: myPlayerCamera(aPlayerCamera)
{
	PostMaster::GetInstance().Subscribe(this, eMessageType::eMouseMessage);
}

CMouseComponent::~CMouseComponent()
{
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eMouseMessage);
}

void CMouseComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
	case eComponentMessageType::eOnCollisionEnter: //??
		break;
	}
}

void CMouseComponent::Destroy()
{
	DL_ASSERT("NOT IMPLEMENTED DESTROY");
}

void CMouseComponent::MouseMoved(const CU::Vector2f& aMousePosition)
{
	//NO ONE OWNS THIS SO IF PARENT IS NULL WE UNSUBSCRIBE FROM MOUSE MESSAGE..... mvh carl
	if (GetParent() == nullptr)
	{
		PostMaster::GetInstance().UnSubscribe(this, eMessageType::eMouseMessage);
	}


	//convert pixel mouse position to world ground position
	CU::Vector2f windowSize(WINDOW_SIZE);
	CU::Vector2f mousePosZeroToOne = aMousePosition / windowSize;
	CU::Vector2f mousePosNormalizedSpace = mousePosZeroToOne * 2.f - CU::Vector2f::One;
	mousePosNormalizedSpace.y *= -1;
	CU::Vector4f mousePosNormalizedHomogeneousSpace(mousePosNormalizedSpace, CU::Vector2f::Zero);
	CU::Vector4f screenToCameraSpaceRay = mousePosNormalizedHomogeneousSpace * myPlayerCamera.GetProjectionInverse();

	CU::Vector3f direction;
	direction.x = (screenToCameraSpaceRay.x * myPlayerCamera.GetTransformation().m11) + (screenToCameraSpaceRay.y * myPlayerCamera.GetTransformation().m21) + myPlayerCamera.GetTransformation().m31;
	direction.y = (screenToCameraSpaceRay.x * myPlayerCamera.GetTransformation().m12) + (screenToCameraSpaceRay.y * myPlayerCamera.GetTransformation().m22) + myPlayerCamera.GetTransformation().m32;
	direction.z = (screenToCameraSpaceRay.x * myPlayerCamera.GetTransformation().m13) + (screenToCameraSpaceRay.y * myPlayerCamera.GetTransformation().m23) + myPlayerCamera.GetTransformation().m33;

	CU::Vector3f targetPosition3D;
	const CU::Vector3f groundNormal(0.f, 1.f, 0.f);
	const float denominator = direction.Dot(groundNormal);
	if (std::fabs(denominator) > 0.0001f)
	{
		const float t = (GetParent()->GetToWorldTransform().GetPosition() - myPlayerCamera.GetPosition()).Dot(groundNormal) / denominator;
		if (std::fabs(t) > 0.0001f)
		{
			targetPosition3D = myPlayerCamera.GetPosition() + direction * t;
		}
	}

	GetParent()->GetLocalTransform().SetPosition(targetPosition3D);
	GetParent()->NotifyComponents(eComponentMessageType::eMoving, SComponentMessageData());
	DL_PRINT("mouse pos: %f, %f, %f", targetPosition3D.x, targetPosition3D.y, targetPosition3D.z);
}

eMessageReturn CMouseComponent::Recieve(const Message& aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}
