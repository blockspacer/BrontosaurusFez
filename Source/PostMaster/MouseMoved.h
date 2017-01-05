#pragma once
#include "Event.h"
#include "../CommonUtilities/vector2.h"

class MouseMoved : public Event
{
public:
	MouseMoved(const CU::Vector2f& aMousePosition);
	~MouseMoved();

	eMessageReturn DoEvent(GUI::GUIManager*) const override;
	eMessageReturn DoEvent(GUICursor*) const override;
	eMessageReturn DoEvent(InputController* aInputController) const;

private:
	CU::Vector2f myMousePosition;
};
