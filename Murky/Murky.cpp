#include "Murky.h"
#include <DxErr.h>
#include "Logger.h"

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
LRESULT CALLBACK			WndProc(HWND, UINT, WPARAM, LPARAM);

MurkyApp*					murky = NULL;

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    murky = new MurkyApp();

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	if (FAILED(murky->InitWindow(hInstance, nCmdShow))) {
		delete murky;
        return 0;
		}

    if (FAILED(murky->InitDevice())) {
        murky->DestroyDevice();
		delete murky;
        return 0;
    }

	murky->LoadMess();
    // Main message loop
    MSG msg = {0};
    while (WM_QUIT != msg.message) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else {
			murky->Update();
            murky->Render();
        }
    }

	murky->DestroyDevice();
	delete murky;
    
    return (int)msg.wParam;
}

MurkyApp::MurkyApp():hWnd(0), hInst(0), stateManager(NULL), texManager(NULL), sceneManager(NULL) {
	wstring tempVar; // this var is used for misc local purposes
	Log::logger.AddLine("it seems we started");
	hwInfo.GetAllInfo(tempVar);
	Log::logger.AddLine(tempVar);
	render = new Renderer();
	}

MurkyApp::~MurkyApp() {
	delete stateManager;
	delete render;
	delete texManager;
	}

HRESULT MurkyApp::InitDevice(void) {
    render->InitDevice(hWnd);
	texManager = new TextureManager(render->GetDevice());
	stateManager = new StateManager();
	sceneManager = new SceneManager(render);
	return S_OK;
	}

HRESULT MurkyApp::DestroyDevice(void) {
	render->DestroyDevice();
	return S_OK;
	}

HRESULT MurkyApp::Update(void) {
	return S_OK;
	}

void MurkyApp::Render(void) {
	render->ClearScene();
	sceneManager->Render();
	render->Render();
	render->Present();
	}

void MurkyApp::LoadMess(void) {
	sceneManager->Get(L"StaticModel");
	}

//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT MurkyApp::InitWindow(HINSTANCE hInstance, int nCmdShow )
{
    // Register class
	WNDCLASSEX wcex = {0};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    //wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"Murky Application";
    //wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    hInst = hInstance;
    RECT rc = { 0, 0, 800, 480 };
	int wndFullScreen = 0;
	if (wndFullScreen==0) {
		AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
		hWnd = CreateWindow(L"Murky Application", L"Murky Application by InsaneMalkavian", WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInst, NULL);
		}
	else {
		// fullscreen
		hWnd = CreateWindow(L"Murky Application", L"Murky Application by InsaneMalkavian", WS_EX_TOPMOST | WS_POPUP,
                           0, 0, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInst, NULL);
		}
    if( !hWnd )
        return E_FAIL;

    ShowWindow(hWnd, nCmdShow);

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}
