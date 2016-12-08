#include "stdafx.h"
#include "PointLight.h"


CPointLight::CPointLight(const CU::Vector4f& aColor, const float aRange, const float aIntensity)
{
	myColor = aColor;
	myIntensity = aIntensity;
	myRange = aRange;
}


CPointLight::~CPointLight()
{
}
