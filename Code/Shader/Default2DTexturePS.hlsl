Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer TextureBuffer : register(b4)
{
	float4 color;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};


float4 main(PixelInputType input) : SV_TARGET
{
	float4 textureColor;

	textureColor = shaderTexture.Sample(SampleType, input.tex);

	textureColor = color * textureColor;

	textureColor = saturate(textureColor);

	return textureColor;
}