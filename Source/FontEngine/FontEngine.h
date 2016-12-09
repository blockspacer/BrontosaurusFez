#pragma once
#include "stdafx.h"

#include "ft2build.h"
#include FT_FREETYPE_H
#include "../CommonUtilities/GrowingArray.h"
#include <map>

class CFT_Font;

class CFontEngine
{
public:
	friend class CFontEngineFacade;

	void Init();
	CFT_Font* GetFace(const char* aFilePath);

private:
	CFT_Font* CreateFace(const char* aFilePath);
	CFontEngine();
	~CFontEngine();

	FT_Library myFreetypeAPI;
	static CFontEngine* ourInstance;

	std::map<std::string, CFT_Font*> myFaces;
};

