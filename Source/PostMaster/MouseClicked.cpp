#include "stdafx.h"
#include "MouseClicked.h"

#include "../GUI/GUIManager/GUIManager.h"
#include "Components\InputController.h"

MouseClicked::MouseClicked(const CU::Vector2f& aMousePosition, const CU::eMouseButtons& aMouseButton)
	: myMousePosition(aMousePosition)
	, myMouseButton(aMouseButton)
{
}

MouseClicked::~MouseClicked()
{
}

eMessageReturn MouseClicked::DoEvent(GUI::GUIManager* aGUIManager) const
{
	if (aGUIManager != nullptr)
	{
		return aGUIManager->MouseClicked(myMouseButton, CU::Vector2f(myMousePosition.x / 1920.f, myMousePosition.y / 1080.f));
	}

	return eMessageReturn::eContinue;
}

eMessageReturn MouseClicked::DoEvent(InputController* aInputController) const
{
	if (aInputController != nullptr)
	{
		return aInputController->MouseClicked(myMouseButton, CU::Vector2f(myMousePosition.x / 1920.f, myMousePosition.y / 1080.f));
	}

	return eMessageReturn::eContinue;
}