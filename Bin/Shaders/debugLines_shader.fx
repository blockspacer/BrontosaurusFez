// struct VertexInput_PosColor
// {
//		float4 position : POSITION;
//		float4 color : COLOR;
// };

// struct PixelInput_PosColor
// {
// 		float4 position : SV_POSITION;	
// 		float4 color : COLOR;
// };

// PixelInput_PosColor VS_PosColor(VertexInput_PosColor input)
// {
// 		PixelInput_PosColor output;
// 		output.position = input.position;
// 		output.color = input.color;
// 		return output;
// }

// float4 PS_PosColor(PixelInput_PosColor input) : SV_TARGET
// {
// 		return input.color;
// }