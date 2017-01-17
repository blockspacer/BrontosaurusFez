#pragma once

#include "../CommonUtilities/StaticArray.h"
#include "../CommonUtilities/vector4.h"

struct SPixelConstantBuffer
{
	enum eMember
	{
		eIsProgressBar,
		eProgressBarValue,
		eSkipEmissive,
		eFlashButton,
		eIsHealthBar,
		eHealthPercent,
		eIsManaBar,
		eManaPercent,
		eLength,
	};

	SPixelConstantBuffer() : myValues(0.f) {}

	CU::StaticArray<float, eMember::eLength> myValues;
	CU::Vector4f myCameraPosition;
};
