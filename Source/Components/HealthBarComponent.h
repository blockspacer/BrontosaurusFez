#pragma once
#include "Component.h"
#include "..\BrontosaurusEngine\SpriteInstance.h"

class CHealthBarComponent : public CComponent
{
	friend class CHealthBarComponentManager;
public:
	void Update();

private:
	CHealthBarComponent();
	~CHealthBarComponent();
	void UpdateSprite(char aPercentHP);
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Render();
	void Destroy();
	CSpriteInstance* mySprite;
};
