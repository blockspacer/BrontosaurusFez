#pragma once

enum class TweenType
{
	Linear,
	Sinusoidal,
	Quadratic,
	Cubic,
	Quartic,
	Quintic,
	Exponential,
	Circular,
	Back,
	Elastic,
	Bounce
};

enum class TweenMod
{
	EaseIn,
	EaseOut,
	EaseInOut
};


class Tween
{
public:
	Tween(TweenType aType, TweenMod aMod, float aStart, float aEnd, float aDuration);
	~Tween();
	void Update(float aDeltaTime);
	float GetValue();
	bool IsFinished();

private:
	class CDBTweener* myTweener;
	float myValue;
	float myProgress;
	float myDuration;
};

