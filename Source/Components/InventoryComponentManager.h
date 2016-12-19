#pragma once

class CInventoryComponent;

class CInventoryComponentManager
{
public:
	static CInventoryComponentManager& GetInstance();
	static void DestroyInstance();

	void Update(const CU::Time & aDeltaTime);
	void RegisterComponent(CInventoryComponent* aComponent);

private:
	CInventoryComponentManager();
	~CInventoryComponentManager();

	CU::GrowingArray<CInventoryComponent*> myComponents; //varför behöver jag inte forward declare'a growing-array, wtf?
										// Är allt i stdafx forward declare'at som default, även om man inte inkluderar något?
	static CInventoryComponentManager* ourInstance;
};

