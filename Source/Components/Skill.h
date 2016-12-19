#pragma once
class CGameObject;

class Skill
{
public:
	Skill();
	~Skill();
	virtual void Activate();
	virtual void Deactivate();
	virtual void Update(float aDeltaTime);
	inline virtual bool GetIsActive();
protected:
	virtual void OnActivation();
	virtual void OnDeActivation();
	
	CGameObject* aUser;
	CGameObject* aTarget;
	bool myIsActive;
};


inline bool Skill::GetIsActive()
{
	return myIsActive;
}
