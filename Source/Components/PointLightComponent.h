#pragma once
#include "Component.h"
#include "Lights.h"
#include "Scene.h"

class PointLightComponent : public CComponent
{
public:
	PointLightComponent(CScene* aScene);
	~PointLightComponent();

	void SetColor(const CU::Vector3f& aColor);
	void SetRange(const float aRange);
	void SetIntensity(const float aIntensity);

	void Destroy() override;
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;

private:
	CScene* myScene;
	InstanceID myLightID;
	CPointLightInstance* myPointLightInstace;
	CU::Vector3f myLastColor;
	float myLastRange;
	float myLastIntensity;
};

