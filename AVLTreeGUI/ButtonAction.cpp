#include "framework.h"
#include "ButtonAction.h"
#include "AVLTree.h"
#include <stdexcept>
#include <fstream>
#include <chrono>

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

void searchButtonAction(HWND g_searchTextbox, AVLTree& t, HWND g_errorStaticbox, std::vector<std::pair<int, Gdiplus::PointF>> nodePosition, Gdiplus::PointF*& searchNodePosition)
{
	if (GetWindowTextLength(g_searchTextbox) == 0 || GetWindowTextLength(g_searchTextbox) > 11)
	{
		ShowWindow(g_errorStaticbox, true);
		return;
	}

	WCHAR buffer[11];
	ZeroMemory(buffer, 11 * sizeof(WCHAR));
	GetWindowText(g_searchTextbox, buffer, GetWindowTextLength(g_searchTextbox) + 1);
	bool isFound;
	try {

		isFound = t.Find(std::stoi(buffer));
		searchNodePosition = nullptr;
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

	if (!isFound)
	{
		ShowWindow(g_errorStaticbox, true);
		SetWindowText(g_errorStaticbox, L"Element not found");
	}
	else
	{
		ShowWindow(g_errorStaticbox, false);
		SetWindowText(g_searchTextbox, L"");
		for (int i = 0; i < nodePosition.size(); ++i)
		{
			if (nodePosition[i].first == std::stoi(buffer))
			{
				searchNodePosition = new Gdiplus::PointF(nodePosition[i].second.X, nodePosition[i].second.Y);
				return;
			}
		}
	}

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

void outputRuntimeButtonAction(HWND g_outputRuntimeButton, HWND g_messageForOutputButton)
{
	std::ofstream file("runtime_info.csv");

	if (file.is_open())
	{
		std::vector<long long> timeVec;
		int count = 1000;
		AVLTree t;

		file << "# elements,Insert,Search,Traverse,Remove\n";

		while (count <= 1000000)
		{
			// Insert
			auto start = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < count; ++i)
			{
				t.Insert(i);
			}
			auto stop = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

			// Search
			auto start1 = std::chrono::high_resolution_clock::now();
			t.Find(rand() % count);
			auto stop1 = std::chrono::high_resolution_clock::now();
			auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1);

			// Traverse;
			auto start2 = std::chrono::high_resolution_clock::now();
			t.getPreorder();
			auto stop2 = std::chrono::high_resolution_clock::now();
			auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2);

			// Remove
			auto start3 = std::chrono::high_resolution_clock::now();
			t.Clear();
			auto stop3 = std::chrono::high_resolution_clock::now();
			auto duration3 = std::chrono::duration_cast<std::chrono::microseconds>(stop3 - start3);

			file << std::to_string(count) <<
				"," << std::to_string(duration.count()) <<
				"," << std::to_string(duration1.count()) <<
				"," << std::to_string(duration2.count()) <<
				"," << std::to_string(duration3.count()) << "\n";
			count += 100;
		}

		ShowWindow(g_messageForOutputButton, true);
		SetWindowText(g_messageForOutputButton, L"file ready");
	}
	else
	{
		ShowWindow(g_messageForOutputButton, true);
		SetWindowText(g_messageForOutputButton, L"Error creating file ...");
	}
	file.close();
}

void updateProperty(const std::vector<HWND>& v, AVLTree& t)
{
	WCHAR buffer[11];

	ZeroMemory(buffer, 11 * sizeof(WCHAR));
	swprintf_s(buffer, L"%d", t.getDepth());
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

	std::string s4 = std::to_string(t.Size());
	std::wstring ws4 = std::wstring(s4.begin(), s4.end());
	SetWindowText(v[4], ws4.c_str());
}
