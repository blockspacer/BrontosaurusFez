#pragma once
#include "Component.h"
class NavigationComponent : public CComponent
{
public:
	NavigationComponent();
	~NavigationComponent();
	void Update(float aDeltaTime);
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;

private:
	void CalculatePath(CU::Vector2f aTargetPosition);
	CU::GrowingArray<CU::Vector2f> myPath;
};

