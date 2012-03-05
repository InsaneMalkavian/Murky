#pragma once
#include <D3D11.h>
#include <D3DX11.h>
#include "Utility.h"

namespace fx
{
	namespace SimpleShader {
		extern ID3D11VertexShader*		VS;
        extern ID3D11PixelShader*		PS;
		extern ID3D11InputLayout*		IL;

		//extern ID3D10EffectShaderResourceVariable*	pMainTextureVariable;
		//extern ID3D10EffectTechnique*				pSecondTechnique;
		}
	

	void InitAll(ID3D11Device* device);
	void SetVariable();
	void DestroyAll();
};