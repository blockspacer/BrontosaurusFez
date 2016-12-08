#pragma once
#include "matrix44.h"
#include "InstanceID.h"

namespace CU
{
	class Time;
	class Camera;
}

class CParticleEmitter;
struct SEmitterData;

class CParticleEmitterInstance
{
public:
	CParticleEmitterInstance();
	CParticleEmitterInstance(const SEmitterData& aEmitterData);

	~CParticleEmitterInstance();

	void Update(const CU::Time & aDeltaTime);
	void Render();
	void Activate();
	void Deactivate();
	inline void SetPosition(CU::Vector3f aPosition);
	inline void SetVisibility(bool);
	inline bool IsVisible() const;

	inline InstanceID GetInstanceID();
	inline void SetInstandeID(const InstanceID aID);

private:
	CU::Matrix44f myToWorldSpace;
	CParticleEmitter* myEmitter;
	bool myIsVisible;
	InstanceID myInstanceID;
};

inline void CParticleEmitterInstance::SetPosition(CU::Vector3f aPosition)
{
	myToWorldSpace.SetPosition(aPosition);
}

inline void CParticleEmitterInstance::SetVisibility(bool aValue)
{
	myIsVisible = aValue;
}

inline bool CParticleEmitterInstance::IsVisible() const
{
	return myIsVisible;
}

inline InstanceID CParticleEmitterInstance::GetInstanceID()
{
	return myInstanceID;
}

inline void CParticleEmitterInstance::SetInstandeID(const InstanceID aID)
{
	myInstanceID = aID;
}
