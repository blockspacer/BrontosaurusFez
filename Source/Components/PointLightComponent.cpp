#include "stdafx.h"
#include "PointLightComponent.h"


PointLightComponent::PointLightComponent()
{
}


PointLightComponent::~PointLightComponent()
{
}

void PointLightComponent::SetColor(const CU::Vector3f & aColor)
{
	myColor = aColor;
}

void PointLightComponent::SetRange(const float & aRange)
{
	myRange = aRange;
}

void PointLightComponent::SetIntensity(const float & aIntensity)
{
	myIntensity = aIntensity;
}

void PointLightComponent::Destroy()
{
}

void PointLightComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
}
