#include <iostream>
#include <windows.h>
#include <cmath>
#include <chrono>

HWND consoleHandle;
HDC hdc;
POINT op;
HPEN pen;

void hidecursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void initGraph()
{
	// Get window handle to console, and device context
	consoleHandle = GetConsoleWindow();
	hdc = GetDC(consoleHandle);
	hidecursor();
}

void closeGraph()
{
	ReleaseDC(consoleHandle, hdc);
	std::cin.ignore();
}

void line(double x1, double y1, double x2, double y2)
{
	MoveToEx(hdc, x1, y1, &op);
	LineTo(hdc, x2, y2);
}

void point(double x, double y)
{
	SetPixel(hdc, x, y, RGB(255, 255, 255));
}

void point(double x, double y, COLORREF color)
{
	SetPixel(hdc, x, y, color);
}

void setBrush(int style, int width, COLORREF color, COLORREF bgcolor)
{
	pen = CreatePen(style, width, color);
	SetBkColor(hdc, bgcolor);
	SelectObject(hdc, pen);
}

void setBrush(int style, int width, COLORREF color)
{
	setBrush(style, width, color, RGB(0, 0, 0));
}

void drawTextColored(std::string text, double x, double y, COLORREF color)
{
	SetTextColor(hdc, color);
	RECT textRect;
	SetRect(&textRect, x, y, x, y);

	DrawText(hdc, std::wstring(text.begin(), text.end()).c_str(), -1, &textRect, DT_TOP | DT_LEFT | DT_NOCLIP);
}

void drawText(std::string text, double x, double y, COLORREF color)
{
	SetBkMode(hdc, TRANSPARENT);
	drawTextColored(text, x, y, color);
}

void drawText(std::string text, double x, double y, COLORREF color, COLORREF bgcolor)
{
	SetBkColor(hdc, bgcolor);
	drawTextColored(text, x, y, color);
}

void ellipse(double x, double y, double radiusH, double radiusV, bool isFilled = true)
{
	if (!isFilled)
	{
		HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
		Ellipse(hdc, x - radiusH, y - radiusV, x + radiusH, y + radiusV);
		SelectObject(hdc, oldbrush);
	}
	else
		Ellipse(hdc, x - radiusH, y - radiusV, x + radiusH, y + radiusV);
}

void circle(double x, double y, double radius, bool isFilled = true)
{
	if (!isFilled)
	{
		HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
		Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);
		SelectObject(hdc, oldbrush);
	}
	else
		Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);	
}

void rectangle(double x, double y, double width, double height)
{
	HBRUSH oldbrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
	Rectangle(hdc, x, y, x + width, y + height);
	SelectObject(hdc, oldbrush);
}

void rectangle(double x, double y, double width, double height, COLORREF color)
{
	RECT rect = { x, y, x + width, y + height };
	HBRUSH brush = CreateSolidBrush(color);

	FillRect(hdc, &rect, brush);

	DeleteObject(brush);
}


int main()
{
	initGraph();
	//-------------------------------

	int delay = 250;

	point(20, 30);
	Sleep(delay);

	point(25, 30, RGB(255, 55, 55));
	Sleep(delay);

	for (int i = 0; i < 5; i++)
	{
		setBrush(i, 1, RGB(255, 0, 0));
		line(50 + i * 10, 50, 200 + i * 10, 300);
	}

	Sleep(delay);
	for (int i = 0; i <= 255; i++)
	{
		setBrush(0, 8, RGB(i, i, i));
		line(100 + i*2, 50, 300 + i*2, 300);
	}

	Sleep(delay);
	drawText("Text example bgcolored", 300, 150, RGB(20, 25, 228), RGB(200, 255, 255));

	Sleep(delay);
	drawText("Text example transparent", 500, 150, RGB(20, 25, 228));

	Sleep(delay);
	setBrush(0, 3, RGB(0, 200, 0));
	ellipse(900, 100, 100, 50, false);

	Sleep(delay);
	circle(1100, 100, 50, false);

	Sleep(delay);
	ellipse(900, 210, 100, 50);
	
	Sleep(delay);
	circle(1100, 210, 50);
	
	Sleep(delay);
	setBrush(0, 3, RGB(0, 200, 200));
	rectangle(1200, 100, 100, 50);

	Sleep(delay);
	rectangle(1200, 210, 70, 70, RGB(120, 50, 120));

	//-------------------------------
	closeGraph();


	return 0;
}