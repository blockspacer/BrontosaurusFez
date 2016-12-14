#include "stdafx.h"
#include "TextBitmap.h"
#include <d3d11.h>


CTextBitmap::CTextBitmap(): myWidth(0), myHeigth(0), myBitmap(nullptr), myUpToDate(false)
{
}


CTextBitmap::~CTextBitmap()
{
	delete[] myBitmap;
}

void CTextBitmap::Init(int aWidth, int aHeight)
{
	if (myBitmap != nullptr)
	{
		delete[] myBitmap;
		myBitmap = nullptr;
	}

	myBitmap = new unsigned char[aWidth * aHeight * 4];
	myWidth = aWidth;
	myHeigth = aHeight;
}

void CTextBitmap::DrawMono(const CU::Vector2i& aPosition, const CU::Vector2i& aSize, unsigned char* aBuffer, const CU::Vector3<unsigned char> aColour)
{
	CU::Vector2i penPosition(0, 0);

	for (int i = 0; i < aSize.x * aSize.y; ++i)
	{
		const CU::Point2i currentPosition = aPosition + penPosition;
		const int pixelIndex = myWidth * currentPosition.y + currentPosition.x * 4;
		
		myBitmap[pixelIndex] = aColour.r;
		myBitmap[pixelIndex + 1] = aColour.g;
		myBitmap[pixelIndex + 2] = aColour.b;
		myBitmap[pixelIndex + 3] = aBuffer[aSize.x * penPosition.y + penPosition.x];

		penPosition.x += 1;
		if (penPosition.x >= aSize.x)
		{
			penPosition.x = 0;
			penPosition.y += 1;
		}
	}
	myUpToDate = false;
}

unsigned char* CTextBitmap::CopyBitmapData()
{
	if (myBitmap != nullptr)
	{
		unsigned char* copy = new unsigned char[myHeigth * myWidth * 4];

		memcpy_s(copy, GetByteSize(), myBitmap, GetByteSize());

		return copy;
	}

	return nullptr;
}
