#pragma once
#include "Component.h"
#include "..\BrontosaurusEngine\SpriteInstance.h"

class CHealthBarComponent : public CComponent
{
	friend class CHealthBarComponentManager;
public:
	void UpdateSprite(CU::Vector2f aPos ,char aPercentHP);

private:
	CHealthBarComponent();
	~CHealthBarComponent();
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Render();
	void Destroy();
	CSpriteInstance* mySprite;
};
