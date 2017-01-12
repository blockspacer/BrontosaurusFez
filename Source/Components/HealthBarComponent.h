#pragma once
#include "Component.h"
#include "..\BrontosaurusEngine\SpriteInstance.h"

namespace CU { class Camera; }

class CHealthBarComponent : public CComponent
{
	friend class CHealthBarComponentManager;
	//Because fuck public

private:
	CHealthBarComponent();
	~CHealthBarComponent();
	void UpdateSprite(char aPercentHP);
	void Update();
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Render();
	void Destroy();
	void SetCamera(const CU::Camera& aPlayerCamera);
	const CU::Camera* myPlayerCamera;
	CSpriteInstance* mySprite;
	CSpriteInstance* myBGSprite;
	bool myHasAppeared;
};
