#pragma once
#include <functional>
#include "../CommonUtilities/vector3.h"
class CGameObject;

class Skill
{
public:
	Skill();
	~Skill();
	virtual void Activate();
	virtual void Deactivate();
	virtual void Update(float aDeltaTime);
	virtual void Init(CGameObject* aUser);
	void BasicAttackUpdate(float aDeltaTime); //we probably want to chagne this later
	inline bool GetIsActive();
	inline void SetTarget(CU::Vector3f aTargetPosition);
	inline bool IsInited();
protected:
	virtual void OnActivation();
	virtual void OnDeActivation();
	
	CGameObject* myUser;
	CU::Vector3f myTargetPosition;
	std::function<void(float)> myUpdateFunction;
	bool myIsActive;
};


inline bool Skill::GetIsActive()
{
	return myIsActive;
}

inline void Skill::SetTarget(CU::Vector3f aTargetPosition)
{
	myTargetPosition = aTargetPosition;
}

inline bool Skill::IsInited()
{
	if(myUser != nullptr)
	{
		return true;
	}
	return false;
}