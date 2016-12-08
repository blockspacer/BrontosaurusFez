#include "stdafx.h"
#include "PointLightInstance.h"
#include "LightManager.h"
#include "Engine.h"

CPointLightInstance::CPointLightInstance(ePointLights aType)
{
	myLight = LIGHTMANAGER->LoadLight(aType);
}

CPointLightInstance::~CPointLightInstance()
{
}

void CPointLightInstance::SetPosition(CU::Vector3f aPos)
{
	myPosition = aPos;
}

CU::Vector3f CPointLightInstance::GetPosition()
{
	return myPosition;
}


float CPointLightInstance::GetRange()
{
	return myLight->GetRange();
}

float CPointLightInstance::GetInstensity()
{
	return myLight->GetIntensity();
}

CU::Vector4f CPointLightInstance::GetColor()
{
	return myLight->GetColor();
}
