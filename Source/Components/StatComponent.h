#pragma once
#include "Component.h"
#include "StatStructs.h"

class CStatComponent : public CComponent
{
public:
	CStatComponent();
	~CStatComponent();
	void Set(const int aStrength, const int aDexterity,const int aIntelligence, const int aVitality);
private:
	void LevelUp();
	void CalculateTotalStats();
private:
	Stats::SBaseStatbody* myBaseStats;
	Stats::SBaseStatbody* myLevelUpBonus;
	Stats::SBonusStats* myBonusStats;
	Stats::STotalStats* myTotalStats;

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;
};

