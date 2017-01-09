#include "stdafx.h"
#include "MouseReleased.h"

#include "../GUI/GUIManager/GUIManager.h"
#include "Game/LevelSelectState.h"
#include "Components/InputController.h"
#include "Components/MouseComponent.h"

MouseReleased::MouseReleased(const CU::Vector2f& aMousePosition, const CU::eMouseButtons& aMouseButton)
	: myMousePosition(aMousePosition)
	, myMouseButton(aMouseButton)
{
}

MouseReleased::~MouseReleased()
{
}

eMessageReturn MouseReleased::DoEvent(GUI::GUIManager* aGUIManager) const
{
	if (aGUIManager != nullptr)
	{
		return aGUIManager->MouseReleased(myMouseButton, CU::Vector2f(myMousePosition.x / 1920.f, myMousePosition.y / 1080.f));
	}

	return eMessageReturn::eContinue;
}

eMessageReturn MouseReleased::DoEvent(LevelSelectState* aLevelSelectState) const
{
	if (aLevelSelectState != nullptr)
	{
		aLevelSelectState->SetStateStatus(State::eStatus::ePop);
	}

	return eMessageReturn::eContinue;
}

eMessageReturn MouseReleased::DoEvent(InputController* aInputController) const
{
	if (aInputController != nullptr)
	{
		aInputController->MouseReleased(myMouseButton, myMousePosition);
	}

	return eMessageReturn::eContinue;
}

eMessageReturn MouseReleased::DoEvent(CMouseComponent* aMouseComponent) const
{
	if (aMouseComponent != nullptr)
	{
		aMouseComponent->SetMouseIsDown(false);
	}

	return eMessageReturn::eContinue;
}
