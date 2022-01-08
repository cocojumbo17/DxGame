
Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);

struct PS_INPUT {
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 direction_to_camera : TEXCOORD1;
};

cbuffer constant: register(b0) {
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	uint4 temp;
	float4 m_light_direction;
	float4 m_camera_position;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float ka = 0.1;
	float3 ambient_color = float3(1.0, 1.0, 1.0);
	float3 ambient = ambient_color * ka;

	float kd = 0.5;
	float3 diffuse_color = float3(1.0, 1.0, 1.0);
	float diffuse_amount = max(0.0, dot(m_light_direction.xyz, input.normal));
	float3 diffuse = kd  * diffuse_amount * diffuse_color;

	float ks = 1.0;
	float3 specular_color = float3(1.0, 1.0, 1.0);
	float3 reflect_direction = normalize(reflect(m_light_direction.xyz, input.normal));
	float power = 30.0;
	float specular_amount = pow(max(0.0, dot(reflect_direction, input.direction_to_camera)), power);
	float3 specular = ks * specular_amount * specular_color;


	float3 final_color = ambient + diffuse + specular;
	return float4(final_color, 1.0);

	//return Texture.Sample(TextureSampler, input.texcoord*0.5);
}