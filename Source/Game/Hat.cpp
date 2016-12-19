#include "stdafx.h"
#include "Hat.h"
#include <ModelComponentManager.h>
#include <StatComponent.h>

CHat::CHat(char* aModelPath, CStatComponent* aStats)
{
	myStatComponent = aStats;
	myModelComponent = MODELCOMP_MGR.CreateComponent(aModelPath);

	//Update ? Set to parent pos.   at head slot.
}

CHat::~CHat()
{
}

const CStatComponent& CHat::GetStats()
{
	return *myStatComponent; //->GetStats() ????
}

void CHat::Render()
{
	// How to render ?
}
