#include "stdafx.h"
#include "PointLightComponent.h"
#include "PointLightInstance.h"

PointLightComponent::PointLightComponent(CScene * aScene)
{
	myScene = aScene;
	myLightID = aScene->AddPointLightInstance(CPointLightInstance());
	myPointLightInstace = aScene->GetPointLightInstance(myLightID);
}

PointLightComponent::~PointLightComponent()
{
	Destroy();
}

void PointLightComponent::SetColor(const CU::Vector3f & aColor)
{
	myPointLightInstace->SetColor(aColor);
}

void PointLightComponent::SetRange(const float aRange)
{
	myPointLightInstace->SetRange(aRange);
}

void PointLightComponent::SetIntensity(const float aIntensity)
{
	myPointLightInstace->SetInstensity(aIntensity);
}

void PointLightComponent::Destroy()
{
	//myScene->RemovePointLightInstance(myLightID);
	//myLightID = -1;
}

void PointLightComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
	case eComponentMessageType::eObjectDone:
	case eComponentMessageType::eMoving:
	{
		myPointLightInstace->SetPosition(GetParent()->GetWorldPosition());
		break;
	}
	default:
		break;
	}
}
