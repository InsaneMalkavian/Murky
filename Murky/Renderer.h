#pragma once
#include <d3d11.h>
#include <d3dx11.h>
class Renderer
	{
	public:
		Renderer(void);
		virtual ~Renderer(void);
		ID3D11Device* GetDevice(void) {return d3dDevice;}
		HRESULT InitDevice(HWND hWnd);
		HRESULT DestroyDevice(void);
		HRESULT Update(void);
		void Render(void);
	private:
		D3D_DRIVER_TYPE				driverType;
		D3D_FEATURE_LEVEL			featureLevel;
		ID3D11Device*				d3dDevice;
		ID3D11DeviceContext*		d3dContext;
		IDXGISwapChain*				swapChain;
		ID3D11RenderTargetView*		backBufferTarget;
	};

