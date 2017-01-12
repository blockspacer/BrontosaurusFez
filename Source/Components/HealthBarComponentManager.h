#pragma once
#include "HealthBarComponent.h"

namespace CU { class Camera; }

class CHealthBarComponentManager
{
public:
	CHealthBarComponentManager(const CU::Camera& aCamera);
	~CHealthBarComponentManager();
	CHealthBarComponent* GetHealthBarAt(short aIndex);
	CHealthBarComponent* CreateHealthbar();
	void Update();
	void Render();
private:
	CU::GrowingArray<CHealthBarComponent*> myHealthbars;
	const CU::Camera& myPlayerCamera;
};

