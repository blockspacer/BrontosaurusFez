#include "stdafx.h"
#include "MouseComponent.h"
#include "..\BrontosaurusEngine\Engine.h"
#include "..\CommonUtilities\Camera.h"
#include "..\Collision\ICollider.h"
#include "..\Game\PollingStation.h"

#include "..\Game\ClickPulse.h"

CMouseComponent::CMouseComponent(const CU::Camera& aPlayerCamera)
	: myPlayerCamera(aPlayerCamera)
	, myHoveredGameObjects(10)
	, myMouseIsDown(false)
{
	myMousePosition = CU::Vector3f::Zero;
	myType = eComponentType::eMouse;
	myClickPulse = new CClickPulse(myPlayerCamera);
}

CMouseComponent::~CMouseComponent()
{
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eMouseMessage);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eGameObjectDied);
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eShopClosed);
}

void CMouseComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
	case eComponentMessageType::eObjectDone:
		InternalInit();
		break;
	case eComponentMessageType::eOnCollisionEnter:
	{
		HandleCollision(aMessageData.myGameObject);
		SComponentMessageData data; data.myFloat = 0.5f;
		break;
	}
	case eComponentMessageType::eOnCollisionExit:
		for(unsigned short i = 0; i < myHoveredGameObjects.Size(); i++)
		{
			if(myHoveredGameObjects[i] == aMessageData.myGameObject)
			{
				myHoveredGameObjects.RemoveAtIndex(i);
			}
		}
		SComponentMessageData data; data.myFloat = 0.f;
		aMessageData.myGameObject->NotifyComponents(eComponentMessageType::eSetHighLight, data);
		break;
	}
}

void CMouseComponent::Destroy()
{
	DL_ASSERT("NOT IMPLEMENTED DESTROY");
}

void CMouseComponent::MouseMoved(const CU::Vector2f& aMousePosition)
{
	myMouseScreenPosition = aMousePosition;

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
			targetPosition3D.y = PollingStation::playerObject->GetWorldPosition().y;
		}
	}
	//for evaluating distance to things
	myMousePosition = targetPosition3D;
	//-------------------------------------
	GetParent()->GetLocalTransform().SetPosition(targetPosition3D);
	GetParent()->NotifyComponents(eComponentMessageType::eMoving, SComponentMessageData());
}

void CMouseComponent::Update(const CU::Time& aDeltaTime)
{
	if (myHoveredGameObjects.Size() > 0)
	{
		HighlightClosest();
		if (myMouseIsDown == true)
		{
			CGameObject* targetObject = nullptr;
			float lowestDistance = 99999999.0f;
			for (unsigned int i = 0; i < myHoveredGameObjects.Size(); ++i)
			{
				CU::Vector3f distance;
				CU::Vector3f objectposition = myHoveredGameObjects[i]->GetWorldPosition();

				distance = objectposition - myMousePosition;

				float dis = distance.Length();

				if (dis < lowestDistance)
				{
					lowestDistance = dis;
					targetObject = myHoveredGameObjects[i];
				}
			}
			assert(targetObject != nullptr && "Finding the closest CGameObject to the mouse position did not work :(");
			DL_PRINT("clicked on enemy: %s", targetObject->GetName().c_str());

			SComponentMessageData hitThisBastard;
			hitThisBastard.myGameObject = targetObject;
			PollingStation::playerObject->NotifyComponents(eComponentMessageType::eSetSkillTargetObject, hitThisBastard);

		}
	}
	myClickPulse->Update(aDeltaTime);
}

eMessageReturn CMouseComponent::Recieve(const Message& aMessage)
{
	return aMessage.myEvent.DoEvent(this);
}

void CMouseComponent::InternalInit()
{
	PostMaster::GetInstance().Subscribe(this, eMessageType::eMouseMessage);
	PostMaster::GetInstance().Subscribe(this, eMessageType::eGameObjectDied);
	PostMaster::GetInstance().Subscribe(this, eMessageType::eShopClosed);
}

void CMouseComponent::HighlightClosest()
{
	CGameObject* gameObject = nullptr;
	float lowestDistance = 99999999.0f;
	for (unsigned int i = 0; i < myHoveredGameObjects.Size(); ++i)
	{
		SComponentMessageData data;
		data.myFloat = 0.0f;
		myHoveredGameObjects[i]->NotifyComponents(eComponentMessageType::eSetHighLight, data);

		CU::Vector3f distance;
		CU::Vector3f objectposition = myHoveredGameObjects[i]->GetWorldPosition();

		distance = objectposition - myMousePosition;

		float dis = distance.Length();

		if (dis < lowestDistance)
		{
			lowestDistance = distance.Length();
			gameObject = myHoveredGameObjects[i];
		}
	}

	if (gameObject == nullptr)
	{
		return;
	}

	SComponentMessageData data;
	data.myFloat = 0.5f;
	if (PollingStation::playerObject != gameObject)
	{
		gameObject->NotifyComponents(eComponentMessageType::eSetHighLight, data);
	}
}

void CMouseComponent::HandleCollision(CGameObject* aCollidedWith)
{
	if (aCollidedWith == PollingStation::playerObject)
	{
		return;
	}
	if (aCollidedWith == nullptr)
	{
		return;
	}

	if (myHoveredGameObjects.Find(aCollidedWith) == myHoveredGameObjects.FoundNone)
	{
		myHoveredGameObjects.Add(aCollidedWith);
	}
}

void CMouseComponent::SetMouseIsDown(const bool aIsDown)
{
	myMouseIsDown = aIsDown;

	if (myMouseIsDown == true)
	{
		// Pulsify!
		// First, update position bc camera has probably moved!
		MouseMoved(myMouseScreenPosition);
		myClickPulse->ActivateAtPos(myMousePosition);

		if(myHoveredGameObjects.Size() > 0)
		{
			HandleCollision(myHoveredGameObjects[0]);
		}
	}
	else
	{
	}
}

void CMouseComponent::CheckIfHoveredGameObjectDied(CGameObject * aGameobjectThatDied)
{
	for (unsigned short i = 0; i < myHoveredGameObjects.Size(); i++)
	{
		if (myHoveredGameObjects[i] == aGameobjectThatDied)
		{
			myHoveredGameObjects.RemoveAtIndex(i);
		}
	}
}

void CMouseComponent::RemoveHoveredObjects()
{
	myHoveredGameObjects.RemoveAll();
}
