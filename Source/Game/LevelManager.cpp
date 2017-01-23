#include "stdafx.h"
#include <assert.h>
#include "LevelManager.h"
#include "..\PostMaster\PostMaster.h"
#include "..\PostMaster\ChangeLevel.h"
#include "..\PostMaster\Message.h"
#include <CommonUtilities.h>

CLevelManager* CLevelManager::ourInstance = nullptr;

void CLevelManager::CreateInstance()
{
	assert(ourInstance == nullptr && "LevelMgr already exists");
	ourInstance = new CLevelManager();
}

void CLevelManager::DestroyInstance()
{
	SAFE_DELETE(ourInstance);
}

CLevelManager* CLevelManager::GetInstance()
{
	assert(ourInstance != nullptr && "LevelMgr doesn't exists");
	return ourInstance;
}

void CLevelManager::GoToLevel(unsigned char aIndex)
{
	DL_PRINT("LevelIndexChangeTo %u",aIndex);
	PostMaster::GetInstance().SendLetter(Message(eMessageType::eStateStackMessage, ChangeLevel(aIndex)));
}