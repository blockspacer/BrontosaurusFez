#pragma once
#include <map>
#include <string>
#include "PostMaster/Subscriber.h"
#include "Components/StatStructs.h"

class CGameObjectManager;

struct SHatBluePrint
{
	std::string HatName;
	std::string HatModel;
	Stats::SBaseStats BaseStats;
	Stats::SBonusStats BonusStats;
	Stats::STotalStats TotalStats;
};

class CHatMaker : public Subscriber
{
public:
	CHatMaker(CGameObjectManager* aGameObjectManager);
	~CHatMaker();
	
	void LoadBluePrints(const std::string& aFilePath);
	void MakeHatFromBluePrint(const std::string& aHatName);
private:
	std::map<std::string, SHatBluePrint*> myBluePrints;
	CGameObjectManager* myGameObjectManager;
	virtual eMessageReturn Recieve(const Message & aMessage) override;
};

