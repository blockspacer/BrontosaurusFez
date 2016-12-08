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

	inline CObjectManager& GetObjectManagerReference();

private:
	CObjectManager myObjectManager;
	StateStack myStateStack;
	CSplashScreen* mySplashScreen;

};

inline CObjectManager& CGame::GetObjectManagerReference()
{
	return myObjectManager;
}
