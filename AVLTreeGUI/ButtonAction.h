#pragma once

#include "framework.h"
#include "AVLTree.h"

void insertButtonAction(HWND, AVLTree&, HWND);
void searchButtonAction(HWND, AVLTree&, HWND);
void removeButtonAction(HWND, AVLTree&, HWND);
void clearButtonAction(HWND, AVLTree&);
void updateProperty(const std::vector<HWND>&, AVLTree&);
