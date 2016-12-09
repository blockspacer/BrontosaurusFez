#include "stdafx.h"
#include "FT_Font.h"
#include "../BrontosaurusEngine/TextBitmap.h"
#include "../CommonUtilities/DL_Debug.h"
#include "../CommonUtilities/vector2.h"
#include <d3d10sdklayers.h>
#include "../BrontosaurusEngine/Engine.h"
#include "../BrontosaurusEngine/DXFramework.h"


#ifndef ERROR_CHECK
#define ERROR_CHECK(aError, msg) assert(aError == FT_Err_Ok && msg);
#endif


CFT_Font::CFT_Font()
{
	myFace = nullptr;
	myFacePath = nullptr;
}


CFT_Font::~CFT_Font()
{
}

void CFT_Font::SetSize(const int pt, const  int aDeviceWidth, const unsigned int aDeviceHeight)
{
	FT_Error error;
	error = FT_Set_Char_Size(myFace, 0, pt * 64, 0, 0);
	ERROR_CHECK(error, "failed to set Face char size");

	error = FT_Set_Pixel_Sizes(myFace, 0, pt);
	ERROR_CHECK(error, "failed to set Face pixel sizes");

}

CTextBitmap CFT_Font::RenderText(const wchar_t* aString, const int aStringLength, const CU::Vector2i& aTargetSize)
{
	if (myFace == nullptr)
	{
		DL_ASSERT("Font not initialized");
	}

	CTextBitmap textBitmap;
	textBitmap.Init(aTargetSize.x, aTargetSize.y);

	const bool useKerning = FT_HAS_KERNING(myFace);

	CU::Vector2i aPenPosition(0, 0);

	FT_UInt prevoiusGlyph = 0;
	for (int i = 0; i < aStringLength; ++i)
	{
		FT_UInt glyphIndex;

		glyphIndex = FT_Get_Char_Index(myFace, static_cast<FT_ULong>(aString[i]));

		if (useKerning && prevoiusGlyph && glyphIndex)
		{
			FT_Vector delta;

			FT_Get_Kerning(myFace, prevoiusGlyph, glyphIndex, FT_KERNING_DEFAULT, &delta);

			aPenPosition.x += delta.x >> 6;
		}

		FT_Error error;

		error = FT_Load_Glyph(myFace, glyphIndex, FT_LOAD_DEFAULT);

		if (error)
		{
			continue;
		}

		error = FT_Render_Glyph(myFace->glyph, FT_RENDER_MODE_NORMAL);

		if (error)
		{
			continue;
		}

		FT_GlyphSlot const glyphSlot = myFace->glyph;

		const int penX = aPenPosition.x + glyphSlot->bitmap_left;
		const int penY = aPenPosition.y + glyphSlot->bitmap_top;
		const CU::Vector2i finalPosition(penX, penY);


		textBitmap.DrawMono(finalPosition, CU::Vector2i(glyphSlot->bitmap.width, glyphSlot->bitmap.rows), glyphSlot->bitmap.buffer);

		aPenPosition.x += glyphSlot->advance.x >> 6;

		prevoiusGlyph = glyphIndex;
	}
	return textBitmap;
}


CTextBitmap CFT_Font::RenderChar(FT_UInt aGlyphIndex) 
{
	FT_Error error;

	error = FT_Load_Glyph(myFace, aGlyphIndex, FT_LOAD_DEFAULT);
	error = FT_Render_Glyph(myFace->glyph, FT_RENDER_MODE_NORMAL);
	
	FT_GlyphSlot const glyphSlot = myFace->glyph;

	CTextBitmap charBitmap;
	charBitmap.Init(glyphSlot->bitmap.width, glyphSlot->bitmap.rows);
	const int fuckingIntforSomkeFuckingreason = 0;
	charBitmap.DrawMono(CU::Vector2i(fuckingIntforSomkeFuckingreason, fuckingIntforSomkeFuckingreason), {static_cast<int>(glyphSlot->bitmap.width), static_cast<int>(glyphSlot->bitmap.rows)}, glyphSlot->bitmap.buffer);

	return charBitmap;
}

void CFT_Font::CreateCharTexture(FT_UInt aGlyphIndex)
{
	CTextBitmap bitmap = RenderChar(aGlyphIndex);

	D3D11_SUBRESOURCE_DATA* subresourceData = new D3D11_SUBRESOURCE_DATA;
	subresourceData->pSysMem = bitmap.CopyBitmapData();
	subresourceData->SysMemPitch = bitmap.GetMemPitch();
	subresourceData->SysMemSlicePitch = bitmap.GetByteSize();

	D3D11_TEXTURE2D_DESC texture2DDesc;

	texture2DDesc.Width = bitmap.GetWidth();
	texture2DDesc.Height = bitmap.GetHeight();
	texture2DDesc.MipLevels = 1;
	texture2DDesc.ArraySize = 1;
	texture2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture2DDesc.SampleDesc.Count = 1;
	texture2DDesc.SampleDesc.Quality = 0;
	texture2DDesc.Usage = D3D11_USAGE_DYNAMIC;
	texture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texture2DDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	texture2DDesc.MiscFlags = 0;

	ID3D11Texture2D * texture2D = nullptr;

	HRESULT result = S_OK;
	result = CEngine::GetInstance()->GetFramework()->GetDevice()->CreateTexture2D(&texture2DDesc, subresourceData, &texture2D);
	if (result != S_OK)
	{
		DL_ASSERT("Failed creating text texture");
	}

	ID3D11ShaderResourceView* resourceView = nullptr;

	result = CEngine::GetInstance()->GetFramework()->GetDevice()->CreateShaderResourceView(texture2D, NULL, &resourceView);
	if (result != S_OK)
	{
		DL_ASSERT("Failed creating text shader resource");
	}

	myRenderedGlyphs[aGlyphIndex] = resourceView;

	texture2D->Release();
}
