#pragma once
#include "../CommonUtilities/vector2.h"
#include "../CommonUtilities/vector4.h"

class CSprite;


class CSpriteInstance
{
public:
	CSpriteInstance(const char* aTexturePath, const CU::Vector2f& aSize, const CU::Vector2f& aPosition = CU::Vector2f::Zero,
		const CU::Vector4f& aRect = {0.f, 0.f , 1.f , 1.f}, const  CU::Vector4f& aColour = { 1.f, 1.f , 1.f , 1.f }, const bool aShouldAutoSize = false);
	~CSpriteInstance();

	void Render();

	inline void SetPosition(const CU::Vector2f& aPosition);
	inline const CU::Vector2f& GetPosition() const;

	inline void SetRect(const CU::Vector4f& aRect);
	inline const CU::Vector4f& GetRect() const;//mic drop
	
	inline void SetColor(const CU::Vector4f& aColor);
	inline const CU::Vector4f& GetColor() const;

	CU::Vector2f GetTextureSize() const;


private:
	CU::Vector2f myPosition;
	CU::Vector2f mySize;
	CU::Vector4f myRect;
	CU::Vector4f myColor;

	CSprite* mySprite;
};

inline void CSpriteInstance::SetPosition(const CU::Vector2f& aPosition)
{
	myPosition = aPosition;
}

inline const CU::Vector2f& CSpriteInstance::GetPosition() const
{
	return myPosition;
}

inline void CSpriteInstance::SetRect(const CU::Vector4f& aRect)
{
	myRect = aRect;
}

inline const CU::Vector4f& CSpriteInstance::GetRect() const
{
	return myRect;
}

inline void CSpriteInstance::SetColor(const CU::Vector4f& aColor) 
{
	myColor = aColor;
}

inline const CU::Vector4f& CSpriteInstance::GetColor() const
{
	return myColor;
}