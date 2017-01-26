#pragma once
#include "Event.h"
class ShopClosed : public Event
{
public:
	ShopClosed();
	~ShopClosed();

	eMessageReturn DoEvent(COpenShopListenerComponent*) const override;
private:
};

