#include "Renderer.h"
#include <DxErr.h>
#include "Logger.h"

Renderer::Renderer(void) {
	backBufferTarget = 0;
	swapChain = 0;
	d3dContext = 0;
	d3dDevice = 0;
	}


Renderer::~Renderer(void)
	{
	}

HRESULT Renderer::InitDevice(HWND hWnd) {
	HRESULT result = S_FALSE;
    result = DestroyDevice();
	if (FAILED(result)) {
        DXTRACE_MSG(L"Failed to release resources!");
        return S_FALSE;
    }

    RECT dimensions;
    GetClientRect (hWnd, &dimensions);

    unsigned int width = dimensions.right - dimensions.left;
    unsigned int height = dimensions.bottom - dimensions.top;

    D3D_DRIVER_TYPE driverTypes[] = {
        D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE, D3D_DRIVER_TYPE_SOFTWARE
    };

    unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);

    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };

    unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory( &swapChainDesc, sizeof( swapChainDesc ) );
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.Windowed = true;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

    unsigned int creationFlags = 0;

#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    unsigned int driver = 0;

    for (driver = 0; driver < totalDriverTypes; ++driver) {
        result = D3D11CreateDeviceAndSwapChain(0, driverTypes[driver], 0, creationFlags, featureLevels, totalFeatureLevels,
                                                D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &d3dDevice, &featureLevel, &d3dContext);

        if (SUCCEEDED(result)) {
            driverType = driverTypes[driver];
            break;
        }
    }

    if (FAILED(result)) {
        DXTRACE_MSG(L"Failed to create the Direct3D device!");
        return S_FALSE;
    }

    ID3D11Texture2D* backBufferTexture;

    result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);

    if (FAILED(result)) {
        DXTRACE_MSG(L"Failed to get the swap chain back buffer!");
        return S_FALSE;
    }

    result = d3dDevice->CreateRenderTargetView(backBufferTexture, 0, &backBufferTarget);

    if (backBufferTexture)
        backBufferTexture->Release();

    if (FAILED(result)) {
        DXTRACE_MSG(L"Failed to create the render target view!");
        return S_FALSE;
    }

    d3dContext->OMSetRenderTargets(1, &backBufferTarget, 0);

    D3D11_VIEWPORT viewport;
    viewport.Width = static_cast<float>(width);
    viewport.Height = static_cast<float>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    d3dContext->RSSetViewports(1, &viewport);

    return S_OK;
	}

HRESULT Renderer::DestroyDevice(void) {
	if (backBufferTarget) backBufferTarget->Release( );
	if (swapChain) swapChain->Release( );
	if (d3dContext) d3dContext->Release( );
	if (d3dDevice) d3dDevice->Release( );

	backBufferTarget = 0;
	swapChain = 0;
	d3dContext = 0;
	d3dDevice = 0;
	return S_OK;
	}

void Renderer::Render() {
	if (d3dContext == 0)
        return;

    float clearColor[4] = { 0.0f, 0.0f, 0.25f, 1.0f };
    d3dContext->ClearRenderTargetView(backBufferTarget, clearColor);

    swapChain->Present( 0, 0 );
	}
