#pragma once

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
	
	bool myIsActive;
};


inline bool Skill::GetIsActive()
{
	return myIsActive;
}
