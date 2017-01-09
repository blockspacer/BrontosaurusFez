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
	void SetTargetPosition(CU::Vector3f aTargetPosition);
	void SetTargetObject(CGameObject* aTargetObject);
	void ActivateCollider();
	inline const bool GetIsActive() const;
	inline const bool GetIsSelected() const;
	inline bool IsInited();
	void Select();
	void Deselect();
protected:
	virtual void OnActivation();
	virtual void OnDeActivation();
	
	CGameObject* myUser;
	CGameObject* myColliderObject;
	CGameObject* myTargetObject;
	CU::Vector3f myTargetPosition;
	std::function<void(float)> myUpdateFunction;
	float myRange;
	float myCoolDown;
	float myAttackRadius2;
	float myElapsedCoolDownTime;
	bool myIsActive;
	bool myIsSelected;
};


inline const bool Skill::GetIsActive() const
{
	return myIsActive;
}

inline const bool Skill::GetIsSelected() const
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

