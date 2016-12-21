#include "stdafx.h"
#include "InputController.h"
#include "../PostMaster/Message.h"
#include "../PostMaster/PostMaster.h"
#include "../PostMaster/Event.h"
#include "../CommonUtilities/EInputMessage.h"
#include "../CommonUtilities/EMouseButtons.h"
#include "GameObject.h"
#include "../BrontosaurusEngine/Engine.h"
#include "CameraManager.h"
#include "../CommonUtilities/Camera.h"

InputController::InputController(const CU::Camera& aPlayerCamera)
	: myPlayerCamera(aPlayerCamera)
{
	PostMaster::GetInstance().AppendSubscriber(this, eMessageType::eMouseMessage);
	PostMaster::GetInstance().AppendSubscriber(this, eMessageType::eMouseDownMessage);
	PostMaster::GetInstance().AppendSubscriber(this, eMessageType::eInputMessagePressed);
	mySkillInputMessageActivators.Init(5);
}


InputController::~InputController()
{
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eMouseMessage);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eMouseDownMessage);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eInputMessagePressed);
}

void InputController::Update(float aDeltaTime)
{
}

eMessageReturn InputController::Recieve(const Message & aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}

void InputController::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
}

void InputController::Destroy()
{
}

eMessageReturn InputController::MouseClicked(const CU::eMouseButtons aMouseButton, const CU::Vector2f & aMousePosition)
{
	if(aMouseButton == CU::eMouseButtons::LBUTTON)
	{
		//convert pixel mouse position to world ground position
		CU::Vector2f windowSize(WINDOW_SIZE);
		CU::Vector2f mousePosZeroToOne = aMousePosition / windowSize;
		CU::Vector2f mousePosNormalizedSpace = mousePosZeroToOne * 2.f - CU::Vector2f::One;
		CU::Vector4f mousePosNormalizedHomogeneousSpace(mousePosNormalizedSpace, CU::Vector2f::Zero);
		CU::Vector4f screenToCameraSpaceRay = mousePosNormalizedHomogeneousSpace * myPlayerCamera.GetProjectionInverse();

		CU::Vector4f direction;
		direction.x = (screenToCameraSpaceRay.x * myPlayerCamera.GetTransformation().m11) + (screenToCameraSpaceRay.y * myPlayerCamera.GetTransformation().m21) + myPlayerCamera.GetTransformation().m31;
		direction.y = (screenToCameraSpaceRay.x * myPlayerCamera.GetTransformation().m12) + (screenToCameraSpaceRay.y * myPlayerCamera.GetTransformation().m22) + myPlayerCamera.GetTransformation().m32;
		direction.z = (screenToCameraSpaceRay.x * myPlayerCamera.GetTransformation().m13) + (screenToCameraSpaceRay.y * myPlayerCamera.GetTransformation().m23) + myPlayerCamera.GetTransformation().m33;
		direction.w = 0.f;

		CU::Vector4f targetPosition3D;
		const CU::Vector4f groundNormal(0.f, 1.f, 0.f, 0.f);
		float denominator = direction.Dot(groundNormal);
		if (std::fabs(denominator) > 0.0001f)
		{
			float t = (GetParent()->GetToWorldTransform().GetPosition() - myPlayerCamera.GetPosition()).Dot(groundNormal.myVector3) / denominator;
			if (std::fabs(t) > 0.0001f)
			{
				targetPosition3D = myPlayerCamera.GetPosition() + CU::Vector3f(direction * t);
			}
		}

		CU::Vector2f targetPosition(targetPosition3D.x, -targetPosition3D.z);
		
		eComponentMessageType type = eComponentMessageType::eSetNavigationTarget;
		SComponentMessageData data;
		data.myVector2f = targetPosition;
		GetParent()->NotifyComponents(type, data);
	}

	return eMessageReturn::eContinue;
}

eMessageReturn InputController::TakeInputMessage(const CU::eInputMessage aInputMessage)
{
	for(unsigned short i = 0; i < mySkillInputMessageActivators.Size(); i++)
	{
		if(mySkillInputMessageActivators[i] == aInputMessage)
		{
			eComponentMessageType type = eComponentMessageType::eSelectSkill;
			SComponentMessageData data;
			data.myInt = i;
			GetParent()->NotifyComponents(type, data);
		}
	}
	return eMessageReturn::eContinue;
}
