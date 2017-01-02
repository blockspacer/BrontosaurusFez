#include "stdafx.h"
#include "ParticleEmitter.h"
#include "EmitterData.h"

#include "ShaderManager.h"
#include "Effect.h"
#include "VertexStructs.h"

#include "DXFramework.h"
#include "ConstBufferTemplate.h"
#include "TextureManager.h"




CParticleEmitter::CParticleEmitter()
{
	myTexture = nullptr;
	myEffect = nullptr;
	myFramework = nullptr;
	myVertexBuffer = nullptr;
	myModelBuffer = nullptr;
}

CParticleEmitter::~CParticleEmitter()
{
	Destroy();
}

void CParticleEmitter::Init(const SEmitterData& aEmitterData)
{
	myTexture = &TEXTUREMGR.LoadTexture(aEmitterData.TexturePath.c_str());
	myMaxNrOfParticles = static_cast<unsigned short>(aEmitterData.NumOfParticles);


	unsigned int ShaderType = 0;
	ShaderType |= EModelBluePrint_Position;
	ShaderType |= EModelBluePrint_Size;
	ShaderType |= EModelBluePrint_Color;
	
	ID3D11VertexShader* vertexShader = SHADERMGR->LoadVertexShader(L"Shaders/particle_shader.fx", ShaderType);
	ID3D11PixelShader* pixelShader = SHADERMGR->LoadPixelShader(L"Shaders/particle_shader.fx", ShaderType);
	ID3D11GeometryShader* geometryShader = SHADERMGR->LoadGeometryShader(L"Shaders/particle_shader.fx", ShaderType);
	ID3D11InputLayout* inputLayout = SHADERMGR->LoadInputLayout(L"Shaders/particle_shader.fx", ShaderType);

	CEffect* effect = new CEffect(vertexShader, pixelShader, geometryShader, inputLayout, D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	myEffect = effect;

	myFramework = FRAMEWORK;
	InitBuffers();
}

void CParticleEmitter::Render(const CU::Matrix44f & aToWorldSpace, const CU::GrowingArray<SParticle, unsigned short, false>& aParticleList)
{
	myEffect->Activate();
	UpdateCBuffers(aToWorldSpace);
	ResizeVertexBuffer(aParticleList);

	DEVICE_CONTEXT->PSSetShaderResources(1, 1, myTexture->GetShaderResourceViewPointer());

	UINT stride = sizeof(SParticle);
	UINT offset = 0;

	myFramework->GetDeviceContext()->IASetVertexBuffers(0, 1, &myVertexBuffer, &stride, &offset);
	myFramework->GetDeviceContext()->Draw(aParticleList.Size(), 0);
}

void CParticleEmitter::Destroy()
{
	if (myTexture)
	{
		TEXTUREMGR.DestroyTexture(myTexture);
	}

	if (myModelBuffer)
		myModelBuffer->Release();

	if (myVertexBuffer)
		myVertexBuffer->Release();

	if (myEffect)
	{
		SAFE_DELETE(myEffect);
	}
}


void CParticleEmitter::ResizeVertexBuffer(const CU::GrowingArray<SParticle, unsigned short, false>& aParticleList)
{
	D3D11_MAPPED_SUBRESOURCE resource;
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	DEVICE_CONTEXT->Map(myVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	unsigned int bufferSize = sizeof(SParticle);

	if (aParticleList.Size() > myMaxNrOfParticles)
		bufferSize *= myMaxNrOfParticles;
	else
		bufferSize *= aParticleList.Size();

	if (aParticleList.Size() > 0)
		memcpy(resource.pData, &aParticleList[0], bufferSize);

	DEVICE_CONTEXT->Unmap(myVertexBuffer, 0);
}

void CParticleEmitter::UpdateCBuffers(const CU::Matrix44f & aToWorldSpace)
{
	//WorldSpace
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	SToWorldSpace updatedWorld;
	updatedWorld.myWorldSpace = aToWorldSpace;

	DEVICE_CONTEXT->Map(myModelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	memcpy(mappedSubresource.pData, &updatedWorld, sizeof(SToWorldSpace));
	DEVICE_CONTEXT->Unmap(myModelBuffer, 0);
	DEVICE_CONTEXT->VSSetConstantBuffers(1, 1, &myModelBuffer);
}

bool CParticleEmitter::InitBuffers()
{
	//VertexBuffer
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(SParticle) * myMaxNrOfParticles;
	vertexBufferDesc.BindFlags = D3D11_USAGE_DYNAMIC | D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;

	HRESULT result;
	result = DEVICE->CreateBuffer(&vertexBufferDesc, nullptr, &myVertexBuffer);
	CHECK_RESULT(result, "Failed to create vertex buffer.");

	//Cbuffers
	SToWorldSpace worldBufferData;
	myModelBuffer = BSR::CreateCBuffer<SToWorldSpace>(&worldBufferData);

	return true;
}
