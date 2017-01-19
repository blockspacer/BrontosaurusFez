#pragma once
struct SBaseStats;
struct PlayerData
{

	SBaseStats* myStats; // kanske inte borde vara pointer //skrev jag det? //Jag vet inte // ok.
	short myGold;
	float myGoldGetModifier = 1.0f;
};