#pragma once

class CInventoryComponent;

class CInventoryComponentManager
{
public:
	static CInventoryComponentManager& GetInstance();
	static void DestroyInstance();

	void Update(const CU::Time & aDeltaTime);
	CInventoryComponent& CreateAndRegisterComponent();

private:
	CInventoryComponentManager();
	~CInventoryComponentManager();

	CU::GrowingArray<CInventoryComponent*> myComponents;

	static CInventoryComponentManager* ourInstance;
};

