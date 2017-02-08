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
#include "SkillData.h"

#include <iostream>
#include "../PostMaster/GameEventMessageEvent.h"


// The Legacy of Kevin!
//HELLLLLLLLLLLLLLLLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//#define true rand() % 100
//CHECK THIS OUTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT

InputController::InputController(const CU::Camera& aPlayerCamera)
	: myPlayerCamera(aPlayerCamera)
	, myMouseIsDown(false)
	, myIsShiftDown(false)
	, myIsActive(true)
	, myIsRightMouseButtonDown(false)
	, myTriangle(nullptr)
{
	PostMaster::GetInstance().Subscribe(this, eMessageType::eMouseMessage);
	PostMaster::GetInstance().Subscribe(this, eMessageType::eKeyboardMessage);
	mySkillInputMessageActivators.Init(5);
	mySkillActivatorKeyDown = -1;
	myType = eComponentType::eInputController;
	myElapsedManaRegenTimer = 1.5f;
	myManaRecover = 1;
	myShouldRecoverManaQuickly = false;
}


InputController::~InputController()
{
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eMouseMessage);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eKeyboardMessage);
}

void InputController::Update(float aDeltaTime)
{
	if (myIsActive == true)
	{
		for (unsigned short i = 0; i < mySkillInputMessageActivators.Size(); i++)
		{
			if(i == 0 && mySkillInputMessageActivators[i] != CU::eInputMessage::DIVIDE)
			{
				mySkillInputMessageActivators[i] = CU::eInputMessage::DIVIDE;
			}
			else if (i == 1 && mySkillInputMessageActivators[i] != CU::eInputMessage::RIGHTMOUSEBUTTON)
			{
				mySkillInputMessageActivators[i] = CU::eInputMessage::RIGHTMOUSEBUTTON;
			}
			else if (i == 2 && mySkillInputMessageActivators[i] != CU::eInputMessage::SPACE)
			{
				mySkillInputMessageActivators[i] = CU::eInputMessage::SPACE;
			}
		}
		myElapsedManaRegenTimer -= aDeltaTime;
		if(myElapsedManaRegenTimer < 0.0f)
		{
			SComponentMessageData manaRecoverData;
			manaRecoverData.myInt = myManaRecover;
			if(myShouldRecoverManaQuickly == true)
			{
				manaRecoverData.myInt = myManaRecover * 1;
			}
			GetParent()->NotifyComponents(eComponentMessageType::eRestoreMana, manaRecoverData);
			myElapsedManaRegenTimer = 1.5f;
		}
		if (myMouseIsDown == true)
		{
			myPath.RemoveAll();

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
			CPath::SWaypoint point;
			point.myPosition = targetPosition3D;
			myPath.Add(point);

			
			CU::Vector2f targetPosition(targetPosition3D.x, targetPosition3D.z);

			TakeInputMessage(CU::eInputMessage::LEFTMOUSEBUTTON);
			eComponentMessageType type = eComponentMessageType::eSetPathPosition;
			SComponentMessageData pathdata;
			pathdata.myPathPointer = &myPath;
			
			GetParent()->NotifyComponents(type, pathdata);
			SComponentMessageData data;
			data.myVector2f = targetPosition;
			if (myIsShiftDown == false)
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

		if (myIsShiftDown == true)
		{
			SComponentMessageData stopData;
			stopData.myFloat = 0.1f;
			GetParent()->NotifyComponents(eComponentMessageType::eStopMovement, SComponentMessageData());
		}

		if (mySkillActivatorKeyDown >= 0)
		{
			eComponentMessageType type = eComponentMessageType::eTryToSelectSkill;
			SComponentMessageData data;
			data.myInt = mySkillActivatorKeyDown;
			GetParent()->NotifyComponents(type, data);
		}

		if (myIsRightMouseButtonDown == true)
		{
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
			eComponentMessageType type;
			SComponentMessageData data;
			data.myVector2f = targetPosition;

			type = eComponentMessageType::eSetSkillTargetPosition;
			GetParent()->NotifyComponents(type, data);

		}
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
		if(strcmp(aMessageData.myString,"BasicAttack") == 0)
		{
			for(unsigned short i = 0; i < mySkillInputMessageActivators.Size(); i++)
			{
				if(mySkillInputMessageActivators[i] == CU::eInputMessage::DIVIDE)
				{
					return;
					DL_PRINT("basicattack adding skipped");
				}
			}
			DL_PRINT("adding basicattack");
			mySkillInputMessageActivators.Add(CU::eInputMessage::DIVIDE);
		}
		else if (strcmp(aMessageData.myString, "WhirlWind") == 0)
		{
			for (unsigned short i = 0; i < mySkillInputMessageActivators.Size(); i++)
			{
				if (mySkillInputMessageActivators[i] == CU::eInputMessage::SPACE)
				{
					DL_PRINT("whirlWind adding skipped");
					return;
				}
			}
			DL_PRINT("adding whirlWind");
			PostMaster::GetInstance().SendLetter( eMessageType::eGameEventMessage, CGameEventMessageEvent({"Skill aquired: Whirlwind", "Activate with space"}));
			mySkillInputMessageActivators.Add(CU::eInputMessage::SPACE);
		}
		else if (strcmp(aMessageData.myString, "SweepAttack") == 0)
		{
			for (unsigned short i = 0; i < mySkillInputMessageActivators.Size(); i++)
			{
				if (mySkillInputMessageActivators[i] == CU::eInputMessage::RIGHTMOUSEBUTTON)
				{
					DL_PRINT("sweep attack adding skipped");
					return;
				}
			}
			DL_PRINT("adding sweep attack");
			PostMaster::GetInstance().SendLetter(eMessageType::eGameEventMessage, CGameEventMessageEvent({ "Skill aquired: Sweep", "Activate with right click" }));
			mySkillInputMessageActivators.Add(CU::eInputMessage::RIGHTMOUSEBUTTON);
		}
		else
		{
			std::cout << "Skill not found when adding key binding." << std::endl;
		}
		DL_PRINT("skill index Input %u", mySkillInputMessageActivators.Size());
	}
	else if (aMessageType == eComponentMessageType::eDied)
	{
		myIsActive = false;
	}
	else if (aMessageType == eComponentMessageType::eRespawned)
	{
		myIsActive = true;
	}
	else if (aMessageType == eComponentMessageType::ePercentMPLeft)
	{
		if(aMessageData.myUChar < 30)
		{
			myShouldRecoverManaQuickly = true;
		}
		else
		{
			myShouldRecoverManaQuickly = false;
		}
	}
}

void InputController::Destroy()
{
}

void InputController::RaycastOnNavmesh()
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
		myMousePosition = aMousePosition;
		TakeInputMessage(CU::eInputMessage::RIGHTMOUSEBUTTON);
		myIsRightMouseButtonDown = true;
		
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
		RaycastOnNavmesh();
	}
	if (aMouseButton == CU::eMouseButtons::RBUTTON)
	{
		myIsRightMouseButtonDown = false;
		for (unsigned short i = 0; i < mySkillInputMessageActivators.Size(); i++)
		{
			if (mySkillInputMessageActivators[i] == CU::eInputMessage::RIGHTMOUSEBUTTON)
			{
				mySkillActivatorKeyDown = -1;
			}
		}
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
			mySkillActivatorKeyDown = i;
			/*eComponentMessageType type = eComponentMessageType::eSelectSkill;
			SComponentMessageData data;
			data.myInt = i;
			GetParent()->NotifyComponents(type, data);*/
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
	if (aKey == CU::eKeys::SPACE)
	{
		eComponentMessageType type;

		type = eComponentMessageType::eStopWhirlwind;
		GetParent()->NotifyComponents(type, SComponentMessageData());
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
