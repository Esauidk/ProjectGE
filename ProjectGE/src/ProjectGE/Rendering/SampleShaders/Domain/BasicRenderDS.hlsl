struct DS_OUTPUT
{
	float4 vPosition  : SV_POSITION;
	float2 vUV : UV_TEXCOORD;
};

struct HS_CONTROL_POINT_OUTPUT
{
	float3 vPosition : POSITION; 
	float2 vUV : UV_TEXCOORD;
};

struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor;
	float InsideTessFactor			: SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 3

[domain("tri")]
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
	DS_OUTPUT Output;

	Output.vPosition = float4(
		patch[0].vPosition*domain.x+patch[1].vPosition*domain.y+patch[2].vPosition*domain.z,1);
	Output.vUV = patch[0].vUV * domain.x + patch[1].vUV * domain.y;

	return Output;
}
