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

void updateProperty(const std::vector<HWND>& v, AVLTree& t)
{
	WCHAR buffer[11];

	ZeroMemory(buffer, 11 * sizeof(WCHAR));
	swprintf_s(buffer, L"%d", t.Size());
	SetWindowText(v[0], buffer);

	std::string s1 = t.getPreorder();
	std::wstring ws1 = std::wstring(s1.begin(), s1.end());
	SetWindowText(v[1], ws1.c_str());

	std::string s2 = t.getPostorder();
	std::wstring ws2 = std::wstring(s2.begin(), s2.end());
	SetWindowText(v[2], ws2.c_str());

	std::string s3 = t.getInorder();
	std::wstring ws3 = std::wstring(s3.begin(), s3.end());
	SetWindowText(v[3], ws3.c_str());
}

void drawHelper(Gdiplus::Graphics& memGraphics, std::vector<Gdiplus::Status>& ans, Gdiplus::PointF treeDataPos, TreeNode* t, int height)
{
	Gdiplus::FontFamily fontFamily(L"Verdana");
	Gdiplus::Font font(&fontFamily, 20, Gdiplus::FontStyleRegular, Gdiplus::Unit::UnitPixel);
	Gdiplus::SolidBrush solidBrush(Gdiplus::Color(0, 0, 0));

	if (!t)
		return;

	ans.push_back(memGraphics.DrawString(L"0", -1, &font, treeDataPos, &solidBrush));
	if (t->left)
	{
		treeDataPos.X /= height;
		treeDataPos.Y += 100;
		drawHelper(memGraphics, ans, treeDataPos, t->left, height);
	}
	if (t->right)
	{
		treeDataPos.X += treeDataPos.X / height;
		treeDataPos.Y += 100;
		drawHelper(memGraphics, ans, treeDataPos, t->right, height);
	}
}