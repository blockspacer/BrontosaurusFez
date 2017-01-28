#include "stdafx.h"
#include "GameObjectDiedEvent.h"
#include "Components\MouseComponent.h"

GameObjectDiedEvent::GameObjectDiedEvent(CGameObject* aGameObjectThatDied)
{
	myGameObjectThatDied = aGameObjectThatDied;
}


GameObjectDiedEvent::~GameObjectDiedEvent()
{
}

eMessageReturn GameObjectDiedEvent::DoEvent(CMouseComponent* aMouseComponent) const
{
	aMouseComponent->CheckIfHoveredGameObjectDied(myGameObjectThatDied);
	return eMessageReturn::eContinue;
}