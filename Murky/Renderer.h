#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <string>

using namespace std;

class Renderer
	{
	friend class MurkyApp;
	public:
		Renderer(void);
		virtual ~Renderer(void);
		HRESULT InitDevice(HWND hWnd);
		HRESULT DestroyDevice(void);
		HRESULT Update(void);
		void Render(void);
		void ClearScene(void);
		void Present(void);
		ID3D11Device* GetDevice(void) {return d3dDevice;}
		ID3D11DeviceContext* GetDeviceContext(void) {return d3dContext;}
		void GetDeviceInfo(DXGI_ADAPTER_DESC &desc);
	private:
		wstring						GetDXFeatureLevel(void);
		D3D_DRIVER_TYPE				driverType;
		D3D_FEATURE_LEVEL			featureLevel;
		ID3D11Device*				d3dDevice;
		ID3D11DeviceContext*		d3dContext;
		IDXGISwapChain*				swapChain;
		ID3D11RenderTargetView*		backBufferTarget;

		ID3D11Texture2D*			mDepthTexture;
        ID3D11DepthStencilView*		mDepthStencilView;

		ID3D11RasterizerState*		mCullNone;
	};

