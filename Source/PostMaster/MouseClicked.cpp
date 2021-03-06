#include "stdafx.h"
#include "MouseClicked.h"

#include "../GUI/GUIManager.h"
#include "Components\InputController.h"
#include "BrontosaurusEngine/Engine.h"
#include "Components/MouseComponent.h"
#include "CommonUtilities/EMouseButtons.h"
#include "Game/DialogState.h"
#include "Game/SplashScreen.h"

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
		return aGUIManager->MouseClicked(myMouseButton, CU::Vector2f(myMousePosition.x / WINDOW_SIZE_F.x, myMousePosition.y / WINDOW_SIZE_F.y));
	}

	return eMessageReturn::eContinue;
}

eMessageReturn MouseClicked::DoEvent(InputController* aInputController) const
{
	if (aInputController != nullptr)
	{
		return aInputController->MouseClicked(myMouseButton, CU::Vector2f(myMousePosition.x, myMousePosition.y));
	}

	return eMessageReturn::eContinue;
}

eMessageReturn MouseClicked::DoEvent(CMouseComponent* aMouseComponent) const
{
	if (aMouseComponent != nullptr)
	{
		if(myMouseButton == CU::eMouseButtons::LBUTTON)
		{
			aMouseComponent->SetMouseIsDown(true);	
		}
	}

	return eMessageReturn::eContinue;
}

eMessageReturn MouseClicked::DoEvent(CDialogState* aDialogState) const
{
	if (aDialogState)
	{
		aDialogState->Next();
		return eMessageReturn::eStop;
	}

	return eMessageReturn::eContinue;
}

eMessageReturn MouseClicked::DoEvent(CSplashScreen* aSplashScreen) const
{
	if (aSplashScreen)
	{
		aSplashScreen->UserWantsToContinue();
	}

	return eMessageReturn::eContinue;
}
