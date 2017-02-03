#pragma once
#include <vector2.h>

class CSpriteInstance;

class CClickPulse
{
	friend class CMouseComponent;

private:
	CClickPulse();
	~CClickPulse();

	void ActivateAtPos(CU::Vector2f aPos);
	void Update(); // early exit if inactive.

private:
	CSpriteInstance* mySprite;
	CU::Vector2f myPos;
	bool myIsActive;
};

