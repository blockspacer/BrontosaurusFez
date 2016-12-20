#pragma once
#include <functional>
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
	inline virtual bool GetIsActive();

protected:
	virtual void OnActivation();
	virtual void OnDeActivation();
	
	CGameObject* myUser;
	CGameObject* myTarget;
	std::function<void(float)> myUpdateFunction;
	bool myIsActive;
};


inline bool Skill::GetIsActive()
{
	return myIsActive;
}
