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
void FreeSprite(Sprite &sprite);