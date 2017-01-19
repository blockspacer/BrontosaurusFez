#pragma once
class CMasterAI
{
public:
	static void Create();

	static void Destroy();

	static CMasterAI& GetInstance();


	const float DetermineHealthDrop();
	void DetermineAmountOfMinionsToSpawn();




private:
	CMasterAI();
	~CMasterAI();



private:
	static CMasterAI* ourInstance;
};

