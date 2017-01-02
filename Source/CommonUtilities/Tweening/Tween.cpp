#include "stdafx.h"
#include "Tween.h"


Tween::Tween(TweenType aType, TweenMod aMod, float aStart, float aEnd, float aDuration) : myValue(aStart), myDuration(aDuration)
{
	CDBTweener::CEquation *aEquation = nullptr;
	CDBTweener::EEasing aEasing = CDBTweener::TWEA_IN;
	
	switch (aType)
	{
	case TweenType::Linear:
		aEquation = &CDBTweener::TWEQ_LINEAR;
		break;
	case TweenType::Sinusoidal:
		aEquation = &CDBTweener::TWEQ_SINUSOIDAL;
		break;
	case TweenType::Quadratic:
		aEquation = &CDBTweener::TWEQ_QUADRATIC;
		break;
	case TweenType::Cubic:
		aEquation = &CDBTweener::TWEQ_CUBIC;
		break;
	case TweenType::Quartic:
		aEquation = &CDBTweener::TWEQ_QUARTIC;
		break;
	case TweenType::Quintic:
		aEquation = &CDBTweener::TWEQ_QUINTIC;
		break;
	case TweenType::Exponential:
		aEquation = &CDBTweener::TWEQ_EXPONENTIAL;
		break;
	case TweenType::Circular:
		aEquation = &CDBTweener::TWEQ_CIRCULAR;
		break;
	case TweenType::Back:
		aEquation = &CDBTweener::TWEQ_BACK;
		break;
	case TweenType::Elastic:
		aEquation = &CDBTweener::TWEQ_ELASTIC;
		break;
	case TweenType::Bounce:
		aEquation = &CDBTweener::TWEQ_BOUNCE;
		break;
	default:
		break;
	}

	switch (aMod)
	{
	case TweenMod::EaseIn:
		aEasing = CDBTweener::TWEA_IN;
		break;
	case TweenMod::EaseOut:
		aEasing = CDBTweener::TWEA_OUT;
		break;
	case TweenMod::EaseInOut:
		aEasing = CDBTweener::TWEA_INOUT;
		break;
	default:
		break;

	}
	myTweener.addTween(aEquation, aEasing, myDuration, &myValue, aEnd);
}

Tween::~Tween()
{
}

void Tween::Update(float aDeltaProgress)
{
	if (!IsFinished())
	{
		myTweener.step(aDeltaProgress);
		myProgress += aDeltaProgress;
	}
}

float Tween::GetValue()
{
	return myValue;
}

bool Tween::IsFinished()
{
	return myProgress >= myDuration;
}
