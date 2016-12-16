#pragma once
#include <unordered_map>
#include <string>
#include "Model.h"
#include "ModelShapes.h"

namespace CU
{
	class ThreadPool;
	class DynamicString;
	//class Matrix44f;
}

class CModel;
class CSprite;
class CModelInstance;
class CSpriteInstance;

class CModelManager
{
public:
	typedef unsigned int ModelId;

	CModelManager();
	~CModelManager();


	/*CModelInstance* CreateInstance(const char* aModelPath);
	CModelInstance* CreateInstance(const eModelShape aShape);
	void CreateInstance(const char* aSpritePath, CSpriteInstance* aSpriteInstance);
	void Render(CU::Matrix44f aSpace, const Lights::SDirectionalLight& aLight, CU::GrowingArray<CPointLightInstance*> aPointLightList);*/

	CModel* LoadModel(const CU::DynamicString& aModelPath);
	CModel* LoadModel(const SShape aModelShape);
	CModel* LoadGUIModel(const CLoaderMesh* aLoaderMesh, const char* aTexturePath);
	CModel* LoadGUIModel(const CLoaderMesh* aLoaderMesh, const CU::GrowingArray<CU::DynamicString>& aTexturePaths);
	
private:

	bool CreateModel(const CU::DynamicString& aModelPath, ModelId aNewModel);
	CModel* CreateShape(const SShape aModelShape);
	
	bool DoesModelExists(const char * aModelPath);

private:

	bool DoesShapeExist(SShape aModelShape);

	std::unordered_map<std::string, ModelId> myModels;
	std::unordered_map<std::string, ModelId> myGUIModels;
	std::unordered_map<eModelShape, ModelId> myShapes;
	std::unordered_map<float, ModelId> myDebugSphereMap;


	CU::ThreadPool* myThreadPool;

	CU::GrowingArray<CModel, ModelId> myDebugModelList;
	CU::GrowingArray<CModel, ModelId> myModelList;
	CU::GrowingArray<CModel, ModelId> myGUIModelList;

	static const unsigned int ourMaxNumberOfModels = 512u;
	static const unsigned int ourMaxNumberOfGUIModels = 64u;
};

