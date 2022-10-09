Texture2D GroundTexture: register(t0);
sampler GroundTextureSampler: register(s0);

Texture2D Normal: register(t1);
sampler NormalSampler: register(s1);

Texture2D Specular: register(t2);
sampler SpecularSampler: register(s2);

Texture2D GroundTexture2: register(t3);
sampler GroundTextureSampler2: register(s3);

Texture2D Normal2: register(t4);
sampler NormalSampler2: register(s4);

Texture2D Specular2: register(t5);
sampler SpecularSampler2: register(s5);

Texture2D Height: register(t6);
sampler HeightSampler: register(s6);

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
	float m_alpha;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{

	float4 ground_texture = GroundTexture.Sample(GroundTextureSampler,float2(input.texcoord.x,1.0 - input.texcoord.y));
	float4 ground_normal = Normal.Sample(NormalSampler, float2(input.texcoord.x, 1.0 - input.texcoord.y));
	float ground_specular = Specular.Sample(SpecularSampler, 1.0 - input.texcoord).r;

	float4 ground_texture2 = GroundTexture2.Sample(GroundTextureSampler2, float2(input.texcoord.x, 1.0 - input.texcoord.y));
	float4 ground_normal2 = Normal2.Sample(NormalSampler2, float2(input.texcoord.x, 1.0 - input.texcoord.y));
	float ground_specular2 = Specular2.Sample(SpecularSampler2, 1.0 - input.texcoord).r;

	float height_map = Height.Sample(HeightSampler, 1.0 - input.texcoord).r;

	float4 final_texture;
	float4 final_normal;
	float final_specular;

	if (m_alpha*0.8 > height_map)
	{
		final_texture = ground_texture;
		final_normal = ground_normal;
		final_specular = ground_specular;
	}
	else
	{
		final_texture = ground_texture2;
		final_normal = ground_normal2;
		final_specular = ground_specular2;
    }


	final_normal.xyz = (final_normal.xyz * 2.0) - 1.0;
	final_normal.xyz = mul(final_normal.xyz, input.tbn);

	float dot_nl = dot(m_light_direction.xyz, input.tbn[2]);

	//AMBIENT LIGHT-------------------------------------------------------------------

	float ka = 8.5;;
	float3 ia = float3(0.09f,0.082f,0.082f);
	ia *= (final_texture.rgb);

	//final ambient light
	float3 ambient_light = ka * ia;


	//DIFFUSE LIGHT-------------------------------------------------------------------
	float kd;
	if (m_alpha * 0.8 > height_map)
	{
	 kd = 0.8;
	}
	else
	{
	 kd = 1;
	}

	//float amount_diffuse_light = max( 0.0,dot(m_light_direction.xyz,input.normal) );
	float amount_diffuse_light = dot(m_light_direction.xyz, final_normal.xyz);

	float3 id;
	if (m_alpha * 0.8 > height_map)
	{ 
	 id = float3(0.5, 0.5, 0.5);
    }
	else
	{
	 id = float3(1, 1, 1);
	}


	id *= (final_texture.rgb);

	//final diffuse light
	float diffuse_light = kd * id * amount_diffuse_light;

	//SPECULAR LIGHT-------------------------------------------------------------------
	float ks;
	//if (m_alpha > height_map)
	//else ks = 0;
    ks = final_specular;
	float3 is = float3(1,1, 1);
	float3 reflected_light = reflect(m_light_direction.xyz, final_normal.xyz);
	float shininess = 30.0;
	float amount_specular_light = pow(max(0.0, dot(reflected_light, input.direction_to_camera)), shininess);


	//final specular light
	float3 specular_light = ks * amount_specular_light * is;


	float3 final_light = ambient_light + diffuse_light + specular_light;

	return float4(final_light,1.0);
}

