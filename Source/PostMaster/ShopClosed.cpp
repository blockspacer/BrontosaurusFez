#include "stdafx.h"
#include "ShopClosed.h"
#include "Components/OpenShopListenerComponent.h"

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
