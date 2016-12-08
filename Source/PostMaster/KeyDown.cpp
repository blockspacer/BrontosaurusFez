#include "stdafx.h"
#include "KeyDown.h"

#include "../CommonUtilities/EKeyboardKeys.h"
#include "../BrontoSaurusEngine/Engine.h"

KeyDown::KeyDown(const CU::eKeys& aKey)
	: myKey(aKey)
{
}

KeyDown::~KeyDown()
{
}