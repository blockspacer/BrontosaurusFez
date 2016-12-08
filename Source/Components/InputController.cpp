#include "stdafx.h"
#include "InputController.h"
#include "../PostMaster/Message.h"
#include "../PostMaster/PostMaster.h"
#include "../PostMaster/Event.h"
#include "../CommonUtilities/EMouseButtons.h"
#include "GameObject.h"

InputController::InputController()
{
}


InputController::~InputController()
{
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
		CU::Vector2f convertedMousePosition = CU::Vector2f(aMousePosition.x * -1920.0f, aMousePosition.y * -1080.0f); // Ändra hårdkodningen memo!
		CU::Vector2f halfScreenPosition = CU::Vector2f(1920.0f / 2.0f, 1080.0f / 2.0f);
		CU::Vector2f playerPosition = CU::Vector2f(GetParent()->GetWorlPosition().x, GetParent()->GetWorlPosition().y); // Don't forget to add conversions between vector 2 and 3! like serisously it is góing to save us so much time.

		CU::Vector2f targetPosition = playerPosition + (convertedMousePosition + halfScreenPosition);
		eComponentMessageType type = eComponentMessageType::eSetNavigationTarget;
		SComponentMessageData data;
		data.myVector2 = targetPosition;
		GetParent()->NotifyComponents(type, data);
	}

	return eMessageReturn::eContinue;
}
