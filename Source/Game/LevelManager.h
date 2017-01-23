#pragma once

class CLevelManager
{
public:
	static void CreateInstance();
	static void DestroyInstance();
	static CLevelManager* GetInstance();
	void GoToLevel(unsigned char aIndex);

private:
	CLevelManager() { myLevelIndex = 1; }
	~CLevelManager() {}
	static CLevelManager* ourInstance;

	unsigned char myLevelIndex;
};

