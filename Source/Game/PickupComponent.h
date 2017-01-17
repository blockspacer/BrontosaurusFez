#pragma once
#include "../Components/Component.h"
#include "PickupType.h"

class CPickupComponent :public CComponent
{
public:
	friend class CPickupManager;

	void Destroy()override;

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData)override;
	void Update(float aDeltaTime);
private:
	CPickupComponent(CPickupManager& myManager);
	~CPickupComponent();

	CPickupManager& myManager;
	float mySpeed;
	float mySuckUpRadius;
};

