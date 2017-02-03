#include "stdafx.h"
#include "MouseClicked.h"

#include "../GUI/GUIManager.h"
#include "Components\InputController.h"
#include "BrontosaurusEngine/Engine.h"
#include "Components/MouseComponent.h"
#include "Game/DialogState.h"

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
		aMouseComponent->SetMouseIsDown(true);
	}

	return eMessageReturn::eContinue;
}

eMessageReturn MouseClicked::DoEvent(CDialogState* aDialogState) const
{
	aDialogState->Next();

	return eMessageReturn::eStop;
}
