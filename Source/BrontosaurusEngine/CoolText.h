#pragma once
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

	void Render(const CU::DynamicString& aString, const CU::Vector2f& aPosition, const CU::Vector4f& aColor, const CU::Vector2i& aSize);

private:
	void CreateVertexBuffer(ID3D11Device& aDevice);
	void CreateConstantBuffer();

	void RenderCharacter(const char aCharacter, const CU::Vector2f& aPosition, const CU::Vector4f& aColor, ID3D11DeviceContext& aDeviceContext);
	void ActivateEffect();
	void UpdateAndSetVertexBuffers(const char aCharacter, ID3D11DeviceContext& aDeviceContext);
	void UpdateAndSetConstantBuffer(const char aCharacter, const CU::Vector2f& aPosition, const CU::Vector4f& aColor, ID3D11DeviceContext& aDeviceContext);
	void CreateTexture();

	ID3D11Buffer* myVertexBufferPos;
	ID3D11Buffer* myVertexBufferUV;
	ID3D11Buffer* myVertexConstantBuffer;
	ID3D11Buffer* myPixelConstantBuffer;

	CU::DynamicString myLastaString;
	CTextBitmap myBitmap;

	CFT_FontFacade myFont;

	ID3D11ShaderResourceView* aResourceView;
};