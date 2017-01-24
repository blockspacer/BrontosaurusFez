#include "stdafx.h"
#include "TextInstance.h"
#include "Text.h"

#include "Renderer.h"

const CU::Vector4f CTextInstance::Red(1, 0, 0, 1);
const CU::Vector4f CTextInstance::Yellow(1, 1, 0, 1);
const CU::Vector4f CTextInstance::Green(0, 1, 0, 1);
const CU::Vector4f CTextInstance::Blue(0, 0, 1, 1);
const CU::Vector4f CTextInstance::Black(0, 0, 0, 1);
const CU::Vector4f CTextInstance::White(1, 1, 1, 1);

CTextInstance::CTextInstance()
	: myText(nullptr)
	, myColor(1, 1, 1, 1)
{
}

CTextInstance::CTextInstance(const CTextInstance& aTextInstance)
{
	*this = aTextInstance;
}

CTextInstance::~CTextInstance()
{
	SAFE_DELETE(myText);
}

void CTextInstance::Init(const CU::DynamicString& aFontPath)
{
	//maybe shouldn't be pointer
	myText = new CCoolText(aFontPath);
}

void CTextInstance::Init(const CU::DynamicString & aFontPath, const int aPixelSize)
{
	myText = new CCoolText(aFontPath,aPixelSize);
}

void CTextInstance::Render()
{
	if (myText != nullptr && myString.Empty() == false)
	{
		SChangeStatesMessage* changeStateMessage = new SChangeStatesMessage();
		changeStateMessage->myBlendState = eBlendState::eAlphaBlend;
		changeStateMessage->myDepthStencilState = eDepthStencilState::eDisableDepth;
		changeStateMessage->myRasterizerState = eRasterizerState::eNoCulling;
		changeStateMessage->mySamplerState = eSamplerState::eClamp;

		CEngine::GetInstance()->GetRenderer().AddRenderMessage(changeStateMessage);

		SRenderTextMessage* renderTextMessage = new SRenderTextMessage();
		renderTextMessage->myColor = myColor;
		renderTextMessage->myPosition = myPosition;
		renderTextMessage->myString = myString;
		renderTextMessage->myText = myText;
		CEngine::GetInstance()->GetRenderer().AddRenderMessage(renderTextMessage);

		changeStateMessage = new SChangeStatesMessage();
		changeStateMessage->myBlendState = eBlendState::eNoBlend;
		changeStateMessage->myDepthStencilState = eDepthStencilState::eDefault;
		changeStateMessage->myRasterizerState = eRasterizerState::eDefault;
		changeStateMessage->mySamplerState = eSamplerState::eClamp;

		CEngine::GetInstance()->GetRenderer().AddRenderMessage(changeStateMessage);
	}
}

float CTextInstance::GetlineHeight() const
{
	return myText->GetlineHeight();
}

CTextInstance& CTextInstance::operator=(const CTextInstance& aTextInstance)
{
	if (myText != nullptr)
	{
		delete myText;
		myText = nullptr;
	}

	if (aTextInstance.myText != nullptr)
	{
		myText = new CCoolText(*aTextInstance.myText);
	}

	myColor = aTextInstance.myColor;
	myPosition = aTextInstance.myPosition;
	myString = aTextInstance.myString;

	return *this;
}
