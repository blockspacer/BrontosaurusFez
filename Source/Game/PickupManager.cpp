#include "stdafx.h"
#include "PickupManager.h"
#include "PickupComponent.h"
#include "../Components/ComponentManager.h"
#include "PickerUpperComponent.h"
#include "CommonUtilities\CUTime.h"
#include "../Components/ComponentMessage.h"
CPickupManager* CPickupManager::ourInstance = nullptr;

void CPickupManager::CreateInstance()
{
	if (ourInstance == nullptr)
	{
		ourInstance = new CPickupManager();
	}
	else
	{
		DL_ASSERT("Pickupmanager already created");
	}
}

CPickupManager& CPickupManager::GetInstance()
{
	if (ourInstance == nullptr)
	{
		DL_ASSERT("Pickup Manager instance not created");
	}

	return *ourInstance;
}

void CPickupManager::DestroyInstance()
{
	assert(ourInstance != nullptr && "pickup manager not created (is NULL)");
	
	SAFE_DELETE(ourInstance);
}

CPickupComponent* CPickupManager::CreatePickupComponent(const ePickupType& aPickupType, const int aValue, const float aSuckUpSpeed, const float aSuckUpRadius)
{
	CPickupComponent* newComponent = new CPickupComponent(*this);

	SPickupData someData;
	someData.myValue = aValue;
	someData.myType = aPickupType;

	newComponent->mySuckUpRadius = aSuckUpRadius;
	newComponent->mySpeed = aSuckUpSpeed;

	myPickupData[newComponent] = someData;

	CComponentManager::GetInstance().RegisterComponent(newComponent);
	myPickUpComponents.Add(newComponent);
	return newComponent;
}

CPickupComponent * CPickupManager::CreatePickupComponent(const ePickupType & aPickupType, const char * aString, const float aSuckUpSpeed, const float aSuckUpRadius)
{
	CPickupComponent* newComponent = new CPickupComponent(*this);

	SPickupData someData;
	someData.myString = aString;
	someData.myType = aPickupType;

	newComponent->mySuckUpRadius = aSuckUpRadius;
	newComponent->mySpeed = aSuckUpSpeed;

	myPickupData[newComponent] = someData;

	CComponentManager::GetInstance().RegisterComponent(newComponent);
	myPickUpComponents.Add(newComponent);
	return newComponent;
}

CPickupComponent* CPickupManager::CreatePickupComponent(const CU::DynamicString& aString, const int aValue, const float aSuckUpSpeed, const float aSuckUpRadius)
{
	ePickupType type = ePickupType::HEALTH;

	if (aString == "mana")
	{
		type = ePickupType::MANA;
	}
	else if(aString == "health")
	{
		type = ePickupType::HEALTH;
	}
	else if (aString == "gold")
	{
		type = ePickupType::GOLD;
	}
	else
	{
		DL_ASSERT((CU::DynamicString("Pickup type ") + aString + " does not exists").c_str());
	}

	return CreatePickupComponent(type, aValue, aSuckUpSpeed, aSuckUpRadius);
}

void CPickupManager::DestroyPickupComp(CPickupComponent* aPickupComponent)
{
	myPickupData.erase(aPickupComponent);

	//CComponentManager::GetInstance().RemoveComponent(aPickupComponent->GetId());

	//delete aPickupComponent;
}

SPickupData CPickupManager::GetPickupData(CPickupComponent* aPickupComponent)
{
	if (myPickupData.count(aPickupComponent) == 0)
	{
		DL_ASSERT("Can't get pickup data with that pointer either it's not created properly, destroyed or not a pickup component");
	}

	return myPickupData[aPickupComponent];
}

CPickerUpperComponent* CPickupManager::CreatePickerUpperComp()
{
	CPickerUpperComponent* newComponent = new CPickerUpperComponent(*this);
	CComponentManager::GetInstance().RegisterComponent(newComponent);

	myCreatedPickerUppers.Add(newComponent);

	return  newComponent;
}

void CPickupManager::DestroyPickerUpperComp(CPickerUpperComponent* aPickerUpperComponent)
{
	//CComponentManager::GetInstance().DeleteComponent(aPickerUpperComponent->GetId());

	myCreatedPickerUppers.RemoveCyclic(aPickerUpperComponent);

	//delete aPickerUpperComponent;
}

void CPickupManager::Update(const CU::Time& aDeltaTime)
{
	for(unsigned short i = 0; i < myPickUpComponents.Size(); i++)
	{
		myPickUpComponents[i]->Update(aDeltaTime.GetSeconds());
	}
	
}

CPickupManager::CPickupManager()
	: myCreatedPickerUppers(8)
{
	myPickUpComponents.Init(100u);
}

CPickupManager::~CPickupManager()
{
	//for (auto it = myPickupData.begin(); it != myPickupData.end(); ++it)
	//{
	//	delete it->first;
	//}
	
	myPickupData.clear();

	//for (unsigned int i = 0; i < myCreatedPickerUppers.Size(); ++i)
	//{
	//	SAFE_DELETE(myCreatedPickerUppers[i]);
	//}

	myCreatedPickerUppers.RemoveAll();
}
