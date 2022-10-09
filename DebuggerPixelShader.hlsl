struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
};

cbuffer constant: register(b0)
{
	
	float4 m_color;
	
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float4 color = m_color;

	return color;

}