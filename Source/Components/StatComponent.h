#pragma once
#include "Component.h"
#include "StatStructs.h"

class CMainStatComponent;
class CStatComponent : public CComponent
{
public:
	friend CMainStatComponent;

	//Don't forget to set Stats AFTER adding this component to its parent.
	CStatComponent();
	~CStatComponent();
	void SetStats(Stats::SBaseStats& aBaseStats, Stats::SBonusStats& aBonusStats);

private:
	Stats::SBaseStats* myBaseStats;
	Stats::SBonusStats* myBonusStats;

	void Receive(const eComponentMessageType aMessageType, const SComponentMessageData& aMessageData) override;
	void Destroy() override;
};

