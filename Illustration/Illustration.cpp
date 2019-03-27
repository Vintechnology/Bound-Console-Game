#include "Illustration.h"
#include <fstream>
#include "../ScreenBuffer/ScreenBuffer.h"

CHAR_INFO SIndx(Sprite &sprite, int x, int y)
{
	return sprite.C[y*sprite.W + x];
}

void LoadSprite(Sprite &sprite, std::string path)
{
	std::ifstream input(path);
	input >> sprite.H >> sprite.W;
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
