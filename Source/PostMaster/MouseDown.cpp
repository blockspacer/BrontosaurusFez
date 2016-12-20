#include "stdafx.h"
#include "MouseDown.h"
#include "Components\InputController.h"

MouseDown::MouseDown(const CU::Vector2f & aMousePosition, const CU::eMouseButtons & aMouseButton)
	: myMousePosition(aMousePosition)
	, myMouseButton(aMouseButton)
{
}

MouseDown::~MouseDown()
{
}

eMessageReturn MouseDown::DoEvent(ControllerComponent* aControllerComponent) const
{
	if (aControllerComponent != nullptr)
	{
	}

	return eMessageReturn::eContinue;
}

eMessageReturn MouseDown::DoEvent(InputController * aInputController) const
{
	if (aInputController != nullptr)
	{
		aInputController->MouseClicked(myMouseButton, myMousePosition);
		
	}

	return eMessageReturn::eContinue;
}
