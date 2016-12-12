#include "stdafx.h"

#include "../Components/ObjectManager.h"
#include "../StateStack/StateStack.h"
#include "BackgroundLoadingManager.h"

namespace CU
{
	class Time;
}

class CSplashScreen;

class CGame
{
public:
	CGame();
	~CGame();

	void Init();
	void Update(const CU::Time& aDeltaTime);
	void Render();

	inline CGameObjectManager& GetObjectManagerReference();

private:
	CGameObjectManager myObjectManager;
	StateStack myStateStack;
	CSplashScreen* mySplashScreen;

};

inline CGameObjectManager& CGame::GetObjectManagerReference()
{
	return myObjectManager;
}
