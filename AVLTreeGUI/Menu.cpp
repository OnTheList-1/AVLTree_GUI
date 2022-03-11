#include "Menu.h"

Menu::Menu()
{
	insertBox = false;
	searchBox = false;
	deleteBox = false;
}

bool Menu::isInsertBox()
{
	return insertBox;
}

bool Menu::isSearchBox()
{
	return searchBox;
}

bool Menu::isDeleteBox()
{
	return deleteBox;
}

void Menu::Draw(const HWND& hwnd, Gdiplus::Graphics& graphics)
{
	Gdiplus::FontFamily fontFamily(L"Verdana");
	Gdiplus::Font font(&fontFamily, 20, Gdiplus::FontStyleRegular, Gdiplus::Unit::UnitPixel);
	Gdiplus::SolidBrush solidBrush(Gdiplus::Color(100, 0, 0));
	Gdiplus::SolidBrush solidBrushSelected(Gdiplus::Color(200, 0, 0));
	Gdiplus::Pen* blackPen = new Gdiplus::Pen(Gdiplus::Color(0, 0, 0));
	Gdiplus::Pen* hoverPen = new Gdiplus::Pen(Gdiplus::Color(200, 0, 0));

	Gdiplus::PointF text1Pos(120, 10);
	Gdiplus::PointF text2Pos(310, 10);
	Gdiplus::PointF text3Pos(500, 10);
	Gdiplus::PointF text4Pos(1500, 10);

	Gdiplus::Rect box1Rect(10, 10, 100, 30);
	Gdiplus::Rect box2Rect(200, 10, 100, 30);
	Gdiplus::Rect box3Rect(390, 10, 100, 30);

	POINT p;
	GetCursorPos(&p);
	ScreenToClient(hwnd, &p);

	graphics.DrawString(L"Insert", -1, &font, text1Pos, &solidBrush);
	graphics.DrawString(L"Search", -1, &font, text2Pos, &solidBrush);
	graphics.DrawString(L"Remove", -1, &font, text3Pos, &solidBrush);
	graphics.DrawString(L"Clear", -1, &font, text4Pos, &solidBrush);

	graphics.DrawRectangle(blackPen, box1Rect);
	graphics.DrawRectangle(blackPen, box2Rect);
	graphics.DrawRectangle(blackPen, box3Rect);

	if (p.x >= 10 && p.x <= 110 && p.y >= 10 && p.y <= 45)
	{
		graphics.DrawRectangle(hoverPen, box1Rect);
		graphics.DrawRectangle(blackPen, box2Rect);
		graphics.DrawRectangle(blackPen, box3Rect);

		insertBox = true;
		searchBox = false;
		deleteBox = false;
	}
	else if (p.x >= 200 && p.x <= 300 && p.y >= 10 && p.y <= 45)
	{
		graphics.DrawRectangle(blackPen, box1Rect);
		graphics.DrawRectangle(hoverPen, box2Rect);
		graphics.DrawRectangle(blackPen, box3Rect);

		insertBox = false;
		searchBox = true;
		deleteBox = false;
	}
	else if (p.x >= 390 && p.x <= 490 && p.y >= 10 && p.y <= 45)
	{
		graphics.DrawRectangle(blackPen, box1Rect);
		graphics.DrawRectangle(blackPen, box2Rect);
		graphics.DrawRectangle(hoverPen, box3Rect);

		insertBox = false;
		searchBox = false;
		deleteBox = true;
	}

	delete blackPen;
	blackPen = nullptr;
}