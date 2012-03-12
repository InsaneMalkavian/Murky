#include "Renderer.h"
#include <DxErr.h>
#include <sstream>
#include "Logger.h"
#include "Shaders.h"
#include "Datatypes.h"
#include "Utility.h"

static const wstring WSTR_RENDERER = L"Renderer: ";

Renderer::Renderer(void) {
	backBufferTarget = NULL;
	swapChain = NULL;
	d3dContext = NULL;
	d3dDevice = NULL;
	mDepthTexture = NULL;
	mDepthStencilView = NULL;
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

	DXGI_ADAPTER_DESC desc = {0};
	GetDeviceInfo(desc);
	Log::logger.AddLine(WSTR_RENDERER+L"Device, swapchain and context created!");
	Log::logger.AddLine(WSTR_RENDERER+L"DX FeatureLevel: "+GetDXFeatureLevel());
	
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

	D3D11_TEXTURE2D_DESC depthTexDesc;
    ZeroMemory( &depthTexDesc, sizeof( depthTexDesc ) );
    depthTexDesc.Width = width;
    depthTexDesc.Height = height;
    depthTexDesc.MipLevels = 1;
    depthTexDesc.ArraySize = 1;
    depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthTexDesc.SampleDesc.Count = 1;
    depthTexDesc.SampleDesc.Quality = 0;
    depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
    depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthTexDesc.CPUAccessFlags = 0;
    depthTexDesc.MiscFlags = 0;

    result = d3dDevice->CreateTexture2D(&depthTexDesc, NULL, &mDepthTexture);

    if (FAILED(result)) {
        DXTRACE_MSG(L"Failed to create the depth texture!");
        return false;
    }

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory( &descDSV, sizeof( descDSV ) );
    descDSV.Format = depthTexDesc.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;

    result = d3dDevice->CreateDepthStencilView(mDepthTexture, &descDSV, &mDepthStencilView);

    if (FAILED(result)) {
        DXTRACE_MSG(L"Failed to create the depth stencil target view!");
        return false;
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

	// Initialize all shaders
	fx::InitAll(d3dDevice);

    return S_OK;
	}

HRESULT Renderer::DestroyDevice(void) {
	Util::SafeRelease(mDepthStencilView);
	Util::SafeRelease(mDepthTexture);
	Util::SafeRelease(backBufferTarget);
	Util::SafeRelease(swapChain);
	Util::SafeRelease(d3dContext);
	Util::SafeRelease(d3dDevice);	
	return S_OK;
	}

void Renderer::Render() {
	// this should be removed
	d3dContext->Draw(3, 0);
	}

void Renderer::GetDeviceInfo(DXGI_ADAPTER_DESC &desc) {
	IDXGIDevice * pDXGIDevice;
	d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);
	IDXGIAdapter * pDXGIAdapter;
	pDXGIDevice->GetAdapter(&pDXGIAdapter);
	pDXGIAdapter->GetDesc(&desc);
	}

wstring Renderer::GetDXFeatureLevel(void) {
	const size_t highOffset = 12;
	const size_t lowOffset = 8;
	std::wstringstream streamVal;
	streamVal<<((featureLevel>>highOffset)&0x000f)<<L"."<<((featureLevel>>lowOffset)&0x000f);
	return streamVal.str();
	}


void Renderer::ClearScene(void) {
	if (d3dContext == 0)
        return;

    float clearColor[4] = { 0.0f, 0.0f, 0.25f, 1.0f };
    d3dContext->ClearRenderTargetView(backBufferTarget, clearColor);
	d3dContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

void Renderer::Present(void) {
    swapChain->Present(0, 0);
	}
		