#pragma once
#include "../CommonUtilities/vector3.h"
#include "../CommonUtilities/vector4.h"

enum class ePointLights;
class CPointLight;

class CPointLightInstance
{
public:
	CPointLightInstance(ePointLights aType);
	~CPointLightInstance();

	void SetPosition(CU::Vector3f aPos);

	CU::Vector3f GetPosition();

	float GetRange();
	float GetInstensity();
	CU::Vector4f GetColor();


private:
	CU::Vector3f myPosition;
	CPointLight* myLight;

};

