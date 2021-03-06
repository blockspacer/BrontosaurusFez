#pragma once
#include "TextBitmap.h"
#include <d3d11.h>
#include "../FontEngine/FT_FontFacade.h"

enum class eAlignment;

namespace CU
{
	class DynamicString;

	template <typename T>
	class Vector4;
	using Vector4f = Vector4<float>;

	template <typename T>
	class Vector2;
	using Vector2f = Vector2<float>;

	template<typename T, typename S = unsigned int, bool F = true>
	class GrowingArray;
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
	CCoolText(const CCoolText & aCoolText);
	CCoolText(const CU::DynamicString& aFontPath, const int aPixelSize);
	~CCoolText();


	void Render(const CU::GrowingArray<CU::DynamicString>& someStrings, const CU::Vector2f& aPosition, const CU::Vector4f& aColor/*, const CU::Vector2i& aSize*/, eAlignment anAlignement);
	float GetlineHeight() const;
	CU::Vector2i CalculateRectPixelSize(const std::string& aText);
private:
	bool InitBuffers();

	void RenderCharacter(const wchar_t aCharacter, const CU::Vector2f& aPosition, const CU::Vector4f& aColor);
	void ActivateEffect();
	void UpdateAndSetVertexConstantBuffer(const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const CU::Vector4f& aRectconst, const CU::Vector4f& aColor);

	CU::Vector2f CalculateAdjustment(eAlignment aAlignement, std::wstring aWString);

	ID3D11Buffer* myVertexBuffer;
	ID3D11Buffer* myVertexConstantBuffer;
	ID3D11Buffer* myPixelConstantBuffer;

	ID3D11Buffer* myInstanceBuffer;
	signed short myCurrentInstanceLimit;

	CTextBitmap myBitmap;

	CFT_FontFacade myFont;


};