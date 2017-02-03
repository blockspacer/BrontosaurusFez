#include "stdafx.h"
#include "ClickPulse.h"
#include "..\BrontosaurusEngine\SpriteInstance.h"

CClickPulse::CClickPulse()
{
	myIsActive = false;
	mySprite = new CSpriteInstance("Sprites/onClickPulse.dds",
	{ 0.04f,0.04f }, { 0.5f,0.05f });
}

CClickPulse::~CClickPulse()
{
	SAFE_DELETE(mySprite);
}

void CClickPulse::ActivateAtPos(CU::Vector2f aPos)
{
	myIsActive = true;
	myPos = aPos;
	mySprite->SetPosition({0.5f,0.5f});
	// mebe pos needs to be usin da camera magic from healthbar?
}

void CClickPulse::Update()
{
	if (myIsActive == true)
	{
		// Pulsate the sprite
		float alpha = mySprite->GetAlpha();
		alpha -= 0.01;
		mySprite->SetAlpha(alpha);

		//when done.
		if (alpha < 0.f)
		{
			myIsActive = false;
			mySprite->SetAlpha(1.f);
		}

		mySprite->Render();
	}
}
