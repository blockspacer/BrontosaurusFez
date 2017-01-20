#pragma once
class CGameObject;
class CMasterAI
{
public:
	static void Create();

	static void Destroy();

	static CMasterAI& GetInstance();


	const float DetermineHealthDrop();
	void DetermineAmountOfMinionsToSpawn();
	void CallForHelp(CGameObject* aEnemyObject, float aCallForHelpRadius);



private:
	CMasterAI();
	~CMasterAI();



private:
	static CMasterAI* ourInstance;
};

