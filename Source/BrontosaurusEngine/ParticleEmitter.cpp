#include "stdafx.h"
#include "ParticleEmitter.h"
//#include <iostream>
#include <CUTime.h>
//#include <CommonUtilities.h>


#include "ShaderManager.h"
#include "Effect.h"
#include "VertexStructs.h"

#include "DXFramework.h"
#include "ConstBufferTemplate.h"
#include "TextureManager.h"

namespace
{
	#define RAND_FLOAT_RANGE(LOW, HIGH) (LOW) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/((HIGH)-(LOW))));


	float fLerp(float aStart, float aEnd, float aTime)
	{
		return (aEnd + aTime*(aStart - aEnd));
	}
}


CParticleEmitter::CParticleEmitter(const SEmitterData& aEmitterData)
{
	myTexture = nullptr;
	myEffect = nullptr;
	myFramework = nullptr;
	myVertexBuffer = nullptr;
	myModelBuffer = nullptr;

	myEmitDelta = 0.f;
	myEmitTimer = 0.f;
	myIsActive = true;

	myEmitterData = aEmitterData;
}
CParticleEmitter::~CParticleEmitter()
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

	myIsActive = false;
}

void CParticleEmitter::Init()
{
	myParticles.Init(64);
	myParticleLogic.Init(64);

	myEmitDelta = 1.f / myEmitterData.EmissionRate;
	myEmitTimer = 0.f;

	myTexture = &TEXTUREMGR.LoadTexture(myEmitterData.TexturePath);

	unsigned int ShaderType = 0;
	ShaderType |= EModelBluePrint_Position;
	ShaderType |= EModelBluePrint_Size;
	ShaderType |= EModelBluePrint_Alpha;
	
	ID3D11VertexShader* vertexShader = SHADERMGR->LoadVertexShader(L"Shaders/particle_shader.fx", ShaderType);
	ID3D11PixelShader* pixelShader = SHADERMGR->LoadPixelShader(L"Shaders/particle_shader.fx", ShaderType);
	ID3D11GeometryShader* geometryShader = SHADERMGR->LoadGeometryShader(L"Shaders/particle_shader.fx", ShaderType);
	ID3D11InputLayout* inputLayout = SHADERMGR->LoadInputLayout(L"Shaders/particle_shader.fx", ShaderType);

	CEffect* effect = new CEffect(vertexShader, pixelShader, geometryShader, inputLayout, D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	myEffect = effect;

	myFramework = FRAMEWORK;
	InitBuffers();
}

void CParticleEmitter::Update(const CU::Time & aDeltaTime, const CU::Matrix44f & aToWorldSpace)
{
	float deltaTime = aDeltaTime.GetSeconds();

	if (myIsActive == true)
	{
		myEmitTimer += deltaTime;
		if (myEmitTimer >= myEmitDelta)
		{
			myEmitTimer -= myEmitDelta;
			EmitParticle(aToWorldSpace);
		}
	}
	if (myParticles.Size() != myParticleLogic.Size())
	{
		assert(false && "Particlelist and logiclist missalligned.");
	}

	for (unsigned int i = 0; i < myParticles.Size(); ++i)
	{
		myParticleLogic[i].lifetimeLeft -= deltaTime;
		if (myParticleLogic[i].lifetimeLeft <= 0.0f)
		{
			myParticles.RemoveCyclicAtIndex(i);
			myParticleLogic.RemoveCyclicAtIndex(i);
			--i;
			continue;
		}

		CU::Vector4f deltaMovement = myParticleLogic[i].movementDir * deltaTime;
		myParticles[i].pos += deltaMovement;
		myParticles[i].pos.w = myParticleLogic[i].rotation;

		myParticles[i].size = fLerp(myEmitterData.StartSize, myEmitterData.EndSize, myParticleLogic[i].lifetimeLeft / myParticleLogic[i].lifeTime);
		myParticles[i].alpha = fLerp(myEmitterData.StartAlpha, myEmitterData.EndAlpha, myParticleLogic[i].lifetimeLeft / myParticleLogic[i].lifeTime);
	}
}

void CParticleEmitter::Render(const CU::Matrix44f & aToWorldSpace)
{
	if (myParticles.Size() > 0)// && myIsActive == true) TODO: Fixa nått snyggare än detta! Woago
	{
		myEffect->Activate();
		UpdateCBuffers(aToWorldSpace);

		DEVICE_CONTEXT->PSSetShaderResources(1, 1, myTexture->GetShaderResourceViewPointer());

		UINT stride = sizeof(SParticle);
		UINT offset = 0;

		myFramework->GetDeviceContext()->IASetVertexBuffers(0, 1, &myVertexBuffer, &stride, &offset);
		myFramework->GetDeviceContext()->Draw(myParticles.Size(), 0);
	}
}

void CParticleEmitter::EmitParticle(const CU::Matrix44f& aToWorldSpace)
{
	SParticle particle;
	particle.pos = aToWorldSpace.GetPosition();
	particle.alpha = myEmitterData.StartAlpha;
	particle.size = myEmitterData.StartSize;

	SParticleLogic logic;
	logic.lifeTime = RAND_FLOAT_RANGE(myEmitterData.MinParticleLifeTime, myEmitterData.MaxParticleLifeTime);
	logic.lifetimeLeft = logic.lifeTime;
	logic.rotation = RAND_FLOAT_RANGE(0.0f, 360.0f);
	
	float x = RAND_FLOAT_RANGE(myEmitterData.MinEmissionVelocity.x, myEmitterData.MaxEmissionVelocity.x);
	float y = RAND_FLOAT_RANGE(myEmitterData.MinEmissionVelocity.y, myEmitterData.MaxEmissionVelocity.y);
	float z = RAND_FLOAT_RANGE(myEmitterData.MinEmissionVelocity.z, myEmitterData.MaxEmissionVelocity.z);
	//std::cout << x << " " << y << " " << z << std::endl;

	logic.movementDir = CU::Vector3f(x, y, z);

	myParticles.Add(particle);
	myParticleLogic.Add(logic);
}

void CParticleEmitter::UpdateCBuffers(const CU::Matrix44f & aToWorldSpace)
{
	//WorldSpace
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	SToWorldSpace updatedWorld;
	updatedWorld.myWorldSpace = aToWorldSpace;
	unsigned int bufferSize;
	DEVICE_CONTEXT->Map(myModelBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	memcpy(mappedSubresource.pData, &updatedWorld, sizeof(SToWorldSpace));
	DEVICE_CONTEXT->Unmap(myModelBuffer, 0);
	DEVICE_CONTEXT->VSSetConstantBuffers(1, 1, &myModelBuffer);


	//VertexBuffer
	ZeroMemory(&mappedSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	DEVICE_CONTEXT->Map(myVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);

	bufferSize = sizeof(SParticle);

	if (myParticles.Size() > ourMaxNrOfParticles)
		bufferSize *= ourMaxNrOfParticles;
	else
		bufferSize *= myParticles.Size();

	if (myParticles.Size() > 0)
		memcpy(mappedSubresource.pData, &myParticles[0], bufferSize);

	DEVICE_CONTEXT->Unmap(myVertexBuffer, 0);
}

bool CParticleEmitter::InitBuffers()
{
	//VertexBuffer
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(SParticle) * ourMaxNrOfParticles;
	vertexBufferDesc.BindFlags = D3D11_USAGE_DYNAMIC;
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
