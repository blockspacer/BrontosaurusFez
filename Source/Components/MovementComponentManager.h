#pragma once

namespace CU
{
	class Time;
}

class MovementComponent;
class MovementComponentManager
{
public:
	static MovementComponentManager& GetInstance();
	static void CreateInstance();
	static void DestroyInstance();

	MovementComponent* CreateAndRegisterComponent();
	void Update(const CU::Time& aDeltaTime);
	~MovementComponentManager();

private:
	MovementComponentManager();

	CU::GrowingArray<MovementComponent*> myComponents;

	static MovementComponentManager* ourInstance;
};

