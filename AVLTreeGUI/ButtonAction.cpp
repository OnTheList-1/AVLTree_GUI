#include "framework.h"
#include "ButtonAction.h"
#include "AVLTree.h"
#include <stdexcept>

void insertButtonAction(HWND g_insertTextbox, AVLTree& t, HWND g_errorStaticbox) //check for max int
{
	if (GetWindowTextLength(g_insertTextbox) == 0 || GetWindowTextLength(g_insertTextbox) > 11)
	{
		ShowWindow(g_errorStaticbox, true);
		return;
	}
	WCHAR buffer[11];
	ZeroMemory(buffer, 11 * sizeof(WCHAR));
	GetWindowText(g_insertTextbox, buffer, GetWindowTextLength(g_insertTextbox) + 1);
	try {
		t.Insert(std::stoi(buffer));
	}
	catch (std::invalid_argument)
	{ // catch for anything that isn't decimal
		ShowWindow(g_errorStaticbox, true);
		return;
	}
	catch (std::out_of_range)
	{ // catch for overflow
		ShowWindow(g_errorStaticbox, true);
		return;
	}
	ShowWindow(g_errorStaticbox, false);
	SetWindowText(g_insertTextbox, L"");

}

void searchButtonAction(HWND g_searchTextbox, AVLTree& t, HWND g_errorStaticbox)
{
	if (GetWindowTextLength(g_searchTextbox) == 0 || GetWindowTextLength(g_searchTextbox) > 11)
	{
		ShowWindow(g_errorStaticbox, true);
		return;
	}

	WCHAR buffer[11];
	ZeroMemory(buffer, 11 * sizeof(WCHAR));
	GetWindowText(g_searchTextbox, buffer, GetWindowTextLength(g_searchTextbox) + 1);
	try {

		bool isFound = t.Find(std::stoi(buffer));
	}
	catch (std::invalid_argument)
	{ // catch for anything that isn't decimal
		ShowWindow(g_errorStaticbox, true);
		return;
	}
	catch (std::out_of_range)
	{ // catch for overflow
		ShowWindow(g_errorStaticbox, true);
		return;
	}
	ShowWindow(g_errorStaticbox, false);
	SetWindowText(g_searchTextbox, L"");

}

void removeButtonAction(HWND g_removeTextbox, AVLTree& t, HWND g_errorStaticbox)
{
	if (GetWindowTextLength(g_removeTextbox) == 0 || GetWindowTextLength(g_removeTextbox) > 11)
	{
		ShowWindow(g_errorStaticbox, true);
		return;
	}
	WCHAR buffer[11];
	ZeroMemory(buffer, 11 * sizeof(WCHAR));
	GetWindowText(g_removeTextbox, buffer, GetWindowTextLength(g_removeTextbox) + 1);
	try {

		t.Remove(std::stoi(buffer));
	}
	catch (std::invalid_argument)
	{ // catch for anything that isn't decimal
		ShowWindow(g_errorStaticbox, true);
		return;
	}
	catch (std::out_of_range)
	{ // catch for overflow
		ShowWindow(g_errorStaticbox, true);
		return;
	}
	ShowWindow(g_errorStaticbox, false);
	SetWindowText(g_removeTextbox, L"");

}

void clearButtonAction(HWND g_clearButton, AVLTree& t)
{
	t.Clear();
}

void updateHeight(HWND g_propertyHeight, AVLTree& t)
{
	WCHAR buffer[11];
	ZeroMemory(buffer, 11 * sizeof(WCHAR));
	//GetWindowText(g_propertyHeight, buffer, GetWindowTextLength(g_propertyHeight) + 1);
	swprintf_s(buffer, L"%d", t.getDepth());
	SetWindowText(g_propertyHeight, L"");
	SetWindowText(g_propertyHeight, buffer);
}