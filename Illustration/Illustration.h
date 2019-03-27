#pragma once

#include <Windows.h>
#include <string>

struct Sprite
{
	int W, H;
	CHAR_INFO C[6400];// @TranTrung: Using dynamic allocation will be better. Remember to add another function for freeing Sprite
};

CHAR_INFO SIndx(Sprite &sprite, int x, int y);
void LoadSprite(Sprite &sprite, std::string path);
void DrawSprite(Sprite &sprite, int x, int y);