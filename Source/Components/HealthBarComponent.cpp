#include "stdafx.h"
#include "HealthBarComponent.h"

CHealthBarComponent::CHealthBarComponent()
{																		 // POS, get npc
	mySprite = new CSpriteInstance("Sprites/healthBar.dds", { 0.06f, 0.01f }, { 0.5f, 0.5f }, { 0.f, 0.f ,1.f, 1.f }, { 1.f, 0.f, 0.f, 0.f });
}

CHealthBarComponent::~CHealthBarComponent()
{
}

void CHealthBarComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType)
	{
	case eComponentMessageType::eTakeDamage: // took ?
		UpdateSprite({0.5f, 0.5f}, 100); // message data entity pos
		break;
	}
}

void CHealthBarComponent::UpdateSprite(CU::Vector2f aPos, char aPercentHP)
{
	aPercentHP;
	//Resize and recolour based on percentage HP left.
	mySprite->SetPosition(aPos);
	mySprite->SetColor({ 1.f, 0.f, 0.f, 1.f }); // temp, just make it visible
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
