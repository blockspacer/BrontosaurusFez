#include "stdafx.h"
#include "ShopClosed.h"
#include "Components/OpenShopListenerComponent.h"
#include "Components/MouseComponent.h"

ShopClosed::ShopClosed()
{
}


ShopClosed::~ShopClosed()
{
}

eMessageReturn ShopClosed::DoEvent(COpenShopListenerComponent* aListener) const
{
	aListener->Reactivate();
	return eMessageReturn::eContinue;
}

eMessageReturn ShopClosed::DoEvent(CMouseComponent* aMouseComponent) const
{
	aMouseComponent->RemoveHoveredObjects();
	return eMessageReturn::eContinue;
}
