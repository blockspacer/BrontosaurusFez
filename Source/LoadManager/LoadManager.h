#pragma once

class CScene;
class CPlayState;

#define LOAD_MGR LoadManager::GetInstance()

class LoadManager
{
public:
	
	static void CreateInstance();
	static LoadManager& GetInstance();
	static void DestroyInstance();

	void SetCurrentScene(CScene* aScene);
	CScene* GetCurrentScene() const;

	void SetCurrentPlayState(CPlayState* aPlaystate);
	CPlayState* GetCurrentPLaystate() const;

private:
	LoadManager();
	~LoadManager();

	static LoadManager* ourInstance;

	CScene* myCurrentScene;
	CPlayState* myCurrentPlaystate;
};

