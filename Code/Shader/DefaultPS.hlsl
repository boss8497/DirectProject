
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer : register(b3)
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
	float padding;
};

cbuffer TextureBuffer : register(b4)
{
	float4 textureBufferColor;
};


struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXTCOORD0;
	float3 normal : NORMAL;
};

float4 main(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;

	//텍스처 좌표 위치에서 샘플러를 사용하여 텍스처에서 픽셀 색상을 샘플링
	textureColor = shaderTexture.Sample(SampleType, input.tex);

	//모든 픽셀의 기본 출력 색상을 주변 공원 값으로 설정
	color = ambientColor;


	/*그냥 좌표면 땅에서 하늘로 쏘는 벡터라서 반전 시키는 거임*/
	//계산을 위해 빛 방향을 반전
	lightDir = -lightDirection;

	/*saturate()라는 함수는 0 이하의 값을 0으로, 1 이상의 값을 1로 바꿔줍니다.*/
	//픽셀의 빛의 양을 계싼
	lightIntensity = saturate(dot(input.normal, lightDir));


	if (lightIntensity > 0.0f)
	{
		//각도가 되면 확산 색과 광 강도의 양에 따라 최종 확산 색을 결정
		color += (diffuseColor * lightIntensity);
	}

	//최종 빛의 색상을 채움
	color = saturate(color);

	//텍스처 픽셀과 최종 확산 색을 곱하여 최종 픽셀 색상 결과를 구함
	color = color * textureColor * textureBufferColor;


	return color;
}