#include "stdafx.h"
#include "Text.h"

#include "Font.h"
#include "Effect.h"
#include "Texture.h"
#include "Engine.h"
#include "DXFramework.h"

#include "../CommonUtilities/vector2.h"
#include "../CommonUtilities/vector4.h"

#include <d3d11.h>
#include "ConstBufferTemplate.h"

#include "../CommonUtilities/MemoryLeekLogger.h"

CFont* CText::ourFont = nullptr;

struct STextVertexInputPos
{
	CU::Vector4f position;
};

struct STextVertexInputUV 
{
	CU::Vector2f uv;
	CU::Vector2f characterSize;
};

struct TextVertexConstantBuffer 
{
	CU::Vector2f worldPosition;
	float size;
	float garbage;
};

struct TextPixelConstantBuffer
{
	CU::Vector4f color;
};

CText::CText(const CU::DynamicString& aFontPath)
	: myVertexBufferPos(nullptr)
	, myVertexBufferUV(nullptr)
	, myVertexConstantBuffer(nullptr)
	, myPixelConstantBuffer(nullptr)
{
	if (ourFont == nullptr)
	{
		ourFont = new CFont(aFontPath);
	}

	ourFont->AddRef();

	CreateVertexBuffer(*CEngine::GetInstance()->GetFramework()->GetDevice());
	CreateConstantBuffer();
}

CText::~CText()
{
	if (ourFont->DecRef() == KILL_ME)
	{
		SAFE_DELETE(ourFont);
	}

	SAFE_RELEASE(myVertexBufferPos);
	SAFE_RELEASE(myVertexBufferUV);
	//SAFE_RELEASE(myVertexConstantBuffer);
	//SAFE_RELEASE(myPixelConstantBuffer);
}

void CText::Render(const CU::DynamicString& aString, const CU::Vector2f& aPosition, const CU::Vector4f& aColor/*, ID3D11DeviceContext& aDeviceContext*/)
{
	ID3D11DeviceContext* deviceContextPtr = CEngine::GetInstance()->GetFramework()->GetDeviceContext();
	if (deviceContextPtr != nullptr)
	{
		ID3D11DeviceContext& deviceContext = *deviceContextPtr;

		CU::Vector2f currentOffset(0.f, 0.f);
		CU::Vector2f windowSize(CEngine::GetInstance()->GetWindowSize());

		ActivateEffect();
		SetTexture(deviceContext);

		for (int i = 0; i < aString.Size(); ++i)
		{
			if (aString[i] == '\n')
			{
				currentOffset.y += ourFont->GetLineHeight() / windowSize.y;
				currentOffset.x = 0.f;
			}
			else if (aString[i] == '\t')
			{
				//do something special
			}
			else
			{
				RenderCharacter(aString[i], aPosition + currentOffset, aColor, deviceContext);
				currentOffset.x += static_cast<float>(ourFont->GetCharacterInfo(aString[i]).width) / windowSize.x;
			}
		}
	}
}

void CText::CreateVertexBuffer(ID3D11Device& aDevice)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(STextVertexInputPos) * 6;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	STextVertexInputPos vertices[6];
	vertices[0].position.Set(0.0f, 0.0f, 0.5f, 1.0f);
	vertices[1].position.Set(0.0f, 1.0f, 0.5f, 1.0f);
	vertices[2].position.Set(1.0f, 1.0f, 0.5f, 1.0f);
	vertices[3].position.Set(0.0f, 0.0f, 0.5f, 1.0f);
	vertices[4].position.Set(1.0f, 1.0f, 0.5f, 1.0f);
	vertices[5].position.Set(1.0f, 0.0f, 0.5f, 1.0f);

	D3D11_SUBRESOURCE_DATA subresourceData = {};
	subresourceData.pSysMem = vertices;

	HRESULT result = aDevice.CreateBuffer(&vertexBufferDesc, &subresourceData, &myVertexBufferPos);
	if (result != S_OK)
	{
		DL_ASSERT("Failed to create vertex buffer in CText");
	}

	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(STextVertexInputUV) * 6;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;

	result = aDevice.CreateBuffer(&vertexBufferDesc, nullptr, &myVertexBufferUV);
	if (result != S_OK)
	{
		DL_ASSERT("Failed to create vertex buffer in CText");
	}
}

void CText::CreateConstantBuffer()
{
	TextVertexConstantBuffer vertexBufferData;
	myVertexConstantBuffer = BSR::CreateCBuffer(&vertexBufferData);

	TextPixelConstantBuffer pixelBufferData;
	myPixelConstantBuffer = BSR::CreateCBuffer(&pixelBufferData);
}

void CText::RenderCharacter(const char aCharacter, const CU::Vector2f& aPosition, const CU::Vector4f& aColor, ID3D11DeviceContext& aDeviceContext)
{
	UpdateAndSetConstantBuffer(aCharacter, aPosition, aColor, aDeviceContext);
	UpdateAndSetVertexBuffers(aCharacter, aDeviceContext);

	aDeviceContext.Draw(6, 0);
}

void CText::ActivateEffect()
{
	CEffect* effect = ourFont->GetEffect();
	if (effect != nullptr)
	{
		effect->Activate();
	}
}

void CText::UpdateAndSetVertexBuffers(const char aCharacter, ID3D11DeviceContext& aDeviceContext)
{
	const SCharacterInfo& characterInfo = ourFont->GetCharacterInfo(aCharacter);

	STextVertexInputUV vertexInput[6] = {};
	vertexInput[0].uv.Set(characterInfo.x, characterInfo.y + characterInfo.height);
	vertexInput[1].uv.Set(characterInfo.x, characterInfo.y/* + characterInfo.height*/);
	vertexInput[2].uv.Set(characterInfo.x + characterInfo.width, characterInfo.y/* + characterInfo.height*/);
	vertexInput[3].uv.Set(characterInfo.x, characterInfo.y + characterInfo.height);
	vertexInput[4].uv.Set(characterInfo.x + characterInfo.width, characterInfo.y/* + characterInfo.height*/);
	vertexInput[5].uv.Set(characterInfo.x + characterInfo.width, characterInfo.y + characterInfo.height);

	CU::Vector2f windowSize(CEngine::GetInstance()->GetWindowSize());
	for (int i = 0; i < 6; ++i)
	{
		vertexInput[i].uv.x /= (float)ourFont->GetTexture()->GetSize().x;
		vertexInput[i].uv.y /= (float)ourFont->GetTexture()->GetSize().y;
		vertexInput[i].characterSize.Set(characterInfo.width / windowSize.x, characterInfo.height / windowSize.y);
	}

	//vertexInput[0].uv.Set(0, 0);
	//vertexInput[1].uv.Set(0, 1);
	//vertexInput[2].uv.Set(1, 1);
	//vertexInput[3].uv.Set(0, 0);
	//vertexInput[4].uv.Set(1, 1);
	//vertexInput[5].uv.Set(1, 0);
	
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	ZeroMemory(&mappedSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	aDeviceContext.Map(myVertexBufferUV, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	memcpy(mappedSubresource.pData, &vertexInput, sizeof(STextVertexInputUV) * 6);
	aDeviceContext.Unmap(myVertexBufferUV, 0);


	UINT strides[2] = { sizeof(STextVertexInputPos), sizeof(STextVertexInputUV) };
	UINT offsets[2] = { 0, 0 };
	ID3D11Buffer* const vertexBuffers[2] = { myVertexBufferPos, myVertexBufferUV };

	aDeviceContext.IASetVertexBuffers(0, 2, vertexBuffers, strides, offsets);
}

void CText::UpdateAndSetConstantBuffer(const char aCharacter, const CU::Vector2f& aPosition, const CU::Vector4f& aColor, ID3D11DeviceContext& aDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE mappedSubResource;
	ZeroMemory(&mappedSubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	const SCharacterInfo& characterInfo = ourFont->GetCharacterInfo(aCharacter);

	TextVertexConstantBuffer vertexConstantBuffer;
	vertexConstantBuffer.worldPosition = aPosition;
	vertexConstantBuffer.worldPosition.y += static_cast<float>(characterInfo.yOffset) / static_cast<float>(CEngine::GetInstance()->GetWindowSize().y);
	vertexConstantBuffer.size = 1.f;

	aDeviceContext.Map(myVertexConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
	memcpy(mappedSubResource.pData, &vertexConstantBuffer, sizeof(TextVertexConstantBuffer));
	aDeviceContext.Unmap(myVertexConstantBuffer, 0);

	aDeviceContext.VSSetConstantBuffers(2u, 1u, &myVertexConstantBuffer);

	ZeroMemory(&mappedSubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	TextPixelConstantBuffer pixelConstantBuffer;
	pixelConstantBuffer.color = aColor;

	aDeviceContext.Map(myPixelConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
	memcpy(mappedSubResource.pData, &pixelConstantBuffer, sizeof(TextPixelConstantBuffer));
	aDeviceContext.Unmap(myPixelConstantBuffer, 0);

	aDeviceContext.PSSetConstantBuffers(2u, 1u, &myPixelConstantBuffer);
}

void CText::SetTexture(ID3D11DeviceContext& aDeviceContext)
{
	CTexture* texture = ourFont->GetTexture();
	if (texture != nullptr)
	{
		ID3D11ShaderResourceView** resourceViewArray = texture->GetShaderResourceViewPointer();
		if (resourceViewArray != nullptr)
		{
			aDeviceContext.PSSetShaderResources(1u, 1u, resourceViewArray);
		}
	}
}
