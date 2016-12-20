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
	void SetTarget(CU::Vector3f aTargetPosition);
	inline bool GetIsActive();
	inline bool GetIsSelected();
	inline bool IsInited();
	inline void Select();
	inline void Deselect();
protected:
	virtual void OnActivation();
	virtual void OnDeActivation();
	
	CGameObject* myUser;
	CU::Vector3f myTargetPosition;
	std::function<void(float)> myUpdateFunction;
	bool myIsActive;
	bool myIsSelected;
};


inline bool Skill::GetIsActive()
{
	return myIsActive;
}

inline bool Skill::GetIsSelected()
{
	return myIsSelected;
}

inline bool Skill::IsInited()
{
	if(myUser != nullptr)
	{
		return true;
	}
	return false;
}

inline void Skill::Select()
{
	myIsSelected = true;
}

inline void Skill::Deselect()
{
	myIsSelected = false;
}
