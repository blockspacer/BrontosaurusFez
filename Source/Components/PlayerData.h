#pragma once
struct SBaseStats;
struct PlayerData
{

	SBaseStats* myStats; // kanske inte borde vara pointer //skrev jag det? //Jag vet inte // ok.
	float myGoldGetModifier = 1.0f;
	float myPercentHPLeft = 100.0f;
	bool myIsWhirlwinding = false;

	inline unsigned short GetGold() const;
	inline unsigned short AddGold(const unsigned short aGold);
	inline unsigned short RemoveGold(const unsigned short aGold);

private:
	unsigned short myGold = 0;
};

inline unsigned short PlayerData::GetGold() const
{
	return myGold;
}

inline unsigned short PlayerData::AddGold(const unsigned short aGold)
{
	unsigned short newGold = myGold + aGold;
	if (newGold < myGold)
	{
		myGold = USHRT_MAX;
		return myGold;
	}

	myGold = newGold;
	return myGold;
}

inline unsigned short PlayerData::RemoveGold(const unsigned short aGold)
{
	if (aGold > myGold)
	{
		myGold = 0;
		return 0;
	}

	myGold -= aGold;
	return myGold;
}
