#pragma once
struct SBaseStats;
struct PlayerData
{

	SBaseStats* myStats; // kanske inte borde vara pointer //skrev jag det? //Jag vet inte // ok.
	bool myIsWhirlwinding = false;
	unsigned short myGold;
	float myGoldGetModifier = 1.0f;
	float myPercentHPLeft = 100.0f;
};