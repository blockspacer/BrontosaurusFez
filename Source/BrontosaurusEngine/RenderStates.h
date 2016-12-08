#pragma once

enum class eRasterizerState
{
	eDefault,
	eNoCulling,
	eNoCullingClockwise,
	eWireFrame,
	eSize,
};

enum class eBlendState
{
	eAlphaBlend,
	eNoBlend,
	eSize,
};

enum class eDepthStencilState
{
	eDefault,
	eReadOnly,
	eDisableDepth,
	eSize,
};

enum class eSamplerState
{
	eClamp,
	eWrap,
	eSize,
};

