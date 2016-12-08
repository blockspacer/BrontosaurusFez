#include "stdafx.h"
#include "MouseMoved.h"

#include "../GUI/GUIManager/GUIManager.h"
#include "../GUI/GUICursor/GUICursor.h"

#include "../BrontosaurusEngine/Engine.h"

MouseMoved::MouseMoved(const CU::Vector2f& aMousePosition)
	: myMousePosition(aMousePosition)
{
}

MouseMoved::~MouseMoved()
{
}


eMessageReturn MouseMoved::DoEvent(GUI::GUIManager* aGUIManager) const
{
	if (aGUIManager != nullptr)
	{
		CU::Vector2f windowSize(CEngine::GetInstance()->GetWindowSize());

		return aGUIManager->MouseMoved(CU::Vector2f(myMousePosition.x / windowSize.x, myMousePosition.y / windowSize.y)); //TODO: remove hardcoded window size
	}

	return eMessageReturn::eContinue;
}

eMessageReturn MouseMoved::DoEvent(GUICursor* aGUICursor) const
{
	if (aGUICursor != nullptr)
	{
		CU::Vector2f windowSize(CEngine::GetInstance()->GetWindowSize());

		const CU::Vector2f ScreenCenter(windowSize.x * 0.5f, windowSize.y * 0.5f); //TODO: remove hardcoded window size
		CU::Vector2f deltaPosition = myMousePosition - ScreenCenter;
		
		aGUICursor->SetPosition(aGUICursor->GetPosition() + CU::Vector2f(deltaPosition.x / windowSize.x, deltaPosition.y / windowSize.y)); //TODO: remove hardcoded window size
	}

	return eMessageReturn::eContinue;
}
