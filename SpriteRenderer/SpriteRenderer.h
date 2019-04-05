#pragma once

#include <Windows.h>
#include <string>

struct Sprite
{
	int W, H;
	CHAR_INFO *C;
};

void LoadSprite(Sprite &sprite, std::string path);
void DrawSprite(Sprite &sprite, int x, int y);
void DrawCrop(Sprite &sprite, int x, int y, int x1, int y1, int x2, int y2);
void FreeSprite(Sprite &sprite);