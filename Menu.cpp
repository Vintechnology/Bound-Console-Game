#include "Illustration/Illustration.h"
#include "ScreenBuffer/ScreenBuffer.h"
#include <fstream>
#include <string>
#include <conio.h>

#define KEY_P 0x50
#define KEY_O 0x4F
#define KEY_C 0x43
#define KEY_H 0x48
#define KEY_E 0x45

void Load()
{
	Sprite Logo;
	LoadSprite(Logo, "GameData/Logo/Logo_Outline.dat");
	DrawSprite(Logo, 10, 5);
	std::ifstream List("GameData/Menu/List.dat");
	Sprite Menu;
	std::string Path;
	for (int i = 0; i < 5; i++)
	{
		std::getline(List, Path);
		LoadSprite(Menu, "GameData/Menu/" + Path);
		DrawSprite(Menu, 20, 30 + i * (Menu.H + 1));
	}
	List.close();
	ScreenBuffer::drawString(35, 25, "BEST: 69", 10);
	ScreenBuffer::drawToConsole();
}

void Menu(int &Func)
{
	Load();
	char Key;
	while (true)
	{
		Key = _getch();
		switch (Key)
		{
		case KEY_E + 32:
		case KEY_E:
			Func = 0;
			return;
		case KEY_P + 32:
		case KEY_P:
			Func = 1;
			return;
		case KEY_O + 32:
		case KEY_O:
			Func = 2;
			return;
		case KEY_C + 32:
		case KEY_C:
			Func = 3;
			return;
		case KEY_H + 32:
		case KEY_H:

			break;
		}
	}
}