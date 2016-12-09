#pragma once
#include "FT_FontFacade.h"

class CFontEngine;
class CFontEngineFacade
{
public:
	CFontEngineFacade();
	~CFontEngineFacade();

	static void CreateInstance();
	static CFontEngineFacade GetInstance();
	static void DestroyInstance();

	void Init();
	CFT_FontFacade GetFace(const char* aFilePath);
private:


	static CFontEngine* ourInstance;
};

