#include "stdafx.h"
#include "CoolText.h"
#include "Engine.h"
#include "DXFramework.h"
#include "../FontEngine/FontEngineFacade.h"
#include "ConstBufferTemplate.h"
#include "Effect.h"
#include <minwinbase.h>


struct SVertexConstantBufferType
{
	CU::Vector2f position;
	CU::Vector2f size;
	CU::Vector2f pivot;
	CU::Vector2f TRASH;
	CU::Vector4f rect;
	CU::Vector4f color;
};

struct TextVertexConstantBuffer
{
	CU::Vector2f worldPosition;
	float size;
	float garbage;
};

struct TextInstanceBufferType
{
	CU::Vector2f position;
	CU::Vector2f size;
	CU::Vector4f rect;
	CU::Vector4f color;
};


CCoolText::CCoolText(const CU::DynamicString& aFontPath): myVertexConstantBuffer(nullptr), myPixelConstantBuffer(nullptr)
{
	myFont = CFontEngineFacade::GetInstance().GetFace(aFontPath.c_str());
	//myFont.SetSize(32, 0, 0);
	InitBuffers();
}

CCoolText::CCoolText(const CCoolText& aCoolText)
{
	myFont = aCoolText.myFont;
	InitBuffers();
}

CCoolText::CCoolText(const CU::DynamicString & aFontPath, const int aPixelSize):
	myVertexBuffer(nullptr),
	myPixelConstantBuffer(nullptr)
{
	myFont = CFontEngineFacade::GetInstance().GetFace(aFontPath.c_str());
	myFont.SetSize(aPixelSize, 0, 0);
	InitBuffers();
}

CCoolText::~CCoolText()
{
	SAFE_RELEASE(myVertexBuffer);
	SAFE_RELEASE(myVertexConstantBuffer);
	SAFE_RELEASE(myPixelConstantBuffer);
}

CU::Vector2f CCoolText::CalculateAdjustment(eAlignment aAlignement,std::wstring aWString)
{
	if (aAlignement == eAlignment::LEFT)
	{
		return CU::Vector2f::Zero;
	}

	float stringWidth = 0;

	for (std::wstring::size_type i = 1; i < aWString.size(); ++i)
	{
		const CU::Vector2i pixelAdvance = myFont.GetAdvance(aWString[i], aWString[i - 1], true);
		const CU::Vector2f screenAdvance(static_cast<float>(pixelAdvance.x) / WINDOW_SIZE_F.x, static_cast<float>(pixelAdvance.y) / WINDOW_SIZE_F.y);
		stringWidth -= screenAdvance.x;
	}

	if (aAlignement == eAlignment::CENTER)
	{
		stringWidth = stringWidth / 2;
	}

	return CU::Vector2f(stringWidth, 0);
}

void CCoolText::Render(const CU::GrowingArray<CU::DynamicString>& someStrings, const CU::Vector2f& aPosition, const CU::Vector4f& aColor/*, const CU::Vector2i& aSize*/, eAlignment anAlignement)
{
	CU::Vector2f penPosition = aPosition;

	ActivateEffect();

	for (int j = 0; j < someStrings.Size(); ++j)
	{

		const std::string tempString = someStrings[j].c_str();
		std::wstring wideString(tempString.begin(), tempString.end());

		penPosition.x = aPosition.x + CalculateAdjustment(anAlignement, wideString).x;
		penPosition.y += GetlineHeight();

		for (size_t i = 0; i < wideString.size(); ++i)
		{

			if (i > 0)
			{
				const CU::Vector2i pixelAdvance = myFont.GetAdvance(wideString[i], wideString[i - 1], true);
				const CU::Vector2f screenAdvance(static_cast<float>(pixelAdvance.x) / WINDOW_SIZE_F.x, static_cast<float>(pixelAdvance.y) / WINDOW_SIZE_F.y);
				penPosition += screenAdvance;
			}
			

			const CU::Vector2i bearing = myFont.GetBearing(wideString[i]);
			const CU::Vector2f screenBearing(static_cast<float>(bearing.x) / WINDOW_SIZE_F.x, static_cast<float>(-bearing.y) / WINDOW_SIZE_F.y);

			RenderCharacter(wideString[i], penPosition + screenBearing, aColor);
		}
	}
}

float CCoolText::GetlineHeight() const
{
	return myFont.GetlineHeight() / WINDOW_SIZE_F.y;
}

CU::Vector2i CCoolText::CalculateRectPixelSize(const std::string& aText)
{
	CU::Vector2i rectSize;
	std::wstring wText(aText.begin(), aText.end());
	for (size_t i = 0; i < wText.size(); ++i)
	{
		CU::Vector2i charSize;
		if (!myFont.RequestCharSize(charSize, wText[i]))
		{
			DL_PRINT("failed to get char size for %c", aText[i]);
			continue;
		}

		rectSize.x += charSize.x;
		if (charSize.y > rectSize.y)
		{
			rectSize.y = charSize.y;
		}
	}

	return rectSize;
}

bool CCoolText::InitBuffers()
{
	/*CU::Vector4f topLeft  = { 0.0f	, 1.0f  , 0.5f, 1.0f };
	CU::Vector4f botLeft  = { 1.0f	, 0.0f	, 0.5f, 1.0f };
	CU::Vector4f topRight = { 0.0f	, 0.0f  , 0.5f, 1.0f };
	CU::Vector4f botRight = { 0.0f	, 1.0f	, 0.5f, 1.0f };*/

	CU::Vector4f vertices[6] =
	{
		CU::Vector4f(0.0f, 0.0f, -0.50f, 1.0f),
		CU::Vector4f(0.0f, 1.0f, -0.50f, 1.0f),
		CU::Vector4f(1.0f, 1.0f, -0.50f, 1.0f),
		CU::Vector4f(0.0f, 0.0f, -0.50f, 1.0f),
		CU::Vector4f(1.0f, 1.0f, -0.50f, 1.0f),
		CU::Vector4f(1.0f, 0.0f, -0.50f, 1.0f)
		//topLeft, botLeft, botRight, botRight, topRight, topLeft
	};

	// VERTEX BUFFER
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.ByteWidth = sizeof(vertices);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = vertices;
	HRESULT result = FRAMEWORK->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &myVertexBuffer);
	CHECK_RESULT(result, "Failed to create vertexbuffer.");

	SVertexConstantBufferType bufferData;
	myVertexConstantBuffer = BSR::CreateCBuffer<SVertexConstantBufferType>(&bufferData);

	return SUCCEEDED(result);
}

void CCoolText::RenderCharacter(const wchar_t aCharacter, const CU::Vector2f& aPosition, const CU::Vector4f& aColor)
{

	const CU::Vector2i glyphSize = myFont.GetCharSize(aCharacter);
	UpdateAndSetVertexConstantBuffer(aPosition, {static_cast<float>(glyphSize.x) / WINDOW_SIZE.x, static_cast<float>(glyphSize.y) / WINDOW_SIZE.y}, { 0.f,0.f,1.f,1.f }, aColor);

	ID3D11ShaderResourceView* const shaderResourceView = myFont.GetCharResourceView(aCharacter);
	DEVICE_CONTEXT->PSSetShaderResources(1u, 1u, &shaderResourceView);

	UINT stride = sizeof(float) * 4u;
	UINT offset = 0;
	DEVICE_CONTEXT->IASetVertexBuffers(0, 1, &myVertexBuffer, &stride, &offset);
	CEngine::GetInstance()->GetFramework()->GetDeviceContext()->Draw(6, 0);
}

void CCoolText::ActivateEffect()
{
	CEffect *effect = myFont.GetEffect();
	effect->Activate();
}

void CCoolText::UpdateAndSetVertexConstantBuffer(const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const CU::Vector4f& aRect, const CU::Vector4f& aColor)
{
	ID3D11DeviceContext& context = *CEngine::GetInstance()->GetFramework()->GetDeviceContext();

	SVertexConstantBufferType cbufferStruct = {};
	cbufferStruct.position = aPosition;
	cbufferStruct.size = aSize;
	cbufferStruct.pivot = { 0.0f,0.0f };
	cbufferStruct.rect = aRect;
	cbufferStruct.color = aColor;

	D3D11_MAPPED_SUBRESOURCE mappedSubResource = {};

	context.Map(myVertexConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
	memcpy(mappedSubResource.pData, &cbufferStruct, sizeof(SVertexConstantBufferType));
	context.Unmap(myVertexConstantBuffer, 0);

	context.VSSetConstantBuffers(1, 1, &myVertexConstantBuffer);
}
