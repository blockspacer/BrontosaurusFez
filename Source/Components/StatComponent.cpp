#include "stdafx.h"
#include "StatComponent.h"
#include "GameObject.h"

CStatComponent::CStatComponent()
{
	myType = eComponentType::eStat;
}

CStatComponent::~CStatComponent()
{
}

void CStatComponent::SetStats(Stats::SBaseStats& aBaseStats, Stats::SBonusStats& aBonusStats)
{
	myBaseStats = &aBaseStats;
	myBonusStats = &aBonusStats;

	SComponentMessageData statsToBeSent;
	statsToBeSent.myComponent = this;
	GetParent()->NotifyComponents(eComponentMessageType::eCollectStats, statsToBeSent);
}

void CStatComponent::Receive(const eComponentMessageType aMessageType, const SComponentMessageData & aMessageData)
{
	switch (aMessageType) // fixa med messages så att man notify'ar eCollectStats när objektet läggs till som parent, mebe.
	{
	}
}

void CStatComponent::Destroy()
{
	delete myBonusStats;
	myBonusStats = nullptr;
	delete myBaseStats;
	myBaseStats = nullptr;
}
