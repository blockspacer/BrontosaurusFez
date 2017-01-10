#pragma once
#include "HealthBarComponent.h"

class CHealthBarComponentManager
{
public:
	CHealthBarComponentManager();
	~CHealthBarComponentManager();
	CHealthBarComponent* GetHealthBarAt(short aIndex);
	CHealthBarComponent* CreateHealthbar();
	void Render();
private:
	CU::GrowingArray<CHealthBarComponent*> myHealthbars;
};

