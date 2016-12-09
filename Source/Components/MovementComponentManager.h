#pragma once
#include "../CommonUtilities/GrowingArray.h"
class MovementComponent;
class MovementComponentManager
{
public:
	static MovementComponentManager& GetInstance();
	static void CreateInstance();
	static void DestroyInstance();

	void RegisterComponent(MovementComponent* aComponent);
	void Update(const CU::Time& aDeltaTime);
	~MovementComponentManager();

private:
	MovementComponentManager();

	CU::GrowingArray<MovementComponent*> myComponents;

	static MovementComponentManager* ourInstance;
};

