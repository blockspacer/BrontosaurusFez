#pragma once
#include "../Components/Component.h"

class CPickerUpperComponent: public CComponent
{
public:
	friend class CPickupManager;

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData)override;
	void Destroy()override;
private:
	CPickerUpperComponent(CPickupManager& aManager);
	~CPickerUpperComponent();

	CPickupManager& myManager;
};

