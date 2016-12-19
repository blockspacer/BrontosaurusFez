#pragma once

class CStatComponent;
class CModelComponent;

class CHat 
{
public:
	CHat() = default; //wew
	CHat(char* aModelPath, CStatComponent* aStats);
	~CHat();
	const CStatComponent& GetStats();
	void Render();
private:
	CStatComponent* myStatComponent;
	CModelComponent* myModelComponent;
};
