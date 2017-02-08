#include "stdafx.h"
#include "PointLightComponent.h"
#include "PointLightInstance.h"

PointLightComponent::PointLightComponent(CScene * aScene)
{
	myScene = aScene;
	myLightID = aScene->AddPointLightInstance(CPointLightInstance());
	myPointLightInstace = aScene->GetPointLightInstance(myLightID);
	myType = eComponentType::ePointLight;
}

PointLightComponent::~PointLightComponent()
{
	Destroy();
}

void PointLightComponent::SetColor(const CU::Vector3f & aColor)
{
	myPointLightInstace->SetColor(aColor);
	myLastColor = aColor;
}

void PointLightComponent::SetRange(const float aRange)
{
	myPointLightInstace->SetRange(aRange);
	myLastRange = aRange;
}

void PointLightComponent::SetIntensity(const float aIntensity)
{
	myPointLightInstace->SetInstensity(aIntensity);
	myLastIntensity = aIntensity;
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

	case eComponentMessageType::eAddComponent:
		if (aMessageData.myComponentTypeAdded != eComponentType::ePointLight) break;
	case eComponentMessageType::eObjectDone:
	case eComponentMessageType::eMoving:
	{
		myPointLightInstace->SetPosition(GetParent()->GetWorldPosition());
		break;
	}
	case eComponentMessageType::eTurnOnThePointLight:
		myPointLightInstace->SetActive(true);
		break;
	case eComponentMessageType::eTurnOffThePointLight:
		myPointLightInstace->SetActive(false);
		break;
	case eComponentMessageType::eSetPointLightIntensity:
		myPointLightInstace->SetInstensity(aMessageData.myFloat);
		break;
	case eComponentMessageType::eSetPointLightRange:
		myPointLightInstace->SetRange(aMessageData.myFloat);
		break;
	case eComponentMessageType::eSetPointLightColor:
		myPointLightInstace->SetColor(aMessageData.myVector3f);
		break;
	case eComponentMessageType::eSetPointLightToLastState:
		myPointLightInstace->SetInstensity(myLastIntensity);
		myPointLightInstace->SetRange(myLastRange);
		myPointLightInstace->SetColor(myLastColor);
		break;
	default:
		break;
	}
}
