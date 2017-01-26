#pragma once
#include <map>
#include <string>
#include "PostMaster/Subscriber.h"
#include "Components/StatStructs.h"

class CGameObjectManager;
struct SHat;
struct SHatBluePrint
{
	std::string HatName;
	std::string HatModel;
	SHat* myHatStruct;
};

class CHatMaker : public Subscriber
{
public:
	CHatMaker(CGameObjectManager* aGameObjectManager);
	~CHatMaker();
	
	void LoadBluePrints(const std::string& aFilePath);
	void MakeHatFromBluePrint(const std::string& aHatName);
	void GiveTheManAHat();
	const bool CheckIfHatIsMade(const std::string& aHatName);
private:
	short myHatsGivenToPlayer;
	std::map<std::string, SHatBluePrint*> myBluePrints;
	CGameObjectManager* myGameObjectManager;
	virtual eMessageReturn Recieve(const Message & aMessage) override;
};

