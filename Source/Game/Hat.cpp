#include "stdafx.h"
#include "Hat.h"
#include <TextureManager.h>


CHat::CHat(char* aTexturePath, CStatComponent* aStats)
{
	CTextureManager texMgr;
	myTexture = &texMgr.LoadTexture(aTexturePath);
}

CHat::~CHat()
{
}
