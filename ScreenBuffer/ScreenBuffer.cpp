#include <iostream>
#include <windows.h>
#include "ScreenBuffer.h"

namespace ScreenBuffer {
	int WIDTH;
	int HEIGHT;

	CHAR_INFO *_bufferScreen = nullptr;
	HANDLE _hConsoleOut;
	SMALL_RECT _windowRect;


	// ---- SETUP FUNCTIONS ----
	void Error(std::string msg) {
		system("cls");
		std::cout << msg << std::endl;
		std::cin.get();
	}

	void setCursorVisible(bool isVisible) {

		CONSOLE_CURSOR_INFO cursorInfo;

		GetConsoleCursorInfo(_hConsoleOut, &cursorInfo);
		cursorInfo.bVisible = isVisible;
		SetConsoleCursorInfo(_hConsoleOut, &cursorInfo);
	}


	void constructConsole(int width, int height, int fontWidth, int fontHeight) {
		if (_hConsoleOut == INVALID_HANDLE_VALUE)
			Error("Bad Handle");

		// Disable Scrollbar by first set the window size to minimum in order to shrink the screen buffer
		// below the actual visual size
		_windowRect = { 0, 0, 1, 1 };
		SetConsoleWindowInfo(_hConsoleOut, TRUE, &_windowRect);

		// Activate Console Screen Buffer and assign the screen buffer to the console
		COORD coord = { (short)width,(short)height };
		SetConsoleScreenBufferSize(_hConsoleOut, coord);

		SetConsoleActiveScreenBuffer(_hConsoleOut);

		// Set console font size in Consolas font
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = fontWidth;
		cfi.dwFontSize.Y = fontHeight;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;

		wcscpy_s(cfi.FaceName, L"Consolas");
		if(!SetCurrentConsoleFontEx(_hConsoleOut, false, &cfi))
			Error("Font size is invalid");

		// Get largest window size to compare
		COORD largestSize = GetLargestConsoleWindowSize(_hConsoleOut);
		if (width > largestSize.X)
			Error("Screen Width too big");
		if (height > largestSize.Y)
			Error("Screen Height too big");

		// Set the console buffer size
		COORD bufferSize = { (short)width,(short)height };
		SetConsoleScreenBufferSize(_hConsoleOut, bufferSize);

		// Set the window size
		_windowRect = { 0,0,(short)width - 1,(short)height - 1 };
		SetConsoleWindowInfo(_hConsoleOut, TRUE, &_windowRect);

		// Disable mouse input by capture it
		HANDLE _hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);// go global?
		if (!SetConsoleMode(_hConsoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT))
			Error("Cannot enable capture mouse input");

		//disable resize
		HWND consoleWindow = GetConsoleWindow();
		SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

		// Init buffer array
		_bufferScreen = new CHAR_INFO[width*height];
		memset(_bufferScreen, 0, sizeof(CHAR_INFO)*width*height);
			
	}

	void SetupBufferScreen(int width, int  height,  int fontWidth,  int fontHeight)
	{
		_hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
		WIDTH = width;
		HEIGHT = height;

		constructConsole(width, height, fontWidth, fontHeight);
		setCursorVisible(false);
	}

	void drawToConsole()
	{
		WriteConsoleOutput(_hConsoleOut, _bufferScreen, { (short)WIDTH,(short)HEIGHT }, { 0,0 }, &_windowRect);
	}

	void deposeBuffer()
	{
		delete[] _bufferScreen;
	}

	// ----DRAWING FUNCTIONS----

	void clip(int &x, int &y) {
		if (x > WIDTH - 1)
			x = WIDTH - 1;
		else if (x < 0)
			x = 0;

		if (y > HEIGHT - 1)
			y = HEIGHT - 1;
		else if (y < 0)
			y = 0;
	}

	// return -1: INVALID
	int getBufferIndex(int x, int y) {
		if (x<0 || y<0 || x>WIDTH - 1 || y>HEIGHT - 1)
			return -1;
		return y * WIDTH + x;
	}

	void draw(int x, int y, char c, short color)
	{
		int index = getBufferIndex(x, y);
		if (index == -1)
			return;
		_bufferScreen[index].Char.AsciiChar = c;
		_bufferScreen[index].Attributes = color;
	}
	void drawString(int x, int y, std::string str, short color)
	{
		int index = getBufferIndex(x, y);
		if (index == -1)
			return;
		for (int i = 0; i < (int)str.length(); i++) {
			if (x + i > WIDTH - 1)
				break;
			_bufferScreen[index + i].Char.AsciiChar = str[i];
			_bufferScreen[index + i].Attributes = color;
		}
	}

	void fillBuffer(char c, short color) {
		for (int i = 0; i < WIDTH*HEIGHT; i++) {
			_bufferScreen[i].Char.AsciiChar = c;
			_bufferScreen[i].Attributes = color;
		}
	}

	void drawLine(int x1, int y1, int x2, int y2, char c, short color) {
		clip(x1, y1);
		clip(x2, y2);
		float dx = x1 - x2;
		float dy = y1 - y2;
		
		float steps;
		if (abs(dx) > abs(dy))
			steps = abs(dx);
		else
			steps = abs(dy);

		float increaseX_perStep = dx / steps;
		float increaseY_perStep = dy / steps;

		for (int step = 0; step <= steps; step++) {
			float x = x2 + increaseX_perStep * step;
			float y = y2 + increaseY_perStep * step;
			draw(roundl(x), roundl(y), c, color);
		}
		
	}

	void drawHorizontalLine(int x1, int x2, int y, char c, short color) {
		if (y >= HEIGHT || y < 0)
			return;
		clip(x1, y);
		clip(x2, y);

		int x = (x1 < x2 ? x1 : x2);
		for (int i=0; i <= abs(x1 - x2); i++) {
			draw(x + i, y, c, color);
		}
	}

	void drawVerticalLine(int x, int y1, int y2, char c, short color) {
		if (x >= WIDTH || x < 0)
			return;
		clip(x, y1);
		clip(x, y2);

		int y = (y1 < y2 ? y1 : y2);
		for (int i = 0; i <= abs(y1 - y2); i++) {
			draw(x, y+i, c, color);
		}

	}

	void drawRect(int x1, int y1, int x2, int y2, char c, short color) {
		drawHorizontalLine(x1, x2, y1, c, color);
		drawVerticalLine(x1, y1, y2, c, color);
		drawVerticalLine(x2, y1, y2, c, color);
		drawHorizontalLine(x1, x2, y2, c, color);
	}

	void fillRect(int x1, int y1, int x2, int y2, char c, short color) {
		int dy = (y1 > y2 ? y1 - y2 : y2 - y1);

		int y = (y1 > y2 ? y2 : y1);
		for (int i = 0; i <= dy; i++) {
			drawHorizontalLine(x1, x2, y+i, c, color);
		}
	}

	// Bresenham’s Algorithm and Midpoint Circle Algorithm. Google search please!!
	void drawCircle(int cx, int cy, int r, char c, short color) {
		if (r <= 0)
			return;
		int x = 0;
		int y = r;
		int decisionParameter = 3 - 2 * r;

		while (y >= x) {// only need to draw 1/8th of the circle because the circle has 8 ways symmetry

			draw(cx + x, cy + y, c, color);
			draw(cx - x, cy + y, c, color);
			draw(cx + x, cy - y, c, color);
			draw(cx - x, cy - y, c, color);

			// swap x y to draw other half of the circle
			draw(cx + y, cy + x, c, color);
			draw(cx - y, cy + x, c, color);
			draw(cx + y, cy - x, c, color);
			draw(cx - y, cy - x, c, color);

			
			if (decisionParameter < 0) {// (x+1,y) is chosen to be the next position to draw
				decisionParameter += 4 * x + 6;
			}
			else { // (x+1,y-1) is chosen
				decisionParameter += 4 * (x - y) + 10;
				y--;
			}
			x++;
			// Note that y=r and x=0 which is a virtual position and is not a real position on screen
		}
	}

	void fillCircle(int cx, int cy, int r, char c, short color) {
		if (r <= 0)
			return;
		int x = 0;
		int y = r;
		int decisionParameter = 3 - 2 * r;

		while (y >= x) {

			drawHorizontalLine(cx - x, cx + x, cy + y, c, color);
			drawHorizontalLine(cx - y, cx + y, cy + x, c, color);
			drawHorizontalLine(cx - x, cx + x, cy - y, c, color);
			drawHorizontalLine(cx - y, cx + y, cy - x, c, color);



			if (decisionParameter < 0) {// (x+1,y) is chosen to be the next position to draw
				decisionParameter += 4 * x + 6;
			}
			else { // (x+1,y-1) is chosen
				decisionParameter += 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	}
}
