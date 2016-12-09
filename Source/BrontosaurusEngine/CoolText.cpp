#include "stdafx.h"
#include "CoolText.h"
#include "Engine.h"
#include "DXFramework.h"
//#include "../FontEngine/FontEngineFacade.h"


CCoolText::CCoolText(const CU::DynamicString& /*aFontPath*/): myVertexBufferPos(nullptr), myVertexBufferUV(nullptr), myVertexConstantBuffer(nullptr), myPixelConstantBuffer(nullptr)
{
	//myFont = CFontEngineFacade::GetInstance().GetFace(aFontPath.c_str());

	//CreateVertexBuffer(*CEngine::GetInstance()->GetFramework()->GetDevice());
}

CCoolText::~CCoolText()
{
}

void CCoolText::Render(const CU::DynamicString& aString, const CU::Vector2f& /*aPosition*/, const CU::Vector4f& /*aColor*/, const CU::Vector2i& /*aSize*/)
{
	if (!(myLastaString == aString))
	{
		//const std::string narrowString = aString.c_str();
		//
		//const std::wstring wideString(narrowString.begin(), narrowString.end());
		//
		//myBitmap = myFont.RenderText(wideString.c_str(), wideString.length(), aSize);
	}

	
}

