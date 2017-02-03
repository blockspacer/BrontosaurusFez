#pragma once
#include "Component.h"
#include "Lights.h"
class PointLightComponent : public CComponent
{
public:
	PointLightComponent();
	~PointLightComponent();

	void SetColor(const CU::Vector3f& aColor);
	void SetRange(const float &aRange);
	void SetIntensity(const float & aIntensity);

	void Destroy() override;
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;

private:
	CU::Vector3f myColor;
	float myRange;
	float myIntensity;
	
};

