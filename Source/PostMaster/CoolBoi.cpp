#include "stdafx.h"
#include "CoolBoi.h"
#include "Components/MouseComponent.h"
#include "Components/GameObject.h"

CoolBoi::CoolBoi(CGameObject* aGameObject)
{
	myGameObject = aGameObject;
}


CoolBoi::~CoolBoi()
{
}

eMessageReturn CoolBoi::DoEvent(CMouseComponent *mouseComponent) const
{
	mouseComponent->RemoveHoveredGameObject(myGameObject);
	return eMessageReturn::eContinue;
}
