#include "stdafx.h"
#include "InputController.h"
#include "../PostMaster/Message.h"
#include "../PostMaster/PostMaster.h"
#include "../PostMaster/Event.h"
#include "../CommonUtilities/EMouseButtons.h"
#include "GameObject.h"
#include <iostream>
#include "../BrontosaurusEngine/Engine.h"
#include "CameraManager.h"
#include "../CommonUtilities/Camera.h"

InputController::InputController()
{
	PostMaster::GetInstance().AppendSubscriber(this, eMessageType::eMouseMessage);
}


InputController::~InputController()
{
	PostMaster::GetInstance().UnSubscribe(this, eMessageType::eMouseMessage);
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
		CU::Vector2f convertedMousePosition = CU::Vector2f(aMousePosition.x * ENGINE->GetWindowSize().x, aMousePosition.y * ENGINE->GetWindowSize().y); // Ändra hårdkodningen memo!
		std::cout <<  "Mouse Pos:" <<"X:" << convertedMousePosition.x << " Y: " << convertedMousePosition.y << std::endl;
		CU::Vector2f halfScreenPosition = CU::Vector2f(ENGINE->GetWindowSize().x / 2.0f, ENGINE->GetWindowSize().y / 2.0f);
		CU::Vector2f playerPosition = CU::Vector2f(GetParent()->GetWorlPosition().x, GetParent()->GetWorlPosition().z); // Don't forget to add conversions between vector 2 and 3! like serisously it is góing to save us so much time.
		std::cout << "Player Pos:" << "X:" << playerPosition.x << " Y: " << GetParent()->GetWorlPosition().z << std::endl;
		std::cout << "Difference Pos:" << "X:" << (convertedMousePosition - halfScreenPosition).x << " Y: " << (convertedMousePosition - halfScreenPosition).y << std::endl;
		CU::Matrix44f newTargetMatrix;
		newTargetMatrix.SetPosition(CU::Vector3f(playerPosition.x, playerPosition.y, 0.0f));
		CU::Matrix33f cameraRotation = CAMERA->GetTransformation();
		//newTargetMatrix.SetRotation(cameraRotation);
		CU::Vector3f movement(convertedMousePosition.x - halfScreenPosition.x, convertedMousePosition.y - halfScreenPosition.y, 0.0f);
		newTargetMatrix.Move(movement);
		CU::Vector2f targetPosition = CU::Vector2f(newTargetMatrix.GetPosition().x, newTargetMatrix.GetPosition().y);
	
		targetPosition.y *= -1;
		std::cout << "Target Pos:" << "X:" << targetPosition.x << " Y: " << targetPosition.y << std::endl;
		std::cout << "" << std::endl;
		eComponentMessageType type = eComponentMessageType::eSetNavigationTarget;
		SComponentMessageData data;
		data.myVector2 = targetPosition;
		//data.myVector2 = playerPosition + CU::Vector2f(100.0f, 100.0f);
		GetParent()->NotifyComponents(type, data);
	}

	return eMessageReturn::eContinue;
}
