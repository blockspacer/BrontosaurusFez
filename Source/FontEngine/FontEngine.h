#pragma once
#include "stdafx.h"

#include "ft2build.h"
#include FT_FREETYPE_H
#include "../CommonUtilities/GrowingArray.h"
#include <map>
#include "FT_FontFacade.h"

//#pragma comment(lib, "C:\\Users\\sthahu2\\Documents\\Project\\BrontosaurusFez\\LibDependencies\\FreeType\\freetype27MT.lib")

class CFT_Font;

class CFontEngine
{
public:
	friend class CFontEngineFacade;

	void Init();
	CFT_FontFacade GetFace(const char* aFilePath);

private:
	CFT_Font* CreateFace(const char* aFilePath);
	CFontEngine();
	~CFontEngine();

	FT_Library myFreetypeAPI;
	static CFontEngine* ourInstance;

	std::map<std::string, CFT_Font*> myFaces;
};

