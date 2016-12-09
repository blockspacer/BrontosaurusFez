#pragma once

#include <Stack.h>
//#include <GrowingArray.h>

#include "Lights.h" //directional //make forward declaration
#include "InstanceID.h"

class CSkybox;
class CModelInstance;
class CPointLightInstance;
class CParticleEmitterInstance;

namespace CU
{
	class Camera;
}


class CScene
{
public:
	CScene();
	~CScene();

	void Update();
	void Render();

	//Add shiz, camera, modeller ljus, partiklar
	InstanceID AddModelInstance(CModelInstance* aModelInstance); //return int
	InstanceID AddDirectionalLight(Lights::SDirectionalLight& aDirectionalLight);
	InstanceID AddPointLightInstance(CPointLightInstance* aPointLight);
	InstanceID AddParticleEmitterInstance(CParticleEmitterInstance* aParticleEmitterInstance);
	InstanceID AddCamera(CU::Camera& aCamera);

	void SetCamera(CU::Camera* aCamera);
	void SetSkybox(const char* aPath);

	CModelInstance& GetModelAt(InstanceID aModelID);

	
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

	CU::GrowingArray<CModelInstance*, InstanceID> myAlphaModels;
	CU::Stack<InstanceID, InstanceID> myFreeAlphaModels;

	CU::GrowingArray<CPointLightInstance*> myPointLights;

	CU::GrowingArray<CParticleEmitterInstance*, InstanceID> myParticleEmitters;
	CU::Stack<InstanceID, InstanceID> myFreeParticleEmitters;

	CU::GrowingArray<CU::Camera, InstanceID> myCameras;
	Lights::SDirectionalLight myDirectionalLight; //make array
	CU::Camera* myActiveCamera;

	CSkybox* mySkybox;

	float myFogStart;
	float myFogEnd;

};

inline CSkybox& CScene::GetSkybox()
{
	assert(mySkybox != nullptr && "Scene skybox is nullo!");
	return *mySkybox;
}
