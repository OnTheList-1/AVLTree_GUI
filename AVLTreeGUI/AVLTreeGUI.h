#pragma once

#include "resource.h"

// Window Process Function Declaration
bool OnCreate(HWND, LPCREATESTRUCT);
void OnPaint(HWND);
void OnCommand(HWND, int, HWND, UINT);
void OnDestroy(HWND);

// Function Declaration
void MemoryBuffer(HWND, HDC);