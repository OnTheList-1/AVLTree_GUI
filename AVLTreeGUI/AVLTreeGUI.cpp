// AVLTreeGUI.cpp : Defines the entry point for the application.
//
#include <windowsx.h>

#include "framework.h"
#include "AVLTreeGUI.h"
#include "AVLTree.h"
#include "Menu.h"

#define MAX_LOADSTRING 100

#pragma region Global Variables

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// AVL Tree Initialization
AVLTree t;

// Menu Initialization
Menu menu;

// Gdiplus Initialization
Gdiplus::GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken;

#pragma endregion

#pragma region Foward Declarations

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// Window Process Function Declaration
bool OnCreate(HWND, LPCREATESTRUCT);
void OnPaint(HWND);
void OnCommand(HWND, int, HWND, UINT);
void OnKeyDown(HWND, UINT, BOOL, int, UINT);
void OnKeyUp(HWND, UINT, BOOL, int, UINT);
void OnDestroy(HWND);

// Function Declaration
void MemoryBuffer(HWND, HDC);

#pragma endregion

#pragma region Main Window

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_AVLTREEGUI, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_AVLTREEGUI));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// Shut down Gdiplus
	Gdiplus::GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}

#pragma endregion

#pragma region Initialization and Registration

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_AVLTREEGUI));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_AVLTREEGUI);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}


#pragma endregion

#pragma region Window Process

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

bool OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	RECT rect1;
	GetWindowRect(hWnd, &rect1);

	RECT rect2;
	GetWindowRect(hWnd, &rect2);

	SetWindowPos(
		hWnd,
		NULL,
		100,
		50,
		1600 + ((rect1.right - rect1.left) - (rect2.right - rect2.left)),
		900 + ((rect1.bottom - rect1.top) - (rect2.bottom - rect2.top)),
		NULL
	);


	return true;
}

void OnPaint(HWND hWnd)
{
	// Initialize Paint
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	RECT rc;
	GetClientRect(hWnd, &rc);
	HDC memdc;
	auto hbuff = BeginBufferedPaint(hdc, &rc, BPBF_COMPATIBLEBITMAP, NULL, &memdc);

	MemoryBuffer(hWnd, memdc);
	EndBufferedPaint(hbuff, TRUE);

	EndPaint(hWnd, &ps);
	InvalidateRect(hWnd, nullptr, false);
}

void OnKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{

	InvalidateRect(hwnd, nullptr, false);
}

void OnKeyUp(HWND hwnd, UINT vk, BOOL fUp, int cRepeat, UINT flags)
{
	if (menu.isInsertBox())
	{
		switch (vk)
		{
		case 0x30:
			break;

		case 0x31:
			break;

		case 0x32:
			break;

		case 0x33:
			break;

		case 0x34:
			break;

		case 0x35:
			break;

		case 0x36:
			break;

		case 0x37:
			break;

		case 0x38:
			break;

		case 0x39:
			break;
		}
	}
	else if (menu.isSearchBox())
	{

	}
	else if (menu.isDeleteBox())
	{

	}

	InvalidateRect(hwnd, nullptr, false);

}

void OnDestroy(HWND hWnd)
{
	PostQuitMessage(0);
}

void OnCommand(HWND hWnd, int id, HWND hwndCtl1, UINT codeNotify)
{
}

#pragma endregion

#pragma region Diaglog Box

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

#pragma endregion

void MemoryBuffer(HWND hWnd, HDC hdc)
{
	// Initalize buffer and main canvas
	Gdiplus::Graphics graphics(hdc);

	Gdiplus::Bitmap* buffer = new Gdiplus::Bitmap(1600, 900, PixelFormat32bppPARGB);
	Gdiplus::Graphics memGraphics(buffer);

	// Draw menu
	menu.Draw(hWnd, memGraphics);

	graphics.Clear(Gdiplus::Color(255, 255, 255));
	graphics.DrawImage(buffer, 0, 0);

	delete buffer;
	buffer = nullptr;
}