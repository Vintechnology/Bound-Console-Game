#pragma once
#include <string>
#include "Color.h"

namespace ScreenBuffer {
	// ---SETUP---
	void SetupBufferScreen( int width,  int height,  int fontWidth=8, int fontHeight=8);
	void drawToConsole();
	void deposeBuffer();

	// ---DRAW---
	void draw(int x, int y, char c, short color=0x000F);
	void drawString(int x, int y, std::string str, short color=0x000F);
	void drawLine(int x1, int y1, int x2, int y2, char c, short color = 0x000F); 

	void drawRect(int x1, int y1, int x2, int y2, char c, short color = 0x000F);
	void fillRect(int x1, int y1, int x2, int y2, char c, short color = 0x000F);

	void drawCircle(int cx, int cy, int r, char c, short color = 0x000F);
	void fillCircle(int cx, int cy, int r, char c, short color = 0x000F);
}