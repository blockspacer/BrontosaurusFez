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

	eMessageReturn DoEvent(GUI::GUIManager* aGUIManager) const override;
	eMessageReturn DoEvent(InputController* aInputController) const override;
	eMessageReturn DoEvent(CMouseComponent* aMouseComponent) const override;

private:
	const CU::Vector2f myMousePosition;
	const CU::eMouseButtons& myMouseButton;
};
