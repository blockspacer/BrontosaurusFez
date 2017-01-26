#pragma once
#include "../CommonUtilities/DynamicString.h"
#include "../CommonUtilities/vector4.h"
#include "../CommonUtilities/vector2.h"
#include "CoolText.h"

class CText;

class CTextInstance
{
public:
	static const CU::Vector4f Red;
	static const CU::Vector4f Yellow;
	static const CU::Vector4f Green;
	static const CU::Vector4f Blue;
	static const CU::Vector4f Black;
	static const CU::Vector4f White;

	CTextInstance();
	CTextInstance(const CTextInstance& aTextInstance);
	~CTextInstance();

	void Init(const CU::DynamicString& aFontPath, const int aPixelSize);
	void Init(const CU::DynamicString& aFontPath = "Default");
	void Render();

	inline void SetPosition(const CU::Vector2f& aPosition);
	inline const CU::Vector2f& GetPosition() const;

	inline void SetColor(const CU::Vector4f& aColor);
	inline const CU::Vector4f& GetColor() const;

	inline void SetText(const CU::DynamicString& aString);
	inline const CU::DynamicString& GetText() const;
	float GetlineHeight() const;

	CU::Vector2f GetQuadSizeNormalized() const;

	CTextInstance& operator=(const CTextInstance& aTextInstance);
private:
	CU::DynamicString myString;
	CU::Vector4f myColor;
	CU::Vector2f myPosition;
	CCoolText* myText;
};

inline void CTextInstance::SetPosition(const CU::Vector2f & aPosition)
{
	myPosition = aPosition;
}

inline const CU::Vector2f & CTextInstance::GetPosition() const
{
	return myPosition;
}

inline void CTextInstance::SetColor(const CU::Vector4f& aColor)
{
	myColor = aColor;
}

inline const CU::Vector4f& CTextInstance::GetColor() const
{
	return myColor;
}

inline void CTextInstance::SetText(const CU::DynamicString& aString)
{
	myString = aString;
}

inline const CU::DynamicString& CTextInstance::GetText() const
{
	return myString;
}
