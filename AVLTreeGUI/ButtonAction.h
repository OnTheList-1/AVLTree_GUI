#pragma once

#include "framework.h"
#include "AVLTree.h"

void insertButtonAction(HWND, AVLTree&, HWND);
void searchButtonAction(HWND, AVLTree&, HWND, std::vector<std::pair<int, Gdiplus::PointF>> nodePosition, Gdiplus::PointF*&);
void removeButtonAction(HWND, AVLTree&, HWND);
void clearButtonAction(HWND, AVLTree&);
void outputRuntimeButtonAction(HWND g_outputRuntimeButton, HWND g_messageForOutputButton);
void updateProperty(const std::vector<HWND>&, AVLTree&);
