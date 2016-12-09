#pragma once
#include "../CommonUtilities/vector2.h"
#include "../BrontosaurusEngine/TextBitmap.h"
class CFT_Font;
class CFT_FontFacade
{
public:
	CFT_FontFacade();
	CFT_FontFacade(CFT_Font* aFont);
	~CFT_FontFacade();

	void SetSize(const int pt, const int aDeviceWidth, const unsigned int aDeviceHeight);
	CTextBitmap RenderText(const wchar_t* aString, const int aStringLength, const CU::Vector2i& aTargetSize);

	CTextBitmap RenderChar(wchar_t aChar);
	CTextBitmap RenderChar(unsigned  aGlyphIndex);

	CTextBitmap GetAdvance(wchar_t aCurrentChar, wchar_t aPrevoiusChar);
	CU::Vector2i GetAdvance(unsigned aCurrentGlyph, unsigned aPrevoiusGlyph);

private:
	CFT_Font* myFontpointer;
};

