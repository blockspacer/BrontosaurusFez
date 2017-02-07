#pragma once
#include "Event.h"

class CMouseComponent;
class CGameObject;

class CoolBoi : public Event
{
public:
	CoolBoi(CGameObject* aGameObject);
	~CoolBoi();
private:
	CGameObject* myGameObject;

	eMessageReturn DoEvent(CMouseComponent*) const override;
};

