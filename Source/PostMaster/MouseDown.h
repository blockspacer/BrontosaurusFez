#pragma once
#include "Event.h"
#include "../CommonUtilities/vector2.h"

namespace CU
{
	enum class eMouseButtons;
}

class MouseDown : public Event
{
public:
	MouseDown(const CU::Vector2f& aMousePosition, const CU::eMouseButtons& aMouseButton);
	~MouseDown();

	eMessageReturn DoEvent(ControllerComponent* aControllerComponent) const;
	eMessageReturn DoEvent(InputController* aInputController) const;

private:
	const CU::Vector2f myMousePosition;
	const CU::eMouseButtons& myMouseButton;
};
