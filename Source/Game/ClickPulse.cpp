#include "stdafx.h"
#include "ClickPulse.h"
#include "..\BrontosaurusEngine\SpriteInstance.h"
#include <Engine.h>
#include <Renderer.h>
#include <Camera.h>
#include <CUTime.h>

#include "..\CommonUtilities\CommonUtilities.h"

CClickPulse::CClickPulse(const CU::Camera& aPlayerCamera) : myCamera(&aPlayerCamera)
{
	myIsActive = false;
	myFadingOut = true;
	mySprite = new CSpriteInstance("Sprites/onClickPulse.dds",
	{ 0.03f,0.03f }, { 0.5f,0.05f });
	myAlpha = 0.0f;
	myOriginalSize = mySize = mySprite->GetSize();
	//myCamera = const_cast<CU::Camera*>(&RENDERER.GetCamera()); //GUI camera
}

CClickPulse::~CClickPulse()
{
	SAFE_DELETE(mySprite);
}

void CClickPulse::ActivateAtPos(CU::Vector3f aPos)
{
	myIsActive = true;
	myFadingOut = false;
	myAlpha = 0.f;
	mySize = myOriginalSize;

	CU::Vector4f objectPosition(aPos);
	const CU::Vector4f cameraSpacePosition = objectPosition * myCamera->GetInverse();
	const CU::Vector4f projectionSpaceCubePos = cameraSpacePosition * myCamera->GetProjection();
	const CU::Vector4f position3D = projectionSpaceCubePos / projectionSpaceCubePos.w;
	const CU::Vector2f projectionSpacePos(position3D.x, position3D.y);

	CU::Vector2f temp = projectionSpacePos;
	temp.x = temp.x / 2.0f + 0.5f;
	temp.y = -temp.y / 2.0f + 0.5f;
	temp -= CU::Vector2f(0.01f, 0.01f);

	mySprite->SetPosition(temp);
	mySprite->SetPivot({ 0.5f, 0.5f });
}

void CClickPulse::Update(const CU::Time& aDeltaTime)
{
	if (myIsActive == true)
	{
		if (myFadingOut == true)
		{
			myAlpha -= 3.f * aDeltaTime.GetSeconds();

			mySprite->Render();

			if (myAlpha < 0.f)
			{
				myIsActive = false;
				myFadingOut = false;
				myAlpha = 1.f;
				mySize = myOriginalSize;
			}
		}
		else
		{
			myAlpha += 3.f * aDeltaTime.GetSeconds();

			mySprite->Render();

			if (myAlpha > 0.5f)
				myFadingOut = true;
		}

		mySize.x += 0.05f * aDeltaTime.GetSeconds();
		mySize.y += 0.05f * aDeltaTime.GetSeconds();
		mySprite->SetSize(mySize);


		mySprite->SetAlpha(myAlpha);
	}
}
