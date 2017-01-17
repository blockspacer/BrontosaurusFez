#include "stdafx.h"
#include "MasterAI.h"
#include "PollingStation.h"

CMasterAI::CMasterAI()
{

}

CMasterAI::~CMasterAI()
{
}

// How to connect Master AI with the game?

// make all dropcomponents use masterAI ?
// or maybe i should make it more general, and let them communicate through the global postmaster.

void CMasterAI::DetermineHealthDrop()
{
	// PollingStation::playerData->myStats->health;  Can't get Health through pollingStation yet.

	//Droprate at 100% health should be 10%
	//Droprate at 10% health should be 100%

	//Each healthglobe could give you a percentage amount of HP, let's say 30%;
}
