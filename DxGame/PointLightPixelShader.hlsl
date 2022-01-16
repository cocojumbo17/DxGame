
Texture2D ColorTexture: register(t0);
sampler ColorTextureSampler: register(s0);


struct PS_INPUT {
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 world_pos : TEXCOORD1;
};

cbuffer constant: register(b0) {
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	float4 m_light_direction;
	float4 m_camera_position;
	float4 m_light_position;
	float m_light_radius;
	float m_time;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float4 tex_color = ColorTexture.Sample(ColorTextureSampler, float2(input.texcoord.x, 1.0 - input.texcoord.y));

	//ambient light
	float ka = 1.5;
	float3 ambient_color = float3(0.09, 0.082, 0.082);
	ambient_color *= tex_color.rgb;
	float3 ambient = ambient_color * ka;

	//point light params
	float3 light_dir = m_light_position.xyz - input.world_pos.xyz;
	float3 light_direction = normalize(light_dir);
	float distance_to_light = length(light_dir);
	float fade_area = max(0, distance_to_light - m_light_radius);
	float constant = 1.0;
	float lin = 0.7;
	float quad = 0.5;
	float attenuation = constant + lin * fade_area + quad * fade_area * fade_area;


	//diffuse light
	float kd = 0.7;
	float3 diffuse_color = float3(1.0, 1.0, 1.0);
	diffuse_color *= tex_color.rgb;
	float diffuse_amount = dot(light_direction.xyz, input.normal);
	float3 diffuse = (kd  * diffuse_color * diffuse_amount)/attenuation;

	//specular light
	float ks = 0.0;
	float3 specular_color = float3(1.0, 1.0, 1.0);
	float3 direction_to_camera = normalize(input.world_pos.xyz - m_camera_position.xyz);
	float3 reflect_direction = normalize(reflect(light_direction.xyz, input.normal));
	float power = 100.0;
	float specular_amount = pow(max(0.0, dot(reflect_direction, direction_to_camera)), power);
	float3 specular = ks * specular_amount * specular_color;


	float3 final_color = (ambient + diffuse + specular)/attenuation;
	return float4(final_color, 1.0);

	//return Texture.Sample(TextureSampler, input.texcoord*0.5);
}