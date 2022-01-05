struct VS_INPUT {
	float4 pos : POSITION0;
	float2 texcoord : TEXCOORD0;
};

struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};

cbuffer constant: register(b0){
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	unsigned int m_time;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.pos = mul(input.pos, m_world);
	output.pos = mul(output.pos, m_view);
	output.pos = mul(output.pos, m_proj);
	output.texcoord = input.texcoord;

	return output;
}