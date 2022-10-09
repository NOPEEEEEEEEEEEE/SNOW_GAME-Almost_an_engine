Texture2D CharacterTexture: register(t0);
sampler CharacterTextureSampler: register(s0);

Texture2D Normal: register(t1);
sampler NormalSampler: register(s1);

Texture2D Specular: register(t2);
sampler SpecularSampler: register(s2);

struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 direction_to_camera: TEXCOORD1;
	row_major float3x3 tbn:TEXCOORD2;

};

cbuffer constant: register(b0)
{
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

	float4 color = CharacterTexture.Sample(CharacterTextureSampler,float2(input.texcoord.x,1.0 - input.texcoord.y));
	if (color.a == 0)return float4(1, 1, 1, 0);
	float4 normal = Normal.Sample(NormalSampler, float2(input.texcoord.x, 1.0 - input.texcoord.y));
	float specular = Specular.Sample(SpecularSampler, 1.0 - input.texcoord).r;

	normal.xyz = (normal.xyz * 2.0) - 1.0;
	normal.xyz = mul(normal.xyz, input.tbn);

	//AMBIENT LIGHT-------------------------------------------------------------------

	float ka = 5.5;;
	float3 ia = float3(0.09f,0.082f,0.082f);
	ia *= (color.rgb);

	//final ambient light
	float3 ambient_light = ka * ia;


	//DIFFUSE LIGHT-------------------------------------------------------------------

	float kd = 0.8;

	float amount_diffuse_light = dot(m_light_direction.xyz, normal.xyz);

	float3 id = float3(1, 1, 1);
	id *= (color.rgb);

	//final diffuse light
	float diffuse_light = kd * id * amount_diffuse_light;

	//SPECULAR LIGHT-------------------------------------------------------------------
	float ks = specular;
	float3 is = float3(0.5,0.5, 0.5);
	float3 reflected_light = reflect(m_light_direction.xyz, normal.xyz);
	float shininess = 5.0;
	float amount_specular_light = pow(max(0.0, dot(reflected_light, input.direction_to_camera)), shininess);


	//final specular light
	float3 specular_light = ks * amount_specular_light * is;


	float3 final_light = ambient_light + diffuse_light + specular_light;
	
		return float4(final_light,1.0);
}

