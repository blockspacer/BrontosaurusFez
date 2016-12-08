#pragma once
#include "Event.h"
#include "../CommonUtilities/vector2.h"

namespace CU
{
	enum class eMouseButtons;
}

class MouseClicked : public Event
{
public:
	MouseClicked(const CU::Vector2f& aMousePosition, const CU::eMouseButtons& aMouseButton);
	~MouseClicked();

	eMessageReturn DoEvent(GUI::GUIManager* aGUIManager) const;
	eMessageReturn DoEvent(InputController* aInputController) const;
private:
	const CU::Vector2f myMousePosition;
	const CU::eMouseButtons& myMouseButton;
};
