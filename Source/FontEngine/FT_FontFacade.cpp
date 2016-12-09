#include "stdafx.h"
#include "FT_FontFacade.h"
#include "FT_Font.h"


CFT_FontFacade::CFT_FontFacade()
{
	myFontpointer = nullptr;
}

CFT_FontFacade::CFT_FontFacade(CFT_Font* aFont)
{
	myFontpointer = aFont;
}

CFT_FontFacade::~CFT_FontFacade()
{
}

void CFT_FontFacade::SetSize(const int pt, const int aDeviceWidth, const unsigned aDeviceHeight)
{
	if (myFontpointer != nullptr)
	{
		myFontpointer->SetSize(pt, aDeviceWidth, aDeviceHeight);
	}
}
