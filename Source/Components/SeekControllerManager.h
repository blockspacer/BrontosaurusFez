#pragma once

class CSeekController;

class CSeekControllerManager
{
public:
	static void Create();
	static CSeekControllerManager& GetInstance();
	static void Destroy();

	void SetTarget();

	CSeekController* CreateAndRegister();

private:
	CSeekControllerManager();
	~CSeekControllerManager();


private:

	static CSeekControllerManager* ourInstance;

	CU::GrowingArray<CSeekController*> myControllers;
};

