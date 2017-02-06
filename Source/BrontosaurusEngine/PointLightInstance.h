#pragma once
#include "Lights.h"

enum class ePointLights;
class CPointLight;

class CPointLightInstance
{
public:
	CPointLightInstance();
	~CPointLightInstance();

	void SetPosition(const CU::Vector3f& aPos);
	void SetRange(const float aRange);
	void SetInstensity(const float aIntencity);
	void SetColor(const CU::Vector3f & aColor);

	const CU::Vector3f& GetPosition();
	float GetRange();
	float GetInstensity();
	const CU::Vector3f& GetColor();

	const Lights::SPointLight& GetData();

	bool GetIsActive();
	void SetActive(const bool aIsActive);
private:
	//CU::Vector3f myPosition;
	//CPointLight* myLight;
	Lights::SPointLight myData;
	bool myIsActive;
};

