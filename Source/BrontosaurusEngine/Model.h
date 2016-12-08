#pragma once
#include <atomic>
#include "BufferStructs.h"
#include "SphereColData.h"


#include "../CommonUtilities/StaticArray.h"
#include "../CommonUtilities/AABB.h"

class CEffect;
class CSurface;
class CDXFramework;
struct ID3D11Buffer;
struct SVertexData;
struct SConstLayout;
class CLoaderModel;
class CLoaderMesh;

class CModelInstance;
class CModelLoader;

namespace Lights
{
	struct SDirectionalLight;
	class matrix44;
}

namespace CU
{
	class Camera;
}


class CPointLightInstance;

struct SLodData
{
	SLodData() { myLodLevel = myIndexCount = myVertexCount = 0; myVertexBuffer = myIndexBuffer = nullptr; }
	SLODDistances myDistances;		  
	ID3D11Buffer* myVertexBuffer;	  
	ID3D11Buffer* myIndexBuffer;	  
									  
	int myLodLevel;		
	unsigned int myVertexCount;		  
	unsigned int myIndexCount;		  

};


class CModel
{
	friend class CModelLoader;

public:
	enum class eShaderStage
	{
		eVertex,
		eGeometry,
		ePixel,
		eLength
	};

	CModel();
	~CModel();
	//FBX
	bool Initialize(CEffect* aEffect, CSurface* aSurface, const CLoaderMesh* aLoadedMesh);
	bool Initialize(CEffect* aEffect, CSurface* aSurface, const CU::GrowingArray<CLoaderMesh*>& aLoadedMesh);

	bool Initialize(CEffect * aEffect, CSurface * aSurface, CU::GrowingArray<SVertexDataCube>& aVertexList, CU::GrowingArray<unsigned int>& aIndexList);
	bool Initialize(CEffect * aEffect, CSurface * aSurface, CU::GrowingArray<SVertexDataCube>& aVertexList);
	//Shape
	bool Initialize(CEffect* aEffect, CSurface* aSurface);


	void Render(const CU::Matrix44f & aToWorldSpace, const CU::Matrix44f& aLastFrameTransformation, const Lights::SDirectionalLight* aLight, const CU::GrowingArray<CPointLightInstance*>* aPointLightList);
	inline bool GetInitialized() { return myIsInitialized == true; }
	inline const SSphereColData& GetCollisionData();

	void UpdateConstantBuffer(const eShaderStage aShaderStage, const void* aBufferStruct, const unsigned int aBufferSize);
	inline void AddConstantBuffer(const eShaderStage aShaderStage, ID3D11Buffer* aConstantBuffer);
	inline bool HasConstantBuffer(const eShaderStage aShaderStage);

	CModel& operator=(const CModel& aModel);
	CModel& operator=(CModel&& aModel);

	inline const CU::AABB& GetBoundingBox() const;
	inline bool IsAlphaModel();
private:

	SLodData& GetCurrentLODModel(const CU::Vector3f& aModelPosition);

	void InitConstBuffers();
	bool InitBuffers(CU::GrowingArray<SVertexDataCube>& aVertexList, CU::GrowingArray<unsigned int>& aIndexList);
	bool InitBuffers(CU::GrowingArray<SVertexDataCube>& aVertexList);
	bool InitBuffers(const CLoaderMesh * aLoadedMesh);
	void UpdateCBuffer(const CU::Matrix44f & aToWorldSpace, const CU::Matrix44f& aLastFrameTransformation, const Lights::SDirectionalLight* aLight, const CU::GrowingArray<CPointLightInstance*>* aPointLightList);

	inline void SetBoundingBox(const CU::AABB& aAABB);

private:
	CU::StaticArray<ID3D11Buffer*, static_cast<int>(eShaderStage::eLength)> myConstantBuffers;

	CEffect* myEffect;
	CSurface* mySurface;
	CDXFramework* myFramework;

	CU::GrowingArray<SLodData> myLODModels;

	ID3D11Buffer* myCbuffer; // vertexBuffer
	ID3D11Buffer* myLightBuffer; // pixelbuffer
	ID3D11Buffer* myTimeCBuffer; // to vertex Shuld be a buffer that sends every frame along with CameraPosition, and stuffz

	std::atomic_bool myIsInitialized;

	//mebe a radius is enough
	CU::AABB myAABB;


	unsigned int myVertexBufferSize;

	SSphereColData mySphereColData;

	bool myIsAlphaModel;

};
inline const SSphereColData& CModel::GetCollisionData()
{
	return mySphereColData;
}

inline const CU::AABB & CModel::GetBoundingBox() const
{
	return myAABB;
}

inline void CModel::AddConstantBuffer(const eShaderStage aShaderStage, ID3D11Buffer* aConstantBuffer)
{
	myConstantBuffers[static_cast<int>(aShaderStage)] = aConstantBuffer;
}

inline bool CModel::HasConstantBuffer(const eShaderStage aShaderStage)
{
	return myConstantBuffers[static_cast<int>(aShaderStage)] != nullptr;
}

inline void CModel::SetBoundingBox(const CU::AABB & aAABB)
{
	myAABB = aAABB;
}

inline bool CModel::IsAlphaModel()
{
	return myIsAlphaModel;
}

