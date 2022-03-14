// AVLTreeGUI.cpp : Defines the entry point for the application.
//
#include <windowsx.h>

#include "framework.h"
#include "AVLTreeGUI.h"
#include "AVLTree.h"
#include "ButtonAction.h"

#define MAX_LOADSTRING 100

#pragma region Global Variables

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Control handles
HWND g_insertButton;
HWND g_insertTextbox;
HWND g_removeButton;
HWND g_removeTextbox;
HWND g_searchButton;
HWND g_searchTextbox;
HWND g_clearButton;
HWND g_messageStaticbox0;
HWND g_messageStaticbox1;
HWND g_messageStaticbox2;
HWND g_propertyTextHeader0;
HWND g_propertyTextValue0;

// Region to draw
RECT g_canvasRect;
RECT g_propertyRect;

// AVL Tree Initialization
AVLTree t;

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

	case WM_CTLCOLORSTATIC:
		HDC hdcStatic;
		hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, RGB(255, 0, 0));
		SetBkMode(hdcStatic, TRANSPARENT);

		return (LRESULT)GetStockObject(NULL_BRUSH);

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

	// Get system font
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	HFONT hFontError = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, 1000,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	g_insertTextbox = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
		10, 10, 100, 20, hWnd, NULL, hInst, NULL);
	SendMessage(g_insertTextbox, WM_SETFONT, (WPARAM)hFont, NULL);

	g_insertButton = CreateWindow(L"button", L"Insert", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		120, 10, 60, 20, hWnd, (HMENU)IDC_INSERT_BUTTON, hInst, NULL);
	SendMessage(g_insertButton, WM_SETFONT, (WPARAM)hFont, NULL);

	g_removeTextbox = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
		230, 10, 100, 20, hWnd, NULL, hInst, NULL);
	SendMessage(g_removeTextbox, WM_SETFONT, (WPARAM)hFont, NULL);

	g_removeButton = CreateWindow(L"button", L"Remove", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		340, 10, 60, 20, hWnd, (HMENU)IDC_REMOVE_BUTTON, hInst, NULL);
	SendMessage(g_removeButton, WM_SETFONT, (WPARAM)hFont, NULL);

	g_searchTextbox = CreateWindow(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
		450, 10, 100, 20, hWnd, NULL, hInst, NULL);
	SendMessage(g_searchTextbox, WM_SETFONT, (WPARAM)hFont, NULL);

	g_searchButton = CreateWindow(L"button", L"Search", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		560, 10, 60, 20, hWnd, (HMENU)IDC_SEARCH_BUTTON, hInst, NULL);
	SendMessage(g_searchButton, WM_SETFONT, (WPARAM)hFont, NULL);

	g_clearButton = CreateWindow(L"button", L"Clear", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		660, 10, 60, 20, hWnd, (HMENU)IDC_CLEARALL_BUTTON, hInst, NULL);
	SendMessage(g_clearButton, WM_SETFONT, (WPARAM)hFont, NULL);

	g_messageStaticbox0 = CreateWindow(L"static", L"INVALID", WS_CHILD,
		10, 40, 50, 50, hWnd, nullptr, hInst, NULL);
	SendMessage(g_messageStaticbox0, WM_SETFONT, (WPARAM)hFontError, NULL);

	g_messageStaticbox1 = CreateWindow(L"static", L"INVALID", WS_CHILD,
		230, 40, 50, 50, hWnd, nullptr, hInst, NULL);
	SendMessage(g_messageStaticbox1, WM_SETFONT, (WPARAM)hFontError, NULL);

	g_messageStaticbox2 = CreateWindow(L"static", L"INVALID", WS_CHILD,
		450, 40, 60, 50, hWnd, nullptr, hInst, NULL);

	g_propertyTextHeader0 = CreateWindow(L"Static", L"Height: ", WS_CHILD | WS_VISIBLE,
		10, 100, 50, 50, hWnd, nullptr, hInst, NULL);
	SendMessage(g_propertyTextHeader0, WM_SETFONT, (WPARAM)hFont, NULL);

	g_propertyTextValue0 = CreateWindow(L"static", L"0", WS_CHILD | WS_VISIBLE,
		50, 100, 50, 50, hWnd, nullptr, hInst, NULL);
	SendMessage(g_propertyTextValue0, WM_SETFONT, (WPARAM)hFont, NULL);

	// Canvas Rect
	g_canvasRect.left = 0;
	g_canvasRect.top = 200;
	g_canvasRect.right = 1600;
	g_canvasRect.bottom = 900;

	// Property Rect
	g_propertyRect.left = 40;
	g_propertyRect.top = 90;
	g_propertyRect.right = 110;
	g_propertyRect.bottom = 140;

	InvalidateRect(hWnd, NULL, true);

	return true;
}

void OnPaint(HWND hWnd)
{
	// Initialize Paint
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	RECT rc;
	GetClientRect(hWnd, &rc);
	rc.top = 100;
	HDC memdc;
	auto hbuff = BeginBufferedPaint(hdc, &rc, BPBF_COMPATIBLEBITMAP, NULL, &memdc);

	MemoryBuffer(hWnd, memdc);
	EndBufferedPaint(hbuff, TRUE);

	EndPaint(hWnd, &ps);
}

void OnDestroy(HWND hWnd)
{
	PostQuitMessage(0);
}

void OnCommand(HWND hWnd, int id, HWND hwndCtl1, UINT codeNotify)
{
	switch (id)
	{
	case IDC_INSERT_BUTTON:
		insertButtonAction(g_insertTextbox, t, g_messageStaticbox0);
		updateHeight(g_propertyTextValue0, t);
		InvalidateRect(hWnd, &g_propertyRect, false);
		InvalidateRect(hWnd, &g_canvasRect, false);

		SendMessage(hWnd, WM_PAINT, NULL, NULL);
		break;

	case IDC_SEARCH_BUTTON:
		searchButtonAction(g_searchTextbox, t, g_messageStaticbox2);
		SendMessage(hWnd, WM_PAINT, NULL, NULL);
		break;

	case IDC_REMOVE_BUTTON:
		removeButtonAction(g_removeTextbox, t, g_messageStaticbox1);
		InvalidateRect(hWnd, &g_propertyRect, false);
		InvalidateRect(hWnd, &g_canvasRect, false);
		updateHeight(g_propertyTextValue0, t);
		SendMessage(hWnd, WM_PAINT, NULL, NULL);
		break;

	case IDC_CLEARALL_BUTTON:
		clearButtonAction(g_removeButton, t);
		InvalidateRect(hWnd, &g_propertyRect, false);
		InvalidateRect(hWnd, &g_canvasRect, false);
		updateHeight(g_propertyTextValue0, t);
		SendMessage(hWnd, WM_PAINT, NULL, NULL);
		break;
	}
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

	Gdiplus::Pen* blackPen = new Gdiplus::Pen(Gdiplus::Color(0, 0, 0));

	graphics.Clear(Gdiplus::Color(255, 255, 255));

	int height = t.getDepth();

	if (height > 0)
	{
		memGraphics.DrawEllipse(blackPen, 775, 200, 50, 50);

		for (int i = height; i > 0; --i)
		{
			memGraphics.DrawEllipse(blackPen, 775 / (i + 1), 200 * (i + 1), 50, 50);
		}
	}

	//memGraphics.DrawLine(blackPen, 780, 240, 600, 400);
	//memGraphics.DrawLine(blackPen, 820, 240, 1000, 400);

	//memGraphics.DrawEllipse(blackPen, 575, 400, 50, 50);
	//memGraphics.DrawEllipse(blackPen, 975, 400, 50, 50);
	//memGraphics.DrawEllipse(blackPen, 375, 600, 50, 50);
	//memGraphics.DrawEllipse(blackPen, 775, 600, 50, 50);
	//memGraphics.DrawEllipse(blackPen, 1175, 600, 50, 50);

	//memGraphics.DrawLine(blackPen, 580, 440, 400, 600);
	//memGraphics.DrawLine(blackPen, 620, 440, 800, 600);
	//memGraphics.DrawLine(blackPen, 980, 440, 800, 600);
	//memGraphics.DrawLine(blackPen, 1020, 440, 1200, 600);


	graphics.DrawImage(buffer, 0, 0);
	delete buffer;
	buffer = nullptr;
}