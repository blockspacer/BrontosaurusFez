// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#define SAFE_DELETE(ptr) delete ptr; ptr = nullptr

#include <utility>

#include "../CommonUtilities/GrowingArray.h"
#include "../CommonUtilities/DL_Debug.h"

#include "../BrontosaurusEngine/Scene.h"
#include "../Game/PlayState.h"

#include "KevinLoader/KevinLoader.h"

#include "LoadManager.h"

//#include "../CommonUtilities/MemoryLeekLogger.h"
// TODO: reference additional headers your program requires here
