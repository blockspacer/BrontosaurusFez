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

	CU::GrowingArray<CInventoryComponent*> myComponents; //varf�r beh�ver jag inte forward declare'a growing-array, wtf?
										// �r allt i stdafx forward declare'at som default, �ven om man inte inkluderar n�got?
	static CInventoryComponentManager* ourInstance;
};

