

cbuffer MatrixBuffer
{
	matrix skeletonBase;
	matrix skeletonWorld[50];
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 weight : BLENDWEIGHT;
	int4 seletonIndex : BLENINICES;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output;
	input.position.w = 1.0f;
	/*float4 lPos = float4(0, 0, 0, 0);
	matrix trans = mul(skeletonWorld[input.seletonIndex.x], input.weight[0]);

	for (int i = 0; i < 4; ++i)
	{
		if (input.seletonIndex[i] != -1)
			lPos += input.weight[i] * mul(input.position, skeletonWorld[input.seletonIndex[i]]);
	}*/

	/*if(input.seletonIndex.x != -1)
		trans += mul(skeletonWorld[input.seletonIndex.y], input.weight[1]);
	else
		trans += mul(skeletonBase, input.weight[1]);*/

	//if (input.seletonIndex.x != -1)
	//	trans += mul(skeletonWorld[input.seletonIndex.z], input.weight[2]);
	//else
	//	trans += mul(skeletonBase, input.weight[2]);

	//if (input.seletonIndex.x != -1)
	//	trans += mul(skeletonWorld[input.seletonIndex.w], input.weight[3]);
	//else
	//	trans += mul(skeletonBase, input.weight[3]);
	////for (int i = 1; i < 4; ++i)
	//{
	//	if(input.seletonIndex[i] != -1)
	//		trans += mul(skeletonWorld[input.seletonIndex[i]], 0);
	//	/*else
	//		trans += mul(skeletonBase, input.weight[i]);*/

	//}

	/*matrix bonTransform = mul(skeletonWorld[input.seletonIndex[0]], input.weight[0]);
	bonTransform += mul(skeletonWorld[input.seletonIndex[1]], input.weight[1]);
	bonTransform += mul(skeletonWorld[input.seletonIndex[2]], input.weight[2]);
	bonTransform += mul(skeletonWorld[input.seletonIndex[3]], input.weight[3]);*/


	/*output.position = mul(input.position, bonTransform);
	float weight = 1.0f - input.weight[0] - input.weight[1] - input.weight[2] - input.weight[3];
	lpos *= weight;
	output.position += lpos;*/



	output.position = mul(input.position, skeletonBase);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;

	output.normal = mul(input.normal, (float3x3)worldMatrix);

	output.normal = normalize(output.normal);


	return output;
}