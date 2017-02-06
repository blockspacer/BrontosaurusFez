#pragma once
#include <vector2.h>
#include <vector3.h>

namespace CU
{
	class Camera;
}

class CSpriteInstance;

class CClickPulse
{
	friend class CMouseComponent;

private:
	CClickPulse(const CU::Camera& aCamera);
	~CClickPulse();

	void ActivateAtPos(CU::Vector3f aPos);
	void Update(const CU::Time& aDeltaTime); // early exit if inactive.

private:
	CSpriteInstance* mySprite;
	const CU::Camera* myCamera;
	CU::Vector2f myOriginalSize;
	CU::Vector2f mySize;
	float myAlpha;
	bool myIsActive;
	bool myFadingOut;
};

