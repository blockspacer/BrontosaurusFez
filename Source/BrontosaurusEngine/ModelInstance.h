#pragma once
#include "../BrontosaurusEngine/ModelShapes.h"
#include "../CommonUtilities/matrix44.h"
#include "../CommonUtilities/AABB.h"

namespace CU
{
	template <typename ObjectType, typename SizeType = unsigned int, bool USE_SAFE_MODE = true>
	class GrowingArray;
}

namespace Lights
{
	struct SDirectionalLight;
}

class CModel;
class CPointLightInstance;
class CSceneAnimator;

class CModelInstance
{
public:
	friend class CModelManager;
	friend class CModelComponent;

public:
	//CModelInstance();
	~CModelInstance();

	CModelInstance(const char* aModelPath);
	CModelInstance(const char* aModelPath, const CU::Matrix44f& aTransformation);

	CModelInstance(const SShape aShape);
	CModelInstance(const SShape aShape, const CU::Matrix44f& aTransformation);

	CModelInstance(CModel* aModel, const CU::Matrix44f& aTransformation);


	bool IsAlpha();

public:
	bool ShouldRender();
	inline const CU::Matrix44f& GetTransformation() const;
	inline const CU::Matrix44f& GetLastFrameTransformation() const;
	void SetVisibility(const bool aFlag);
	inline bool GetVisibility() { return myIsVisible; }

	//Make GA StaticArray, no need for dynamicArray yo // or make it VectorOnStack?? :) // VectorOnAStick
	void Render(Lights::SDirectionalLight* aLight, CU::GrowingArray<CPointLightInstance*>* aPointLightList);
	void Update(const CU::Time aDeltaTime);

	void SetTransformation(CU::Matrix44f& aTransformation);
	void SetPosition(CU::Vector3f aPosition);
	
	//LAT!
	inline CModel* GetModel();
	CU::AABB GetModelBoundingBox();
private:
	CU::Matrix44f myTransformation;
	CU::Matrix44f myLastFrame;

	CModel* myModel;
	CSceneAnimator* mySceneAnimator;
	float myAnimationCounter;
	bool myIsVisible;
};

inline const CU::Matrix44f& CModelInstance::GetTransformation() const
{
	return myTransformation;
}

inline const CU::Matrix44f & CModelInstance::GetLastFrameTransformation() const
{
	return myLastFrame;
}

inline void CModelInstance::SetVisibility(const bool aFlag)
{
	myIsVisible = aFlag;
}

inline CModel* CModelInstance::GetModel()
{
	return myModel;
}


