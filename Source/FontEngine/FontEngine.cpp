#include "stdafx.h"
#include "FontEngine.h"
#include "FT_Font.h"

#ifndef ERROR_CHECK
#define ERROR_CHECK(aError, msg) assert(aError == FT_Err_Ok && msg);
#endif

CFontEngine* CFontEngine::ourInstance = nullptr;


CFontEngine::CFontEngine()
{
	myFreetypeAPI = nullptr;
}


CFontEngine::~CFontEngine()
{
	for (auto i = myFaces.begin(); i != myFaces.end(); ++i)
	{
		delete i->second;
		i->second = nullptr;
	}
}

void CFontEngine::Init()
{
	FT_Error error;
	error = FT_Init_FreeType(&myFreetypeAPI);
	ERROR_CHECK(error, "failed to create FREETYPE2 API");
}

CFT_FontFacade CFontEngine::GetFace(const char* aFilePath)
{
	if (myFaces.count(aFilePath)  == 0)
	{
		myFaces[aFilePath] = CreateFace(aFilePath);
	}
	
	return CFT_FontFacade(myFaces[aFilePath]);
}

CFT_Font* CFontEngine::CreateFace(const char* aFilePath)
{
	CFT_Font* font = new CFT_Font();
	font->myFacePath = aFilePath;

	FT_Error error = 0;
	error = FT_New_Face(myFreetypeAPI, aFilePath, 0, &font->myFace);

	ERROR_CHECK(error, "failed to create FREETYPE2 Face from file");
	return font;
}
