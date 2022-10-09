Texture2D Scene: register(t0);
sampler SceneSampler: register(s0);

Texture2D HUD: register(t1);
sampler HUDSampler: register(s1);

struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
//	float2 texcoord_HUD: TEXCOORD1;

};


float4 psmain(PS_INPUT input) : SV_TARGET
{
	
	float4 scene_color = Scene.Sample(SceneSampler, input.texcoord);
	float4 HUD_color = HUD.Sample(HUDSampler, input.texcoord);
	int b = 0;
	if (HUD_color.a == 1) b = 0;
	else b = 1;
	float4 final_color = scene_color* b +HUD_color* HUD_color.a;
//	float4 final_color = HUD_color;//* HUD_color.a;

	//if (HUD_color.a == 0)final_color.r = 1;


	return final_color;


}