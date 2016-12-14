#pragma once
#include "../CommonUtilities/GrowingArray.h"
#include "TextBitmap.h"
#include <d3d11.h>
#include "../FontEngine/FT_FontFacade.h"

namespace CU
{
	class DynamicString;

	template <typename T>
	class Vector4;
	using Vector4f = Vector4<float>;

	template <typename T>
	class Vector2;
	using Vector2f = Vector2<float>;
}

class CFont;
struct SCharacterInfo;
struct STextVertexInput;

struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;

class CCoolText
{
public:
	CCoolText(const CU::DynamicString& aFontPath);
	~CCoolText();

	void Render(const CU::DynamicString& aString, const CU::Vector2f& aPosition, const CU::Vector4f& aColor/*, const CU::Vector2i& aSize*/);

private:
	bool InitBuffers();

	void RenderCharacter(const wchar_t aCharacter, const CU::Vector2f& aPosition, const CU::Vector4f& aColor);
	void ActivateEffect();
	void UpdateAndSetVertexConstantBuffer(const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const CU::Vector4f& aRectconst, const CU::Vector4f& aColor);


	ID3D11Buffer* myVertexBuffer;
	ID3D11Buffer* myVertexConstantBuffer;
	ID3D11Buffer* myPixelConstantBuffer;

	CTextBitmap myBitmap;

	CFT_FontFacade myFont;
};