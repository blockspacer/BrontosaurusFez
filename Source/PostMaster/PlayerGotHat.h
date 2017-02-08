#pragma once
#include "Event.h"

struct SHatBluePrint;

class CPlayerGotHat : public Event
{
public:
	CPlayerGotHat(SHatBluePrint* aHatBluePrint);
	~CPlayerGotHat();

	eMessageReturn DoEvent(GUI::GUIManager* aGUIManager) const override;
	eMessageReturn DoEvent(GUI::HatContainer* aGUIHatContainer) const override;

private:
	SHatBluePrint* myHatBluePrint;
};
