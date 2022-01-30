
Texture2D ColorTexture: register(t0);
sampler ColorTextureSampler: register(s0);

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
	float4 m_light_direction;
	float4 m_camera_position;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float4 color = ColorTexture.Sample(ColorTextureSampler, float2(input.texcoord.x, 1.0 - input.texcoord.y));

	float ka = 1.5;
	float3 ambient_color = float3(0.09, 0.082, 0.082);
	ambient_color *= color.rgb;
	float3 ambient = ambient_color * ka;

	float kd = 0.7;
	float3 diffuse_color = color.rgb;
	float diffuse_amount = dot(m_light_direction.xyz, input.normal);
	float3 diffuse = kd  * diffuse_color * diffuse_amount;

	float ks = 0.0;
	float3 specular_color = float3(1.0, 1.0, 1.0);
	float3 reflect_direction = normalize(reflect(m_light_direction.xyz, input.normal));
	float power = 30.0;
	float specular_amount = pow(max(0.0, dot(reflect_direction, input.direction_to_camera)), power);
	float3 specular = ks * specular_amount * specular_color;


	float3 final_color = ambient + diffuse + specular;
	return float4(final_color, 1.0);
}