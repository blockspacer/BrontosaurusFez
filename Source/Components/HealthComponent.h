#pragma once
#include "Component.h"

typedef int HealthPoint;

class CHealthComponent : public CComponent
{
public:
	CHealthComponent();
	~CHealthComponent();
	const float GetPrecentLeft();

	void Update(const CU::Time& aDeltaTime);
	void SetHealth(const HealthPoint aValue);
	void SetMaxHealth(const HealthPoint aValue);

	void Destroy() override;
private:
	float myPercentageLeft;
	HealthPoint myHealth;
	HealthPoint myMaxHealth;
};

