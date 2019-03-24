#pragma once

#include <Windows.h>
#include <string>

struct Sprite
{
	int W, H;
	_CHAR_INFO C[6400];
};

_CHAR_INFO SIndx(Sprite &sprite, int x, int y);
void LoadSprite(Sprite &sprite, std::string path);
void DrawSprite(Sprite &sprite, int x, int y);