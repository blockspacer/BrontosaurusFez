#pragma once
#include "Event.h"

namespace CU
{
	enum class eKeys;
}

class KeyReleased : public Event
{
public:
	KeyReleased(const CU::eKeys& aKey);
	~KeyReleased();

	eMessageReturn DoEvent(CDebugInfoDrawer* aDebugInfoDrawer) const;
	eMessageReturn DoEvent(InputController* aDebugInfoDrawer) const;
private:
	const CU::eKeys& myKey;
};

