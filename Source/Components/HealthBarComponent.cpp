#include "stdafx.h"
#include "HealthBarComponent.h"

CHealthBarComponent::CHealthBarComponent()
{					
	myType = eComponentType::eHealthBar;									 // POS, get npc
	mySprite = new CSpriteInstance("Sprites/healthBar.dds", { 0.06f, 0.01f }, { 0.5f, 0.5f }, { 0.f, 0.f ,1.f, 1.f }, { 1.f, 0.f, 0.f, 0.f });
}

CHealthBarComponent::~CHealthBarComponent()
{
}

void CHealthBarComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData)
{
	switch (aMessageType)
	{
	case eComponentMessageType::eTakeDamage:
		UpdateSprite(aMessageData.myUChar); //msgData
		break;
	case eComponentMessageType::eDied:
		//Mark for deletion in Mgr.
		Destroy();
		break;
	}
}

void CHealthBarComponent::UpdateSprite(char aPercentHP)
{
	//Resize based on percentage HP left.
	mySprite->SetColor({ 1.f, 0.f, 0.f, 1.f }); // temp, just make it visible
}

void CHealthBarComponent::Update()
{
	CU::Vector2f hpBarPos = { GetParent()->GetWorldPosition().x, GetParent()->GetWorldPosition().z };    // Wattido
	mySprite->SetPosition(hpBarPos); // - CU::Vector2f(0.f, 0.07);
}

void CHealthBarComponent::Render() // Create manager that renders everything, mebe bebe.
{
	mySprite->Render();
}

void CHealthBarComponent::Destroy()
{
	mySprite->~CSpriteInstance();
	mySprite = nullptr;
}
