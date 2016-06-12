#pragma once

#include <windows.h>

#include <math.h>
#include <cassert>
#include <crtdbg.h>
#include <shellapi.h>

#include <fcntl.h>

#include <gl/gl.h>
#include <gl/glu.h>

#include "Engine/Utilities/EngineCommon.hpp"
#include "Engine/Utilities/Time.hpp"
#include "Engine/Graphics/Renderer.hpp"
#include "TheGame.hpp"
#include "Engine/LimitSingleInstance.hpp"

#pragma comment( lib, "opengl32" ) // Link in the OpenGL32.lib static library
#pragma comment( lib, "glu32" )

LimitSingleInstance g_SingleInstanceObj(TEXT("Global\\{182a2ccf-5c20-4c43-a4cd-cfc135bf95cd}"));

const double frameDuration = 1.0 / 60.0;
double g_previousFrameDuration = frameDuration;

#define UNUSED(expr) do { (void)(expr); } while (0)



HWND g_hWnd = nullptr;
HDC g_displayDeviceContext = nullptr;
HGLRC g_openGLRenderingContext = nullptr;
const char* APP_NAME = "Alterlife by Christian Walker";
LPWSTR *szArgList;

//-----------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowsMessageHandlingProcedure(HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam)
{
	unsigned char asKey = (unsigned char)wParam;
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(windowHandle, &point);
	Vec2 pos = Vec2(point.x, point.y);
	MouseEvent me;
	me.m_cursorPos = pos;

	if (asKey == VK_ESCAPE)
		g_isQuitting = true;

	switch (wmMessageCode)
	{

	case WM_CLOSE:

	case WM_DESTROY:

	case WM_QUIT:
		g_isQuitting = true;
		return 0;

	case WM_CHAR:
		TheGame::GetInstance().KeyPressEvent(asKey);
		break;

	case WM_KEYDOWN:
		break;

	case WM_KEYUP:
		TheGame::GetInstance().KeyReleaseEvent(asKey);
		break;

	case WM_RBUTTONDOWN:
		me.m_mouseEventType = RIGHT_BUTTON_DOWN;
		TheGame::GetInstance().OnMouseEvent(me);
		break;
	case WM_LBUTTONDOWN:
		me.m_mouseEventType = LEFT_BUTTON_DOWN;
		TheGame::GetInstance().OnMouseEvent(me);
		break;

	case WM_LBUTTONUP:
		me.m_mouseEventType = LEFT_BUTTON_UP;
		TheGame::GetInstance().OnMouseEvent(me);
		break;

	case WM_RBUTTONUP:
		me.m_mouseEventType = RIGHT_BUTTON_UP;
		TheGame::GetInstance().OnMouseEvent(me);
		break;

	case WM_MOUSEWHEEL:
		short result = 0;
		if ((GET_WHEEL_DELTA_WPARAM(wParam)) > 0) result = 1;
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) result = -1;
		break;
	}

	return DefWindowProc(windowHandle, wmMessageCode, wParam, lParam);
}


//-----------------------------------------------------------------------------------------------
void CreateOpenGLWindow(HINSTANCE applicationInstanceHandle)
{

	// Define a window class
	WNDCLASSEX windowClassDescription;
	memset(&windowClassDescription, 0, sizeof(windowClassDescription));
	windowClassDescription.cbSize = sizeof(windowClassDescription);
	windowClassDescription.style = CS_OWNDC; // Redraw on move, request own Display Context
	windowClassDescription.lpfnWndProc = static_cast<WNDPROC>(WindowsMessageHandlingProcedure); // Assign a win32 message-handling function
	windowClassDescription.hInstance = GetModuleHandle(NULL);
	windowClassDescription.hIcon = NULL;
	windowClassDescription.hCursor = NULL;
	windowClassDescription.lpszClassName = TEXT("Simple Window Class");
	RegisterClassEx(&windowClassDescription);

	const DWORD windowStyleFlags = WS_CAPTION | WS_BORDER | WS_THICKFRAME | WS_SYSMENU | WS_OVERLAPPED;
	const DWORD windowStyleExFlags = WS_EX_APPWINDOW;

	RECT desktopRect;
	HWND desktopWindowHandle = GetDesktopWindow();
	GetClientRect(desktopWindowHandle, &desktopRect);

	RECT windowRect = { 50 + 0, 50 + 0, (long)(50 + SCREEN_WIDTH), (long)(50 + SCREEN_HEIGHT) };
	AdjustWindowRectEx(&windowRect, windowStyleFlags, FALSE, windowStyleExFlags);

	WCHAR windowTitle[1024];
	MultiByteToWideChar(GetACP(), 0, APP_NAME, -1, windowTitle, sizeof(windowTitle) / sizeof(windowTitle[0]));
	g_hWnd = CreateWindowExW(
		windowStyleExFlags,
		windowClassDescription.lpszClassName,
		windowTitle,
		windowStyleFlags,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		applicationInstanceHandle,
		NULL);

	ShowWindow(g_hWnd, SW_SHOW);
	SetForegroundWindow(g_hWnd);
	SetFocus(g_hWnd);

	g_displayDeviceContext = GetDC(g_hWnd);

	HCURSOR cursor = LoadCursor(NULL, IDC_ARROW);
	SetCursor(cursor);

	PIXELFORMATDESCRIPTOR pixelFormatDescriptor;
	memset(&pixelFormatDescriptor, 0, sizeof(pixelFormatDescriptor));
	pixelFormatDescriptor.nSize = sizeof(pixelFormatDescriptor);
	pixelFormatDescriptor.nVersion = 1;
	pixelFormatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixelFormatDescriptor.iPixelType = PFD_TYPE_RGBA;
	pixelFormatDescriptor.cColorBits = 24;
	pixelFormatDescriptor.cDepthBits = 24;
	pixelFormatDescriptor.cAccumBits = 0;
	pixelFormatDescriptor.cStencilBits = 8;

	int pixelFormatCode = ChoosePixelFormat(g_displayDeviceContext, &pixelFormatDescriptor);
	SetPixelFormat(g_displayDeviceContext, pixelFormatCode, &pixelFormatDescriptor);
	g_openGLRenderingContext = wglCreateContext(g_displayDeviceContext);
	wglMakeCurrent(g_displayDeviceContext, g_openGLRenderingContext);
	glLineWidth(2.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glEnable(GL_LINE_SMOOTH);

	Renderer& myRenderer = Renderer::GetInstance();
	myRenderer.InitializeAdvancedOpenGLFunctions();

}


void InitializeShaders() {

}


//-----------------------------------------------------------------------------------------------
void RunMessagePump()
{
	MSG queuedMessage;
	for (;;)
	{
		const BOOL wasMessagePresent = PeekMessage(&queuedMessage, NULL, 0, 0, PM_REMOVE);
		if (!wasMessagePresent)
		{
			break;
		}

		TranslateMessage(&queuedMessage);
		DispatchMessage(&queuedMessage);
	}
}

//-----------------------------------------------------------------------------------------------
void Update(double deltaTimeSeconds)
{
	TheGame::GetInstance().Update(deltaTimeSeconds);
}

//-----------------------------------------------------------------------------------------------
void Render()
{
	TheGame::GetInstance().Render();
	SwapBuffers(g_displayDeviceContext);
}

//-----------------------------------------------------------------------------------------------
void RunFrame()
{
	ProfTimer checkTimer;
	Update(g_previousFrameDuration);
	RunMessagePump();
	Render();
	checkTimer.Stop();
	g_previousFrameDuration = checkTimer.GetDurationInSecs();
}

void Initialize(HINSTANCE applicationInstanceHandle) {
	SetProcessDPIAware();
	CreateOpenGLWindow(applicationInstanceHandle);
}

std::string wstrtostr(const std::wstring &wstr)
{
	std::string strTo;
	char *szTo = new char[wstr.length() + 1];
	szTo[wstr.size()] = '\0';
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
	strTo = szTo;
	delete[] szTo;
	return strTo;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	(void)hwnd;
	(void)lParam;
	return 1;
}

//-----------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE applicationInstanceHandle, HINSTANCE, LPSTR commandLineString, int)
{

	if (g_SingleInstanceObj.IsAnotherInstanceRunning()) {
		EnumWindows(EnumWindowsProc, 0);
		return FALSE;
	}


	int argCount;

	std::vector<std::string> args;

	szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);

	std::string argsV = commandLineString;

	if (!(szArgList == NULL) && szArgList[1] != NULL)
	{
		AllocConsole();

		HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
		int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
		FILE* hf_out = _fdopen(hCrt, "w");
		setvbuf(hf_out, NULL, _IONBF, 1);
		*stdout = *hf_out;

		HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
		hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
		FILE* hf_in = _fdopen(hCrt, "r");
		setvbuf(hf_in, NULL, _IONBF, 128);
		*stdin = *hf_in;
	}


	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	//ShowCursor(false);
	(void)commandLineString;
	Initialize(applicationInstanceHandle);

	TheGame::GetInstance().ParseArgumentList(argsV);

	while (!g_isQuitting)
	{
		RunFrame();
	}

	g_isQuitting = true;
//	ReportAllocations();

	/*
	#if defined( _WIN32 ) && defined( _DEBUG )
	assert( _CrtCheckMemory() );
	_CrtDumpMemoryLeaks();
	#endif
	#*/

	return 0;
}
