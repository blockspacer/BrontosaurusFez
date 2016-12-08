#include "stdafx.h"
#include <assert.h>
#include "Model.h"
#include "Engine.h"
#include "DXFramework.h"
#include "Effect.h"
#include "BufferStructs.h"
#include "ConstBufferTemplate.h"
#include <Camera.h>
#include "Surface.h"
#include "CommonUtilities.h"
#include "PointLightInstance.h"
#include "FBXLoader.h"
#include "ModelInstance.h"
#include "Lights.h"
#include "../CommonUtilities/matrix44.h"
#include "../Collision/Intersection.h"
#include <CUTime.h>


CModel::CModel()
	: myConstantBuffers(nullptr)
	, myEffect(nullptr)
	, mySurface(nullptr)
	, myFramework(nullptr)
	, myLODModels(4)
	, myCbuffer(nullptr)
	, myLightBuffer(nullptr)
	, myTimeCBuffer(nullptr)
	, myIsInitialized(false)
	, myIsAlphaModel(false)
	, myVertexBufferSize(0)
{
}

CModel::~CModel()
{
	//TODO Fix this so overloaded = operator can be removed. mebe?

	for (int i = 0; i < myConstantBuffers.Size(); ++i)
	{
		SAFE_RELEASE(myConstantBuffers[i]);
	}

	myFramework = nullptr;

	for (unsigned int i = 0; i < myLODModels.Size(); ++i)
	{
		SAFE_RELEASE(myLODModels[i].myIndexBuffer);
		SAFE_RELEASE(myLODModels[i].myVertexBuffer);
	}

	SAFE_RELEASE(myCbuffer);
	SAFE_RELEASE(myLightBuffer);
	SAFE_RELEASE(myTimeCBuffer);
	myIsInitialized = false;
	myVertexBufferSize = 0;

	SAFE_DELETE(myEffect); //make sure this releases stuff
	SAFE_DELETE(mySurface);
}

bool CModel::Initialize(CEffect* aEffect, CSurface* aSurface)
{
	mySurface = aSurface;
	myFramework = CEngine::GetInstance()->GetFramework();
	myEffect = aEffect;
	myIsInitialized = true;
	InitConstBuffers();

	myAABB.myCenterPos = { 0.0f, 0.0f, 0.0f };

	return true;
}

//	FBX - AssImp
bool CModel::Initialize(CEffect* aEffect, CSurface* aSurface, const CLoaderMesh* aLoadedMesh)
{
	Initialize(aEffect, aSurface);
	myAABB.myMinPos = aLoadedMesh->myMinPoint;
	myAABB.myMaxPos = aLoadedMesh->myMaxPoint;

	myAABB.myRadius = myAABB.myMinPos.Length();
	if (myAABB.myMinPos.Length2() < myAABB.myMaxPos.Length2())
	{
		myAABB.myRadius = myAABB.myMaxPos.Length();
	}

	myIsAlphaModel = aLoadedMesh->myIsAlphaMesh;

	myVertexBufferSize = aLoadedMesh->myVertexBufferSize;
	myLODModels.Add(SLodData());
	return InitBuffers(aLoadedMesh);
}
bool CModel::Initialize(CEffect* aEffect, CSurface* aSurface, const CU::GrowingArray<CLoaderMesh*>& aLoadedMeshList)
{
	Initialize(aEffect, aSurface);
	myAABB.myMinPos = aLoadedMeshList.GetLast()->myMinPoint;
	myAABB.myMaxPos = aLoadedMeshList.GetLast()->myMaxPoint;

	myAABB.myRadius = myAABB.myMinPos.Length();
	if (myAABB.myMinPos.Length2() < myAABB.myMaxPos.Length2())
	{
		myAABB.myRadius = myAABB.myMaxPos.Length();
	}

	myIsAlphaModel = aLoadedMeshList.GetLast()->myIsAlphaMesh;


	myVertexBufferSize = aLoadedMeshList[0]->myVertexBufferSize;

	for (unsigned int i = 0; i < aLoadedMeshList.Size(); ++i)
	{
		myLODModels.Add(SLodData());
		myLODModels.GetLast().myLodLevel = aLoadedMeshList[i]->myLODLevel;



		float temp = SQUARE(aLoadedMeshList[i]->myLOD_DistStart);
		myLODModels.GetLast().myDistances.LOD_DistStart2 = temp * 10;

		temp = SQUARE(aLoadedMeshList[i]->myLOD_DistEnd);
		myLODModels.GetLast().myDistances.LOD_DistEnd2 = temp * 10;

		if (InitBuffers(aLoadedMeshList[i]) == false)
		{
			DL_ASSERT("Failed To create LOD:", i, " from path: ", aLoadedMeshList[i]->myName);
		}
	}

	return true;
}

bool CModel::Initialize(CEffect* aEffect, CSurface* aSurface, CU::GrowingArray<SVertexDataCube>& aVertexList, CU::GrowingArray<unsigned int>& aIndexList)
{
	Initialize(aEffect, aSurface);
	myLODModels.Add(SLodData());
	return InitBuffers(aVertexList, aIndexList);
}
bool CModel::Initialize(CEffect* aEffect, CSurface* aSurface, CU::GrowingArray<SVertexDataCube>& aVertexList)
{
	Initialize(aEffect, aSurface);
	myLODModels.Add(SLodData());
	return InitBuffers(aVertexList);
}

bool CModel::InitBuffers(const CLoaderMesh* aLoadedMesh)
{
	myLODModels.GetLast().myIndexCount = static_cast<unsigned int>(aLoadedMesh->myIndexes.size());
	myLODModels.GetLast().myVertexCount = static_cast<unsigned int>(aLoadedMesh->myVertexCount);


	// VERTEX BUFFER
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = aLoadedMesh->myVertexCount * myVertexBufferSize;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = aLoadedMesh->myVerticies;
	HRESULT result;
	result = DEVICE->CreateBuffer(&vertexBufferDesc, &vertexData, &myLODModels.GetLast().myVertexBuffer);
	CHECK_RESULT(result, "Failed to create vertexbuffer.");


	// INDEX BUFFER
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(int) * myLODModels.GetLast().myIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = &aLoadedMesh->myIndexes[0];
	result = DEVICE->CreateBuffer(&indexBufferDesc, &indexData, &myLODModels.GetLast().myIndexBuffer);
	CHECK_RESULT(result, "Failed to create indexbuffer.");

	return true;
}
bool CModel::InitBuffers(CU::GrowingArray<SVertexDataCube>& aVertexList, CU::GrowingArray<unsigned int>& aIndexList)
{
	myVertexBufferSize = sizeof(SVertexDataCube);
	myLODModels.GetLast().myIndexCount = aIndexList.Size();
	myLODModels.GetLast().myIndexCount = aVertexList.Size();

	// VERTEX BUFFER
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = aVertexList.Size() * myVertexBufferSize;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = &aVertexList[0];
	HRESULT result;
	result = DEVICE->CreateBuffer(&vertexBufferDesc, &vertexData, &myLODModels.GetLast().myVertexBuffer);
	CHECK_RESULT(result, "Failed to create vertexbuffer.");


	// INDEX BUFFER
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(int) * aIndexList.Size();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexData, sizeof(indexData));
	indexData.pSysMem = &aIndexList[0];
	result = DEVICE->CreateBuffer(&indexBufferDesc, &indexData, &myLODModels.GetLast().myIndexBuffer);
	CHECK_RESULT(result, "Failed to create indexbuffer.");




	return true;
}
bool CModel::InitBuffers(CU::GrowingArray<SVertexDataCube>& aVertexList)
{
	myLODModels.Add(SLodData());
	myVertexBufferSize = sizeof(SVertexDataCube);
	myLODModels.GetLast().myVertexCount = aVertexList.Size();

	// VERTEX BUFFER
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = aVertexList.Size() * myVertexBufferSize;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory(&vertexData, sizeof(vertexData));
	vertexData.pSysMem = &aVertexList[0];
	HRESULT result;
	result = DEVICE->CreateBuffer(&vertexBufferDesc, &vertexData, &myLODModels.GetLast().myVertexBuffer);
	CHECK_RESULT(result, "Failed to create vertexbuffer.");

	
	return true;
}

void CModel::Render(const CU::Matrix44f& aToWorldSpace, const CU::Matrix44f& aLastFrameTransformation, const Lights::SDirectionalLight* aLight, const CU::GrowingArray<CPointLightInstance*>* aPointLightList)
{
	myEffect->Activate();

	if (mySurface != nullptr)
		mySurface->Activate();
	
	UpdateCBuffer(aToWorldSpace, aLastFrameTransformation, aLight, aPointLightList);

	UINT stride = myVertexBufferSize;
	UINT offset = 0;

	SLodData& currentLodModel = GetCurrentLODModel(aToWorldSpace.GetPosition());

	DEVICE_CONTEXT->IASetVertexBuffers(0, 1, &currentLodModel.myVertexBuffer, &stride, &offset);
	DEVICE_CONTEXT->IASetIndexBuffer(currentLodModel.myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	if (myLODModels.GetLast().myIndexBuffer == nullptr)
	{
		DEVICE_CONTEXT->Draw(currentLodModel.myVertexCount, 0);
	}
	else
	{
		DEVICE_CONTEXT->DrawIndexed(currentLodModel.myIndexCount, 0, 0);
	}
}

void CModel::UpdateCBuffer(const CU::Matrix44f & aToWorldSpace, const CU::Matrix44f& aLastFrameTransformation, const Lights::SDirectionalLight* aLight, const CU::GrowingArray<CPointLightInstance*>* aPointLightList) // TODO: Do not update some of the cbuffers	
{
	// WorldSpace thingy
	D3D11_MAPPED_SUBRESOURCE mappedSubResource;
	ZeroMemory(&mappedSubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	SToWorldSpace updated;
	updated.myWorldSpaceLastFrame = aLastFrameTransformation;
	updated.myWorldSpace = aToWorldSpace;
	DEVICE_CONTEXT->Map(myCbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
	memcpy(mappedSubResource.pData, &updated, sizeof(SToWorldSpace));
	DEVICE_CONTEXT->Unmap(myCbuffer, 0);
	DEVICE_CONTEXT->VSSetConstantBuffers(1, 1, &myCbuffer);


	//Time uglyness
	//if (myTimeCBuffer != nullptr)
	{
		ZeroMemory(&mappedSubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
		CU::Time dt = ENGINE->GetTime();
		float dtf = dt.GetSeconds();
		CU::Vector4f frameTime(dtf, 1337.0f, 0.0f, 0.0f);
		DEVICE_CONTEXT->Map(myTimeCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
		memcpy(mappedSubResource.pData, &frameTime, sizeof(CU::Vector4f));
		DEVICE_CONTEXT->Unmap(myTimeCBuffer, 0);
		DEVICE_CONTEXT->VSSetConstantBuffers(2, 1, &myTimeCBuffer);
	}








	if (aLight != nullptr && aPointLightList != nullptr)
	{
		//LIGHT

		const CU::GrowingArray<CPointLightInstance*>& pointlightList = *aPointLightList;
		ZeroMemory(&mappedSubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		Lights::SLightsBuffer updatedLights;
		updatedLights.myCameraPos = CAMERA->GetPosition();
		updatedLights.myDirectionalLight.direction = aLight->direction;
		updatedLights.myDirectionalLight.color = aLight->color;

		for (unsigned int i = 0; i < NUMBER_OF_POINTLIGHTS; ++i)
		{
			if (i >= pointlightList.Size())
			{
				updatedLights.myPointLights[i].color = { 0.0f, 0.0f, 0.0f, 0.0f };
				updatedLights.myPointLights[i].position = { 0.0f, 0.0f, 0.0f };
				updatedLights.myPointLights[i].intensity = 0.0f;
				updatedLights.myPointLights[i].range = 0.0f;
			}
			else
			{
				updatedLights.myPointLights[i].color = pointlightList[i]->GetColor();
				updatedLights.myPointLights[i].position = pointlightList[i]->GetPosition();
				updatedLights.myPointLights[i].intensity = pointlightList[i]->GetInstensity();
				updatedLights.myPointLights[i].range = pointlightList[i]->GetRange();
			}
		}

		updatedLights.cubeMap_mipCount = 11; // TODO FIX WTH?!

		DEVICE_CONTEXT->Map(myLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
		memcpy(mappedSubResource.pData, &updatedLights, sizeof(Lights::SLightsBuffer));
		DEVICE_CONTEXT->Unmap(myLightBuffer, 0);
		DEVICE_CONTEXT->PSSetConstantBuffers(0, 1, &myLightBuffer);
	}



	//if (myConstantBuffers[eShaderStage::eVertex] != nullptr)
	//{
	//	myFramework->GetDeviceContext()->VSSetConstantBuffers(2, 1, &myConstantBuffers[eShaderStage::eVertex]);
	//}

	if (myConstantBuffers[static_cast<int>(eShaderStage::ePixel)] != nullptr)
	{
		myFramework->GetDeviceContext()->PSSetConstantBuffers(1, 1, &myConstantBuffers[static_cast<int>(eShaderStage::ePixel)]);
	}

	//if (myConstantBuffers[eShaderStage::eGeometry] != nullptr)
	//{
	//	myFramework->GetDeviceContext()->GSSetConstantBuffers(1, 1, &myConstantBuffers[eShaderStage::eGeometry]);
	//}
}

void CModel::UpdateConstantBuffer(const eShaderStage aShaderStage, const void* aBufferStruct, const unsigned int aBufferSize)
{
	D3D11_MAPPED_SUBRESOURCE mappedSubResource;
	ZeroMemory(&mappedSubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	myFramework->GetDeviceContext()->Map(myConstantBuffers[static_cast<int>(aShaderStage)], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubResource);
	memcpy(mappedSubResource.pData, aBufferStruct, aBufferSize);
	myFramework->GetDeviceContext()->Unmap(myConstantBuffers[static_cast<int>(aShaderStage)], 0);
}


SLodData & CModel::GetCurrentLODModel(const CU::Vector3f& aModelPosition)
{
	if (myLODModels.Size() > 1)
	{
		float distanceToCam2 = Intersection::Distance2Points2(CAMERA->GetPosition(), aModelPosition);
		for (unsigned int i = 0; i < myLODModels.Size(); ++i)
		{
			if (distanceToCam2 >= myLODModels[i].myDistances.LOD_DistStart2 && distanceToCam2 <= myLODModels[i].myDistances.LOD_DistEnd2)
			{
				return myLODModels[i];
			}
		}
	}

	return myLODModels.GetLast();
}

void CModel::InitConstBuffers()
{
	SToWorldSpace bufferData;
	myCbuffer = BSR::CreateCBuffer<SToWorldSpace>(&bufferData);

	Lights::SLightsBuffer light;
	myLightBuffer = BSR::CreateCBuffer<Lights::SLightsBuffer>(&light);

	CU::Vector4f tempTime;
	myTimeCBuffer = BSR::CreateCBuffer<CU::Vector4f>(&tempTime);
}

CModel& CModel::operator=(CModel&& aModel)
{
	SAFE_DELETE(myEffect);
	myEffect = aModel.myEffect;
	aModel.myEffect = nullptr;

	SAFE_DELETE(mySurface);
	mySurface = aModel.mySurface;
	aModel.mySurface = nullptr;

	myFramework = aModel.myFramework;
	aModel.myFramework = nullptr;

	for (unsigned int i = 0; i < myLODModels.Size(); ++i)
	{
		SLodData& lodModel = myLODModels[i];

		SAFE_RELEASE(lodModel.myVertexBuffer);
		SAFE_RELEASE(myLODModels[i].myIndexBuffer);
	}

	myLODModels.RemoveAll();

	for (unsigned int i = 0; i < aModel.myLODModels.Size(); ++i)
	{
		SLodData& otherLodModel = aModel.myLODModels[i];
		
		myLODModels.Add(SLodData());
		SLodData& lodModel = myLODModels.GetLast();

		lodModel.myDistances = otherLodModel.myDistances;

		lodModel.myVertexBuffer = otherLodModel.myVertexBuffer;
		otherLodModel.myVertexBuffer = nullptr;
		myLODModels[i].myIndexBuffer = otherLodModel.myIndexBuffer;
		otherLodModel.myIndexBuffer = nullptr;

		lodModel.myLodLevel = otherLodModel.myLodLevel;
		lodModel.myVertexCount = otherLodModel.myVertexCount;
		lodModel.myIndexCount = otherLodModel.myIndexCount;
	}

	myCbuffer = aModel.myCbuffer;
	aModel.myCbuffer = nullptr;
	myLightBuffer = aModel.myLightBuffer;
	aModel.myLightBuffer = nullptr;
	myTimeCBuffer = aModel.myTimeCBuffer;
	aModel.myTimeCBuffer = nullptr;

	myIsInitialized = aModel.myIsInitialized.load();
	aModel.myIsInitialized = false;

	myVertexBufferSize = aModel.myVertexBufferSize;
	aModel.myVertexBufferSize = 0;

	mySphereColData = std::move(aModel.mySphereColData);

	for (int i = 0; i < myConstantBuffers.Size(); ++i)
	{
		myConstantBuffers[i] = aModel.myConstantBuffers[i];
		aModel.myConstantBuffers[i] = nullptr;
	}

	return *this;
}
CModel& CModel::operator=(const CModel& aModel)
{

	SAFE_DELETE(myEffect);
	if (aModel.myEffect != nullptr)
	{
		myEffect = new CEffect(*aModel.myEffect);
	}

	SAFE_DELETE(mySurface);
	if (aModel.mySurface != nullptr)
	{
		mySurface = new CSurface(*aModel.mySurface);
	}

	myFramework = aModel.myFramework;

	for (unsigned int i = 0; i < myLODModels.Size(); ++i)
	{
		SLodData& lodModel = myLODModels[i];

		SAFE_RELEASE(lodModel.myVertexBuffer);
		SAFE_RELEASE(myLODModels[i].myIndexBuffer);
	}

	myLODModels.RemoveAll();

	for (unsigned int i = 0; i < aModel.myLODModels.Size(); ++i)
	{
		const SLodData& otherLodModel = aModel.myLODModels[i];

		myLODModels.Add(SLodData());
		SLodData& lodModel = myLODModels.GetLast();

		lodModel.myDistances = otherLodModel.myDistances;

		lodModel.myVertexBuffer = otherLodModel.myVertexBuffer;
		SAFE_ADD_REF(lodModel.myVertexBuffer);
		lodModel.myIndexBuffer = otherLodModel.myIndexBuffer;
		SAFE_ADD_REF(lodModel.myIndexBuffer);

		lodModel.myLodLevel = otherLodModel.myLodLevel;
		lodModel.myVertexCount = otherLodModel.myVertexCount;
		lodModel.myIndexCount = otherLodModel.myIndexCount;
	}

	myCbuffer = aModel.myCbuffer;
	SAFE_ADD_REF(myCbuffer);
	myLightBuffer = aModel.myLightBuffer;
	SAFE_ADD_REF(myLightBuffer);
	myTimeCBuffer = aModel.myTimeCBuffer;
	SAFE_ADD_REF(myTimeCBuffer);

	myIsInitialized = aModel.myIsInitialized.load();

	myVertexBufferSize = aModel.myVertexBufferSize;

	mySphereColData = aModel.mySphereColData;

	for (int i = 0; i < myConstantBuffers.Size(); ++i)
	{
		myConstantBuffers[i] = aModel.myConstantBuffers[i];
		SAFE_ADD_REF(myConstantBuffers[i]);
	}

	return *this;



	//if (aModel.myEffect != nullptr)
	//{
	//	if (myEffect != nullptr)
	//	{
	//		delete myEffect;
	//	}
	//	myEffect = new CEffect(*aModel.myEffect);
	//}

	//if (aModel.mySurface != nullptr)
	//{
	//	if (mySurface != nullptr)
	//	{
	//		delete mySurface;
	//	}
	//	mySurface = new CSurface(*aModel.mySurface);
	//}

	//myLODModels = aModel.myLODModels;

	//myFramework = aModel.myFramework;
	//myCbuffer = aModel.myCbuffer;
	//myTimeCBuffer = aModel.myTimeCBuffer;

	//myLightBuffer = aModel.myLightBuffer;

	//myVertexBufferSize = aModel.myVertexBufferSize;

	//myIsInitialized = aModel.myIsInitialized.load();
	//mySphereColData = aModel.mySphereColData;

	//return *this;
}
