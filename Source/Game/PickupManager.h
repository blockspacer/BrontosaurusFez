#pragma once
#include "DynamicString.h"
#include "CommonUtilities\GrowingArray.h"
#include <unordered_map>
enum class ePickupType;
class CPickupComponent;
class CPickerUpperComponent;

struct SPickupData
{
	int myValue;
	const char* myString;
	ePickupType myType;
};

namespace CU
{
	class Time;
}

class CPickupManager
{
public:
	static void CreateInstance();
	static CPickupManager& GetInstance();
	static void DestroyInstance();

	CPickupComponent* CreatePickupComponent(const ePickupType& aPickupType, const int aValue, const float aSuckUpSpeed, const float aSuckUpRadius);
	CPickupComponent* CreatePickupComponent(const ePickupType& aPickupType, const char* aString, const float aSuckUpSpeed, const float aSuckUpRadius);
	CPickupComponent* CreatePickupComponent(const CU::DynamicString& aString, const int aValue, const float aSuckUpSpeed, const float aSuckUpRadius);

	void DestroyPickupComp(CPickupComponent* aPickupComponent);
	SPickupData GetPickupData(CPickupComponent* aPickupComponent);

	CPickerUpperComponent* CreatePickerUpperComp();
	void DestroyPickerUpperComp(CPickerUpperComponent* aPickerUpperComponent);

	void Update(const CU::Time& aDeltaTime);
private:
	CPickupManager();
	~CPickupManager();

	std::unordered_map<CPickupComponent*, SPickupData> myPickupData;
	CU::GrowingArray<CPickerUpperComponent*> myCreatedPickerUppers;

	CU::GrowingArray<CPickupComponent*> myPickUpComponents;
	static CPickupManager* ourInstance;
};

