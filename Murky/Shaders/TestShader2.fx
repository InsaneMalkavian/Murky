//--------------------------------------------------------------------------------------
// File: TestShader2.fx
//
// Copyright (c) InsaneMalkavian. All rights reserved. 2012
//--------------------------------------------------------------------------------------

struct VS_INPUT {
	float3 Pos : POSITION;
	float3 Nor : NORMAL;
	float2 Tex : TEXCOORD;
};

struct PS_INPUT {
	float4 Pos : SV_POSITION;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
float4 VS( VS_INPUT vs_In ) : SV_POSITION
{
	vs_In.Pos.z-=10.5f;
    return float4(vs_In.Pos,1.0f);
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( float4 Pos : SV_POSITION ) : SV_Target
{
    return float4( 1.0f, 1.0f, 1.0f, 1.0f );
}
