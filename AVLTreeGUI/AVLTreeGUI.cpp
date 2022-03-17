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
HWND g_outputRuntimeButton;
HWND g_messageForOutputButton;

// Property info
HWND g_messageStaticbox0;
HWND g_messageStaticbox1;
HWND g_messageStaticbox2;
HWND g_propertyTextHeader0;
HWND g_propertyTextHeader1;
HWND g_propertyTextHeader2;
HWND g_propertyTextHeader3;
HWND g_propertyTextHeader4;
HWND g_propertyTextValue0;
HWND g_propertyTextValue1;
HWND g_propertyTextValue2;
HWND g_propertyTextValue3;
HWND g_propertuTextValue4;


std::vector<HWND> propertyVec;

// Region to draw
RECT g_canvasRect;
RECT g_propertyRect;

// AVL Tree Initialization
AVLTree t;

// Gdiplus Initialization
Gdiplus::GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken;

// node and its position on screen
std::vector<std::pair<int, Gdiplus::PointF>> nodePosition;

// PointF
Gdiplus::PointF* searchNodePosition = new Gdiplus::PointF;

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

	// Clean up
	delete searchNodePosition;
	searchNodePosition = nullptr;

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

	g_outputRuntimeButton = CreateWindow(L"button", L"Get Runtime Info", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1460, 10, 100, 20, hWnd, (HMENU)IDC_RUNTIME_BUTTON, hInst, NULL);
	SendMessage(g_outputRuntimeButton, WM_SETFONT, (WPARAM)hFont, NULL);

	g_messageForOutputButton = CreateWindow(L"static", L"", WS_CHILD,
		1460, 30, 100, 20, hWnd, nullptr, hInst, NULL);

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
		80, 100, 50, 50, hWnd, nullptr, hInst, NULL);
	SendMessage(g_propertyTextValue0, WM_SETFONT, (WPARAM)hFont, NULL);

	g_propertyTextHeader1 = CreateWindow(L"static", L"Print Preorder: ", WS_CHILD | WS_VISIBLE,
		10, 115, 70, 50, hWnd, nullptr, hInst, NULL);
	SendMessage(g_propertyTextHeader1, WM_SETFONT, (WPARAM)hFont, NULL);

	g_propertyTextValue1 = CreateWindow(L"static", L"NULL", WS_CHILD | WS_VISIBLE,
		80, 115, 1600, 50, hWnd, nullptr, hInst, NULL);
	SendMessageA(g_propertyTextValue1, WM_SETFONT, (WPARAM)hFont, NULL);

	g_propertyTextHeader2 = CreateWindow(L"static", L"Print Postorder: ", WS_CHILD | WS_VISIBLE,
		10, 130, 80, 50, hWnd, nullptr, hInst, NULL);
	SendMessage(g_propertyTextHeader2, WM_SETFONT, (WPARAM)hFont, NULL);

	g_propertyTextValue2 = CreateWindow(L"static", L"NULL", WS_CHILD | WS_VISIBLE,
		80, 130, 1600, 50, hWnd, nullptr, hInst, NULL);
	SendMessage(g_propertyTextValue2, WM_SETFONT, (WPARAM)hFont, NULL);

	g_propertyTextHeader3 = CreateWindow(L"static", L"Print Inorder: ", WS_CHILD | WS_VISIBLE,
		10, 145, 70, 50, hWnd, nullptr, hInst, NULL);
	SendMessage(g_propertyTextHeader3, WM_SETFONT, (WPARAM)hFont, NULL);

	g_propertyTextValue3 = CreateWindow(L"static", L"NULL", WS_CHILD | WS_VISIBLE,
		80, 145, 1600, 50, hWnd, nullptr, hInst, NULL);
	SendMessage(g_propertyTextValue3, WM_SETFONT, (WPARAM)hFont, NULL);

	g_propertyTextHeader4 = CreateWindow(L"static", L"# elements: ", WS_CHILD | WS_VISIBLE,
		10, 160, 100, 50, hWnd, nullptr, hInst, NULL);
	SendMessage(g_propertyTextHeader4, WM_SETFONT, (WPARAM)hFont, NULL);

	g_propertuTextValue4 = CreateWindow(L"static", L"0", WS_CHILD | WS_VISIBLE,
		80, 160, 1600, 50, hWnd, nullptr, hInst, NULL);
	SendMessage(g_propertuTextValue4, WM_SETFONT, (WPARAM)hFont, NULL);

	// Canvas Rect
	g_canvasRect.left = 0;
	g_canvasRect.top = 180;
	g_canvasRect.right = 1600;
	g_canvasRect.bottom = 900;

	// Property Rect
	g_propertyRect.left = 40;
	g_propertyRect.top = 90;
	g_propertyRect.right = 1600;
	g_propertyRect.bottom = 180;

	// Property Vector
	propertyVec.push_back(g_propertyTextValue0);
	propertyVec.push_back(g_propertyTextValue1);
	propertyVec.push_back(g_propertyTextValue2);
	propertyVec.push_back(g_propertyTextValue3);
	propertyVec.push_back(g_propertuTextValue4);

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
	rc.top = 60;
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
		nodePosition.clear();
		searchNodePosition = nullptr;
		insertButtonAction(g_insertTextbox, t, g_messageStaticbox0);
		updateProperty(propertyVec, t);
		InvalidateRect(hWnd, &g_propertyRect, false);
		InvalidateRect(hWnd, &g_canvasRect, false);

		SendMessage(hWnd, WM_PAINT, NULL, NULL);
		break;

	case IDC_SEARCH_BUTTON:
		searchButtonAction(g_searchTextbox, t, g_messageStaticbox2, nodePosition, searchNodePosition);
		InvalidateRect(hWnd, &g_propertyRect, false);
		InvalidateRect(hWnd, &g_canvasRect, false);
		SendMessage(hWnd, WM_PAINT, NULL, NULL);
		break;

	case IDC_REMOVE_BUTTON:
		nodePosition.clear();
		searchNodePosition = nullptr;
		removeButtonAction(g_removeTextbox, t, g_messageStaticbox1);
		InvalidateRect(hWnd, &g_propertyRect, false);
		InvalidateRect(hWnd, &g_canvasRect, false);
		updateProperty(propertyVec, t);
		SendMessage(hWnd, WM_PAINT, NULL, NULL);
		break;

	case IDC_CLEARALL_BUTTON:
		searchNodePosition = nullptr;
		clearButtonAction(g_removeButton, t);
		InvalidateRect(hWnd, &g_propertyRect, false);
		InvalidateRect(hWnd, &g_canvasRect, false);
		updateProperty(propertyVec, t);
		SendMessage(hWnd, WM_PAINT, NULL, NULL);
		break;

	case IDC_RUNTIME_BUTTON:
		outputRuntimeButtonAction(g_outputRuntimeButton, g_messageForOutputButton);

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

Gdiplus::PointF getParentPosition(int currentVal, std::vector<std::pair<int, Gdiplus::PointF>> parentPos)
{
	for (int i = 0; i < parentPos.size(); ++i)
	{
		if (currentVal == parentPos[i].first)
			return parentPos[i].second;
	}
	return Gdiplus::PointF(0, 0);
}

void MemoryBuffer(HWND hWnd, HDC hdc)
{
	// Initalize buffer and main canvas
	Gdiplus::Graphics graphics(hdc);

	Gdiplus::Bitmap* buffer = new Gdiplus::Bitmap(1600, 900, PixelFormat32bppPARGB);
	Gdiplus::Graphics memGraphics(buffer);

	// Initialize materials to draw
	Gdiplus::Pen* blackPen = new Gdiplus::Pen(Gdiplus::Color(0, 0, 0));
	Gdiplus::FontFamily fontFamily(L"Verdana");
	Gdiplus::SolidBrush solidBrush(Gdiplus::Color(0, 0, 0));
	Gdiplus::PointF treeDataPos(775, 200);
	Gdiplus::PointF rootPos(775, 200);
	int offsetX_ellipse = 15;
	int offsetY_ellipse = 10;
	int count = 0; // spaces between child with different parents
	std::vector<std::string> dataPerLevel = t.PrintLevel();

	/*

		Drrrrrrrraaaaaaaaawwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww

	*/

	std::vector<int> dataHeight;
	int level = 0;

	for (int i = 0; i < dataPerLevel.size(); ++i)
	{ // get current level as we draw
		if (dataPerLevel[i] == "/")
			dataHeight.push_back(++level);
	}
	level = 0;

	// This is where we draw
	for (int i = 1; i < dataPerLevel.size() - 1; ++i)
	{
		if (dataPerLevel[i] == "/")
		{ // if it's a new line, update x, y, and level
			rootPos.X -= 150;
			rootPos.Y += 120;
			treeDataPos.X = rootPos.X;
			treeDataPos.Y += 120;
			++level;
			count = 0;
			continue;
		}

		if (dataPerLevel[i] == ".")
		{ // if it's a null node, move along
			if (++count == 2)
			{
				treeDataPos.X += rootPos.X / 3;
				count = 0;
			}
			else
				treeDataPos.X += rootPos.X / 2;
			continue;
		}

		// convert string to wchar* && get parent of the current node
		std::wstring wstr = std::wstring(dataPerLevel[i].begin(), dataPerLevel[i].end());
		int parentValue = t.getParentValue(std::stoi(dataPerLevel[i]));
		Gdiplus::PointF offsetDest(treeDataPos.X + 11, treeDataPos.Y + 40);
		nodePosition.push_back(std::make_pair(std::stoi(dataPerLevel[i]), offsetDest));

		Gdiplus::Font font(&fontFamily, dataPerLevel[i].size() > 1 ? 1.51f * (20 / dataPerLevel[i].size()) : 20, Gdiplus::FontStyleRegular, Gdiplus::Unit::UnitPixel);

		memGraphics.DrawString(wstr.c_str(), -1, &font, treeDataPos, &solidBrush);
		memGraphics.DrawEllipse(blackPen, (int)treeDataPos.X - (int)(dataPerLevel[i].size() > 1 ? offsetX_ellipse - (dataPerLevel[i].size() * 1.8) : offsetX_ellipse), (int)treeDataPos.Y - offsetY_ellipse, 50, 50);

		if (dataPerLevel[i] == dataPerLevel[1]) // if its the root node then we skip so we don't have to draw line
			continue;

		// Draw line connecting the node to its parent
		Gdiplus::PointF offsetSrc(treeDataPos.X + 7, treeDataPos.Y - 10);
		memGraphics.DrawLine(blackPen, offsetSrc, getParentPosition(parentValue, nodePosition));

		// update x pos so we can print horizontally
		if (++count == 2)
		{
			treeDataPos.X += rootPos.X / 3;
			count = 0;
		}
		else
			treeDataPos.X += rootPos.X / 2;

		//treeDataPos.X += (Gdiplus::REAL)550 / dataHeight[level];
	}

	if (searchNodePosition)
	{
		Gdiplus::Pen* redPen = new Gdiplus::Pen(Gdiplus::Color(225, 6, 0), 5.0f);
		memGraphics.DrawEllipse(redPen, (int)searchNodePosition->X - 25, (int)searchNodePosition->Y - 50, 50, 50);
		delete redPen;
		redPen = nullptr;
	}

	// Clear background with white
	graphics.Clear(Gdiplus::Color(255, 255, 255));

	// take drawn buffer image and draw it onto the main window
	graphics.DrawImage(buffer, 0, 0);
	delete buffer;
	buffer = nullptr;
}