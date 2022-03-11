#pragma once

#include "Resource.h"
#include "framework.h"

class Menu
{
public:
	Menu();

	bool isInsertBox();
	bool isSearchBox();
	bool isDeleteBox();

	void Draw(const HWND&, Gdiplus::Graphics&);

private:
	bool insertBox;
	bool searchBox;
	bool deleteBox;
};