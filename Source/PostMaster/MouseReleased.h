#pragma once
#include "Event.h"
#include "../CommonUtilities/vector2.h"

namespace CU
{
	enum class eMouseButtons;
}

class MouseReleased : public Event
{
public:
	MouseReleased(const CU::Vector2f& aMousePosition, const CU::eMouseButtons& aMouseButton);
	~MouseReleased();

	eMessageReturn DoEvent(GUI::GUIManager* aGUIManager) const override;
	eMessageReturn DoEvent(LevelSelectState* aLevelSelectState) const override;
	eMessageReturn DoEvent(InputController* aInputController) const override;
	eMessageReturn DoEvent(CMouseComponent* aMouseComponent) const override;
	eMessageReturn DoEvent(CDialogState* aDialogState)const override;
private:
	const CU::Vector2f myMousePosition;
	const CU::eMouseButtons& myMouseButton;
};

