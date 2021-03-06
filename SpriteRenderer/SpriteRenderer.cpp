#include "SpriteRenderer.h"
#include <fstream>
#include "../ScreenBuffer/ScreenBuffer.h"

void LoadSprite(Sprite &sprite, std::string path)
{
	std::ifstream input(path);
	input >> sprite.H >> sprite.W;
	sprite.C = new CHAR_INFO[sprite.H*sprite.W];
	int Char;
	for (int i = 0; i < sprite.H*sprite.W; i++)
	{
		input >> Char;
		sprite.C[i].Char.AsciiChar = (char) Char;
	}
	for (int i = 0; i < sprite.H*sprite.W; i++)
		input >> sprite.C[i].Attributes;
	input.close();
}

void DrawSprite(Sprite & sprite, int x, int y)
{
	for (int i = 0; i < sprite.W; i++)
		for (int j = 0; j < sprite.H; j++)
			if (sprite.C[j*sprite.W + i].Char.AsciiChar != 0)
			ScreenBuffer::draw(i + x, j + y, sprite.C[j*sprite.W + i].Char.AsciiChar, sprite.C[j*sprite.W + i].Attributes);
}

void DrawCrop(Sprite &sprite, int x, int y, int x1, int y1, int x2, int y2)
{
	for (int i = x1; i <= x2; i++)
		for (int j = y1; j <= y2; j++)
			if (sprite.C[j*sprite.W + i].Char.AsciiChar != 0)
				ScreenBuffer::draw(i + x - x1, j + y - y1, sprite.C[j*sprite.W + i].Char.AsciiChar, sprite.C[j*sprite.W + i].Attributes);
}

void FreeSprite(Sprite &sprite)
{
	delete[] sprite.C;
}
