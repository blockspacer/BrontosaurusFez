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
#include "../CommonUtilities/EKeyboardKeys.h"

#include <iostream>

InputController::InputController(const CU::Camera& aPlayerCamera)
	: myPlayerCamera(aPlayerCamera)
	, myMouseIsDown(false)
	, myIsShiftDown(false)
{
	PostMaster::GetInstance().Subscribe(this, eMessageType::eMouseMessage);
	PostMaster::GetInstance().Subscribe(this, eMessageType::eKeyboardMessage);
	mySkillInputMessageActivators.Init(5);
	mySkillActivatorKeyDown = -1;
}


InputController::~InputController()
{
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eMouseMessage);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eKeyboardMessage);
}

void InputController::Update(float aDeltaTime)
{
	if (myMouseIsDown == true)
	{
		//convert pixel mouse position to world ground position
		CU::Vector2f windowSize(WINDOW_SIZE);
		CU::Vector2f mousePosZeroToOne = myMousePosition / windowSize;
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

		CU::Vector2f targetPosition(targetPosition3D.x, targetPosition3D.z);
		
		TakeInputMessage(CU::eInputMessage::LEFTMOUSEBUTTON);
		eComponentMessageType type = eComponentMessageType::eSetNavigationTarget;
		SComponentMessageData data;
		data.myVector2f = targetPosition;
		GetParent()->NotifyComponents(type, data);

		if(myIsShiftDown == false)
		{
			type = eComponentMessageType::eSetSkillTargetPosition;
			GetParent()->NotifyComponents(type, data);
		
		}
		else
		{
			type = eComponentMessageType::eSetSkillTargetPositionWhileHoldingPosition;
			GetParent()->NotifyComponents(type, data);
		}
	}

	if(myIsShiftDown == true)
	{
		GetParent()->NotifyComponents(eComponentMessageType::eStopMovement, SComponentMessageData());
	}

	if(mySkillActivatorKeyDown >= 0)
	{
		eComponentMessageType type = eComponentMessageType::eSelectSkill;
		SComponentMessageData data;
		data.myInt = mySkillActivatorKeyDown;
		GetParent()->NotifyComponents(type, data);
	}
}

eMessageReturn InputController::Recieve(const Message& aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}

void InputController::Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData)
{
	if (aMessageType == eComponentMessageType::eAddSkill)
	{
		if(aMessageData.myString == "BasicAttack")
		{
			mySkillInputMessageActivators.Add(CU::eInputMessage::DIVIDE);
		}
		else if (aMessageData.myString == "WhirlWind")
		{
			mySkillInputMessageActivators.Add(CU::eInputMessage::ONE);
		}
		else if (aMessageData.myString == "SweepAttack")
		{
			mySkillInputMessageActivators.Add(CU::eInputMessage::TWO);
		}
		else
		{
			std::cout << "Skill not found when adding key binding." << std::endl;
		}
	}
}

void InputController::Destroy()
{
}

eMessageReturn InputController::MouseClicked(const CU::eMouseButtons aMouseButton, const CU::Vector2f& aMousePosition)
{
	if(aMouseButton == CU::eMouseButtons::LBUTTON)
	{
		myMouseIsDown = true;
		myMousePosition = aMousePosition;
	}
	else if(aMouseButton == CU::eMouseButtons::RBUTTON)
	{
		TakeInputMessage(CU::eInputMessage::RIGHTMOUSEBUTTON);
	}
	else if (aMouseButton == CU::eMouseButtons::MIDBUTTON)
	{
		TakeInputMessage(CU::eInputMessage::MIDDLEMOUSEBUTTON);
	}
	return eMessageReturn::eContinue;
}

eMessageReturn InputController::MouseReleased(const CU::eMouseButtons aMouseButton, const CU::Vector2f & aMousePosition)
{
	if (aMouseButton == CU::eMouseButtons::LBUTTON)
	{
		myMouseIsDown = false;
	}

	return eMessageReturn::eContinue;
}

eMessageReturn InputController::MouseMoved(const CU::Vector2f& aMousePosition)
{
	myMousePosition = aMousePosition;

	return eMessageReturn::eContinue;
}

eMessageReturn InputController::TakeInputMessage(const CU::eInputMessage aInputMessage)
{
	for(unsigned int i = 0; i < mySkillInputMessageActivators.Size(); i++)
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

eMessageReturn InputController::TakeKeyPressed(const CU::eKeys & aKey)
{
	if(aKey == CU::eKeys::LSHIFT)
	{
		myIsShiftDown = true;
	}

	for (unsigned short i = 0; i < mySkillInputMessageActivators.Size(); i++)
	{
		if (mySkillInputMessageActivators[i] == static_cast<CU::eInputMessage>(aKey))
		{
			mySkillActivatorKeyDown = i;
		}
	}

	return eMessageReturn::eContinue;
}

eMessageReturn InputController::TakeKeyReleased(const CU::eKeys & aKey)
{
	if (aKey == CU::eKeys::LSHIFT)
	{
		myIsShiftDown = false;
	}

	for(unsigned short i = 0; i < mySkillInputMessageActivators.Size(); i++)
	{
		if(mySkillInputMessageActivators[i] == static_cast<CU::eInputMessage>(aKey))
		{
			mySkillActivatorKeyDown = -1;
		}
	}
	return eMessageReturn::eContinue;
}
