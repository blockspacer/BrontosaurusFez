#pragma once
#include "Event.h"
class CGameObject;
class GameObjectDiedEvent : public Event
{
public:
	GameObjectDiedEvent(CGameObject* aGameObjectThatDied);
	~GameObjectDiedEvent();

	eMessageReturn DoEvent(CMouseComponent* aMouseComponent) const override;

private:
	CGameObject* myGameObjectThatDied;
};

