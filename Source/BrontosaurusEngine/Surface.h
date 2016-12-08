#pragma once
#include <GrowingArray.h>

class CDXFramework;
class CTexture;

class CSurface
{
public:
	CSurface(const CU::GrowingArray<const wchar_t*>& aTexturePathList);
	CSurface(const CSurface& aSurface);
	~CSurface();
	void Activate();

	const CU::Vector2ui& GetTextureSize() const;
	CU::Vector2f GetTextureSizeFloat() const;

private:
	CU::GrowingArray<CTexture*> myTextures;
	CDXFramework* myFramework;
};

