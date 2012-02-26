#pragma once
#include <Windows.h>
#include <memory>
#include <d3d11.h>
#include <d3dx11.h>

#include "HardwareInfo.h"
#include "Renderer.h"
#include "StateManager.h"
#include "TextureManager.h"

class MurkyApp {
public:
	MurkyApp(void);
	virtual ~MurkyApp(void);
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice(void);
	HRESULT DestroyDevice(void);
	HRESULT Update(void);
	void Render(void);
protected:
	HWND					hWnd;
	HINSTANCE				hInst;
	HWInformation			hwInfo;
	Renderer*				render;
	StateManager*			stateManager;
	TextureManager*			texManager;
	};