#pragma once


#include "../CommonUtilities/vector2.h"
#include <map>
#include <d3d11.h>
#include <ft2build.h>
#include FT_FREETYPE_H

class CTextBitmap;

class CFT_Font
{
public:
	CFT_Font();
	~CFT_Font();

	void SetSize(const int pt, const int aDeviceWidth, const unsigned int aDeviceHeight);
	CTextBitmap RenderText(const wchar_t* aString, const int aStringLength, const CU::Vector2i& aTargetSize);

	CTextBitmap RenderChar(wchar_t aChar);
	CTextBitmap RenderChar(unsigned  aGlyphIndex);

	CTextBitmap GetAdvance(wchar_t aCurrentChar, wchar_t aPrevoiusChar);
	CU::Vector2i GetAdvance(unsigned aCurrentGlyph, unsigned aPrevoiusGlyph);

	FT_Face myFace;
	const char* myFacePath;
private:
	void CreateCharTexture(unsigned aGlyphIndex);

	std::map<unsigned, ID3D11ShaderResourceView*> myRenderedGlyphs;
};

