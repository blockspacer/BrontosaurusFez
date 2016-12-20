#pragma once
#include "Component.h"
#include "StatStructs.h"

class CMainStatComponent : public CComponent
{
public:
	CMainStatComponent();
	~CMainStatComponent();
private:
	void CalculateTotalStats();
	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;
private:
	Stats::SBaseStats* myBaseStats;
	Stats::SBonusStats* myBonusStats;
	Stats::STotalStats* myTotalStats;
};

