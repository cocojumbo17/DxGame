
Texture2D EarthColorTexture: register(t0);
sampler EarthColorTextureSampler: register(s0);

Texture2D EarthSpecTexture: register(t1);
sampler EarthSpecTextureSampler: register(s1);

Texture2D EarthNightTexture: register(t2);
sampler EarthNightTextureSampler: register(s2);

Texture2D CloudsTexture: register(t3);
sampler CloudsTextureSampler: register(s3);

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
	float m_time;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float4 earth_color = EarthColorTexture.Sample(EarthColorTextureSampler, 1.0-input.texcoord);
	float earth_spec = EarthSpecTexture.Sample(EarthSpecTextureSampler, 1.0 - input.texcoord).r;
	float4 earth_night = EarthNightTexture.Sample(EarthNightTextureSampler, 1.0 - input.texcoord);
	float clouds = CloudsTexture.Sample(CloudsTextureSampler, 1.0 - input.texcoord + float2(m_time/300.0, 0)).r;

	float ka = 1.5;
	float3 ambient_color = float3(0.09, 0.082, 0.082);
	ambient_color *= (earth_color.rgb + clouds);
	float3 ambient = ambient_color * ka;

	float kd = 0.7;
	float3 diffuse_color_day = float3(1.0, 1.0, 1.0);
	diffuse_color_day *= (earth_color.rgb + clouds);

	float3 diffuse_color_night = float3(1.0, 1.0, 1.0);
	diffuse_color_night *= (earth_night.rgb + clouds*0.3);

	float diffuse_amount = dot(m_light_direction.xyz, input.normal);

	float3 diffuse_color = lerp(diffuse_color_night, diffuse_color_day, (diffuse_amount + 1.0) / 2.0);

	float3 diffuse = kd  * diffuse_color;

	float ks = earth_spec;
	float3 specular_color = float3(1.0, 1.0, 1.0);
	float3 reflect_direction = normalize(reflect(m_light_direction.xyz, input.normal));
	float power = 30.0;
	float specular_amount = pow(max(0.0, dot(reflect_direction, input.direction_to_camera)), power);
	float3 specular = ks * specular_amount * specular_color;


	float3 final_color = ambient + diffuse + specular;
	return float4(final_color, 1.0);

	//return Texture.Sample(TextureSampler, input.texcoord*0.5);
}