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

	CU::Vector3f cameraPos = myPlayerCamera->GetTransformation().GetPosition();
	CU::Vector3f unitPos = GetParent()->GetWorldPosition();


	CU::Matrix44f cameraSpace = myPlayerCamera->GetProjection() * myPlayerCamera->GetInverse();

	unitPos = unitPos * cameraSpace;

	CU::Vector2f UnitPosNormalized = { unitPos.x / WINDOW_SIZE.x, -unitPos.y / WINDOW_SIZE.y };
	UnitPosNormalized += {0.5f, 0.5f};

	CU::Vector2f hpBarPos = UnitPosNormalized;

	CU::Vector2f displacementVector = { 0.025f, 0.1f };

	mySprite->SetPosition(hpBarPos - displacementVector);
	myBGSprite->SetPosition(mySprite->GetPosition());
}

void CHealthBarComponent::Render() // Create manager that renders everything, mebe bebe.
{
	if (mySprite->GetColor().a != 0.f)
	{
		mySprite->Render();
		myBGSprite->Render();
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
