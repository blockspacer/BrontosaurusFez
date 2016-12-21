#pragma once

#include <Stack.h>
//#include <GrowingArray.h>

#include "Lights.h" //directional //make forward declaration
#include "InstanceID.h"
#include "Camera.h"

class CSkybox;
class CModelInstance;
class CPointLightInstance;
class CParticleEmitterInstance;

namespace CU
{
	class Camera;
	class Time;
}


class CScene
{
public:
	enum class eCameraType
	{
		ePlayerOneCamera,
		eGUICamera,
		eLength
	};

	CScene();
	~CScene();

	void Update(const CU::Time aDeltaTime);
	void Render();

	//Add shiz, camera, modeller ljus, partiklar
	InstanceID AddModelInstance(CModelInstance* aModelInstance); //return int
	InstanceID AddDirectionalLight(Lights::SDirectionalLight& aDirectionalLight);
	InstanceID AddPointLightInstance(CPointLightInstance* aPointLight);
	InstanceID AddParticleEmitterInstance(CParticleEmitterInstance* aParticleEmitterInstance);
	void AddCamera(const eCameraType aCameraType);

	void SetSkybox(const char* aPath);

	CModelInstance& GetModelAt(InstanceID aModelID);
	CU::Camera& GetCamera(const eCameraType aCameraType);
	
	//Delete Shiz here
	void DeleteModelInstance(CModelInstance* anInstance);
	void DeleteModelInstance(const InstanceID& anId);

	inline unsigned int GetModelListSize()   {return myModels.Size();}
	inline unsigned int GetLightListSize()   {return myPointLights.Size();}
	inline unsigned int GetEmitterListSize() {return myParticleEmitters.Size();}
	inline unsigned int GetCameraListSize()  {return myCameras.Size();}

	void DeleteParticleEmitterInstance(const InstanceID anID);

	inline CSkybox& GetSkybox();
	
private:
	//Inget bör vara pekare sen.
	CU::GrowingArray<CModelInstance*, InstanceID> myModels;
	CU::Stack<InstanceID, InstanceID> myFreeModels;

	CU::GrowingArray<CPointLightInstance*> myPointLights;

	CU::GrowingArray<CParticleEmitterInstance*, InstanceID> myParticleEmitters;
	CU::Stack<InstanceID, InstanceID> myFreeParticleEmitters;

	CU::StaticArray<CU::Camera, static_cast<int>(eCameraType::eLength)> myCameras;
	Lights::SDirectionalLight myDirectionalLight; //make array

	CSkybox* mySkybox;

	float myFogStart;
	float myFogEnd;

};

inline CSkybox& CScene::GetSkybox()
{
	assert(mySkybox != nullptr && "Scene skybox is nullo!");
	return *mySkybox;
}
