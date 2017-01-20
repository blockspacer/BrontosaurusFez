#pragma once
struct HatActivatorData
{
	float healthLimitActivator;
	float manaLimitActivator;
	bool isLimitActivationUnder;
};
struct SHat;
class HatActivator
{
public:
	HatActivator(bool aIsActive, HatActivatorData* someData, SHat* aHatPointer);
	~HatActivator();
	void CheckHealthLimit(float aHealthPercentage);
	void CheckManaLimit(float aManaPercentage);
	inline bool GetIsActive();
private:
	void Activate();
	void Deactivate();
private:
	HatActivatorData* myData;
	SHat* myHatStructPointer;
	bool myIsActive;
};

inline bool HatActivator::GetIsActive()
{
	return myIsActive;
}