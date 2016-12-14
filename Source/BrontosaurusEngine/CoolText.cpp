#include "stdafx.h"
#include "CoolText.h"
#include "Engine.h"
#include "DXFramework.h"
#include "../FontEngine/FontEngineFacade.h"
#include "ConstBufferTemplate.h"
#include "Effect.h"

struct SVertexConstantBufferType
{
	CU::Vector2f position;
	CU::Vector2f size;
	CU::Vector4f rect;
	CU::Vector4f color;
};

struct TextVertexConstantBuffer
{
	CU::Vector2f worldPosition;
	float size;
	float garbage;
};


CCoolText::CCoolText(const CU::DynamicString& aFontPath): myVertexConstantBuffer(nullptr), myPixelConstantBuffer(nullptr)
{
	myFont = CFontEngineFacade::GetInstance().GetFace(aFontPath.c_str());

	InitBuffers();
}

CCoolText::~CCoolText()
{
}

void CCoolText::Render(const CU::DynamicString& aString, const CU::Vector2f& aPosition, const CU::Vector4f& aColor/*, const CU::Vector2i& aSize*/)
{
	CU::Vector2f penPosition = aPosition;

	ActivateEffect();

	const std::string tempString = aString.c_str();
	std::wstring wideString(tempString.begin(), tempString.end());

	for (int i = 0; i < wideString.size(); ++i)
	{
		if (i > 0)
		{
			const CU::Vector2i pixelAdvance = myFont.GetAdvance(wideString[i], wideString[i - 1], true);
			const CU::Vector2f screenAdvance(static_cast<float>(pixelAdvance.x / WINDOW_SIZE.x), static_cast<float>(pixelAdvance.y / WINDOW_SIZE.y));
			penPosition += screenAdvance;
		}

		RenderCharacter(wideString[i], penPosition, aColor);
	}
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
	UpdateAndSetVertexConstantBuffer(aPosition, {static_cast<float>(glyphSize.x / WINDOW_SIZE.x), static_cast<float>(glyphSize.y / WINDOW_SIZE.y)}, { 0.f,1.f,0.f,1.f }, aColor);

	ID3D11ShaderResourceView* const shaderResourceView = myFont.GetCharResourceView(aCharacter);
	DEVICE_CONTEXT->PSSetShaderResources(1u, 1u, &shaderResourceView);
	CEngine::GetInstance()->GetFramework()->GetDeviceContext()->Draw(6, 0);
}

void CCoolText::ActivateEffect()
{
	CEffect *effect = myFont.GetEffect();
}

void CCoolText::UpdateAndSetVertexConstantBuffer(const CU::Vector2f& aPosition, const CU::Vector2f& aSize, const CU::Vector4f& aRect, const CU::Vector4f& aColor)
{
	ID3D11DeviceContext& context = *CEngine::GetInstance()->GetFramework()->GetDeviceContext();

	SVertexConstantBufferType cbufferStruct = {};
	cbufferStruct.position = aPosition;
	cbufferStruct.size = aSize;
	cbufferStruct.rect = aRect;
	cbufferStruct.color = aColor;

	D3D11_MAPPED_SUBRESOURCE mappedSubResource = {};

	context.Map(myVertexConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
	memcpy(mappedSubResource.pData, &cbufferStruct, sizeof(SVertexConstantBufferType));
	context.Unmap(myVertexConstantBuffer, 0);

	context.VSSetConstantBuffers(0, 1, &myVertexConstantBuffer);
}

