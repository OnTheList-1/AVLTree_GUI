#pragma once

#include "framework.h"
#include "AVLTree.h"

void insertButtonAction(HWND, AVLTree&, HWND);
void searchButtonAction(HWND, AVLTree&, HWND);
void removeButtonAction(HWND, AVLTree&, HWND);
void clearButtonAction(HWND, AVLTree&);
void updateHeight(HWND, AVLTree&);