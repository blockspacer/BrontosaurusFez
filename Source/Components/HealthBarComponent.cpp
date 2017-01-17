#include "stdafx.h"
#include "HealthBarComponent.h"
#include "Camera.h"
#include "../BrontosaurusEngine/Engine.h"

CHealthBarComponent::CHealthBarComponent() : myBarSize(0.06f,0.01f)
{		
	myType = eComponentType::eHealthBar;
	mySprite = new CSpriteInstance("Sprites/healthBar.dds", myBarSize,   { 0.5f, 0.5f }, { 0.f, 0.f ,1.0f, 1.f }, { 1.f, 0.f, 0.f, 0.f });
	myBGSprite = new CSpriteInstance("Sprites/healthBar.dds", myBarSize, { 0.5f, 0.5f }, { 0.f, 0.f ,0.8f, 1.f }, { 0.1f, 0.1f, 0.1f, 0.f });
	myHasAppeared = false;
	myShouldBeDeleted = false;
}

CHealthBarComponent::~CHealthBarComponent()
{
	SAFE_DELETE(mySprite);
	SAFE_DELETE(myBGSprite);
}

void CHealthBarComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData)
{
	switch (aMessageType)
	{
	case eComponentMessageType::eDied:
		Destroy();
		break;
	case eComponentMessageType::ePercentHPLeft:
		UpdateSprite(aMessageData.myUChar); //msgData
		break;
	}
}

void CHealthBarComponent::UpdateSprite(char aPercentHP)
{
	if (myHasAppeared == false)
	{
		myBGSprite->SetColor({ 0.1f, 0.1f, 0.1f, 0.6f });
		mySprite->SetColor({ 1.f, 0.f, 0.f, 1.f });
		myHasAppeared = true;
	}


	CU::Vector2f size = myBarSize;
	size.x *= ((float)aPercentHP / 100.f);
	mySprite->SetSize(size);
}

void CHealthBarComponent::Update()
{
	CU::Vector4f objectPosition(GetParent()->GetToWorldTransform().GetPosition());	//GetInverse?
	const CU::Vector4f cameraSpacePosition = objectPosition * myPlayerCamera->GetInverse();
	const CU::Vector4f projectionSpaceCubePos = cameraSpacePosition *  myPlayerCamera->GetProjection();
	const CU::Vector4f position3D = projectionSpaceCubePos / projectionSpaceCubePos.w;
	const CU::Vector2f projectionSpacePos(position3D.x, position3D.y);

	CU::Vector2<float> temp;

	if (cameraSpacePosition.z <= 0)
	{
		temp = -projectionSpacePos;
		//if (myPosition.Length() < 1)
		{
			temp = temp.GetNormalized() * 0.95f;
		}
	}
	else
	{
		temp = projectionSpacePos;
		if (temp.Length() > 1)
		{
			temp = temp.GetNormalized() * 0.95f;
		}
	}

	if (temp.Length() > 1)
	{
		int apa = 1;
	}

	temp.x = CLAMP(temp.x, -1.f, 1.f);
	temp.y = CLAMP(temp.y, -1.f, 1.f);

	temp.x = temp.x / 2.0f + 0.5f;
	temp.y = -temp.y / 2.0f + 0.5f;

	temp -= CU::Vector2f(0.025f, 0.1f);

	mySprite->SetPosition(temp);
	myBGSprite->SetPosition(temp);
}

void CHealthBarComponent::Render() // Create manager that renders everything, mebe bebe.
{
	if (mySprite->GetColor().a != 0.f)
	{
		myBGSprite->Render();
		mySprite->Render();
	}
}

void CHealthBarComponent::Destroy()
{
	myShouldBeDeleted = true;
}

void CHealthBarComponent::SetCamera(const CU::Camera& aPlayerCamera)
{
	myPlayerCamera = &aPlayerCamera;
}
