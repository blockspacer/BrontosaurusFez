#pragma once
#include "Event.h"

namespace CU
{
	enum class eKeys;
}

class KeyPressed : public Event
{
public:
	KeyPressed(const CU::eKeys& aKey);
	~KeyPressed();

	eMessageReturn DoEvent(CDebugInfoDrawer* aDebugInfoDrawer) const;
	eMessageReturn DoEvent(CreditsState*) const override;
	eMessageReturn DoEvent(CPlayState*) const override;
	eMessageReturn DoEvent(State*) const override;
	eMessageReturn DoEvent(PauseMenu*) const override;

private:
	const CU::eKeys& myKey;
};

