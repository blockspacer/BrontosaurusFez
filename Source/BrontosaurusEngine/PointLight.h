#pragma once
#include <vector4.h>

class CPointLight
{
public:
	CPointLight(const CU::Vector4f & aColor = {0.0f, 0.0f, 0.0f, 0.0f}, const float aRange = 0.0f, const float aIntensity = 0.0f);
	~CPointLight();
	inline CU::Vector4f& GetColor();
	inline float GetIntensity();
	inline float GetRange();


	inline void SetColor(const CU::Vector4f& aColor);
	inline void SetIntensity(const float aIntensity);
	inline void SetRange(const float aRange);
	
private:
	CU::Vector4f myColor;
	float myIntensity;
	float myRange;
};

inline CU::Vector4f & CPointLight::GetColor()
{
	return myColor;
}

inline float CPointLight::GetIntensity()
{
	return myIntensity;
}

inline float CPointLight::GetRange()
{
	return myRange;
}

inline void CPointLight::SetColor(const CU::Vector4f & aColor)
{
	myColor = aColor;
}

inline void CPointLight::SetIntensity(const float aIntensity)
{
	myIntensity = aIntensity;
}

inline void CPointLight::SetRange(const float aRange)
{
	myRange = aRange;
}
