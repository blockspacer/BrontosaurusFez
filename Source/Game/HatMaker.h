#pragma once
#include <map>
#include <string>
#include "PostMaster/Subscriber.h"
#include "Components/StatStructs.h"
#include "matrix44.h"


class CGameObjectManager;
class CGameObject;
class CModelComponent;

struct SHat;
struct SHatBluePrint
{
	std::string HatName;
	std::string HatModel;
	SHat* myHatStruct;
};
class CHatMaker : public Subscriber
{
private:
	struct SHatObject
	{
		CGameObject* myObjectPtr = nullptr;
		CU::Matrix44f myTransformation;
	};

public:
	CHatMaker(CGameObjectManager* aGameObjectManager);
	~CHatMaker();
	void Update();
	void LoadBluePrints(const std::string& aFilePath);
	void MakeHatFromBluePrint(const std::string& aHatName);
	void GiveTheManAHat();
	const bool CheckIfHatIsMade(const std::string& aHatName);
private:
	short myHatsGivenToPlayer;
	CU::GrowingArray<SHatObject> myHatObjects;
	CModelComponent* myPlayerModel;;
	std::map<std::string, SHatBluePrint*> myBluePrints;
	CGameObjectManager* myGameObjectManager;
	virtual eMessageReturn Recieve(const Message & aMessage) override;
};

