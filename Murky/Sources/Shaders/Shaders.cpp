#include "Shaders.h"
#include <D3Dcompiler.h>
#include <D3D11Shader.h>

using namespace fx;

ID3D11VertexShader*	SimpleShader::VS = 0;
ID3D11PixelShader*	SimpleShader::PS = 0;
ID3D11InputLayout*	SimpleShader::IL = 0;

static const wstring SHADERS_PATH = L"Shaders\\";

int CompileShader(const wstring filePath, const char* entry, const char* shaderModel, ID3DBlob** buffer);
HRESULT CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout);

void fx::InitAll(ID3D11Device* device) {
	ID3DBlob* vsBuffer = 0;

    int compileResult = CompileShader(L"Color.fx", "VS", "vs_4_0", &vsBuffer );

    if (compileResult != 0) {
        DXTRACE_MSG(L"Error compiling the vertex shader!");
        return;
    }

    HRESULT hr;

    hr = device->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), 0, &SimpleShader::VS);

    if (FAILED(hr)) {
        DXTRACE_MSG(L"Error creating the vertex shader!");
		if (vsBuffer)
			vsBuffer->Release();
        return;
    }

	// here we can create inputlayout manually or automatically
	hr = CreateInputLayoutDescFromVertexShaderSignature(vsBuffer, device, &SimpleShader::IL);

    vsBuffer->Release();

    if (FAILED(hr)) {
        DXTRACE_MSG(L"Error creating the input layout!");
        return;
    }

    ID3DBlob* psBuffer = 0;

    compileResult = CompileShader(L"Color.fx", "PS", "ps_4_0", &psBuffer);

    if (compileResult != 0) {
        DXTRACE_MSG(L"Error compiling pixel shader!");
        return;
    }

	hr = device->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), 0, &SimpleShader::PS);

    psBuffer->Release();

    if (FAILED(hr)) {
        DXTRACE_MSG(L"Error creating pixel shader!");
        return;
    }
	}

int CompileShader(const wstring filePath, const char* entry, const char* shaderModel, ID3DBlob** buffer) {
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )
    shaderFlags |= D3DCOMPILE_DEBUG;
#endif

    ID3DBlob* errorBuffer = 0;
    HRESULT result;

    result = D3DX11CompileFromFile((SHADERS_PATH+filePath).c_str(), 0, 0, entry, shaderModel, shaderFlags, 0, 0, buffer, &errorBuffer, 0);

    if (FAILED(result)) {
        if (errorBuffer != 0) {
            OutputDebugStringA((char*)errorBuffer->GetBufferPointer());
            errorBuffer->Release();
        }
        return -1;
    }
    
    if (errorBuffer != 0)
        errorBuffer->Release();

	return 0;
	}

HRESULT CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout) {
	// Reflect shader info
	ID3D11ShaderReflection* pVertexShaderReflection = NULL;	
	if (FAILED(D3DReflect(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**) &pVertexShaderReflection)))  {
		return S_FALSE;
	}
	
	// Get shader info
	D3D11_SHADER_DESC shaderDesc;
	pVertexShaderReflection->GetDesc( &shaderDesc );
	
	// Read input layout description from shader info
	size_t byteOffset = 0;
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for (size_t i=0; i< shaderDesc.InputParameters; i++) {
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;		
		pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc );

		// fill out input element desc
		D3D11_INPUT_ELEMENT_DESC elementDesc;	
		elementDesc.SemanticName = paramDesc.SemanticName;		
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = byteOffset;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;	

		// determine DXGI format
		if (paramDesc.Mask == 1) {
			if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			byteOffset += 4;
		}
		else if ( paramDesc.Mask <= 3 ) {
			if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			byteOffset += 8;
		}
		else if ( paramDesc.Mask <= 7 ) {
			if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			byteOffset += 12;
		}
		else if ( paramDesc.Mask <= 15 ) {
			if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if ( paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32 ) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			byteOffset += 16;
		}
			
		//save element desc
		inputLayoutDesc.push_back(elementDesc);
	}		

	// Try to create Input Layout
	HRESULT hr = pD3DDevice->CreateInputLayout( &inputLayoutDesc[0], inputLayoutDesc.size(), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pInputLayout );

	//Free allocation shader reflection memory
	pVertexShaderReflection->Release();
	return hr;
}