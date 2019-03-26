#include <iostream>
#include <conio.h>
#include <chrono>
#include <SDL_mixer.h>
#include <SDL.h>
#include "ScreenBuffer/ScreenBuffer.h"
#include "ScreenBuffer/Color.h"
#include "AudioPlayer/AudioPlayer.h"
#include "Illustration/Illustration.h"

// === CONSTANTS DECLARE ===
const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 80;

const int FONT_WIDTH = 8;
const int FONT_HEIGHT = 8;

const int  GAME_WIDTH = 48;
const int GAME_HEIGHT = 78;

const int BALL_RADIUS = 1;
const int BALL_HEIGHT = 3;

const int SPACE_WIDTH = 5;
const int WALL_HEIGHT = 4;

const int NUMBER_OF_WALLS = 4;

const int BALL_LIMIT = 50;

const int KEY_P = 0x50;
const int KEY_O = 0x4F;
const int KEY_C = 0x43;
const int KEY_H = 0x48;
const int KEY_E = 0x45;

// === Declare for Input ===
int next_space=0;
int pre_space=0;
int spacePressed;
int leftPressed;
int rightPressed;

// === ENUM DECLARE ===

// === STRUCT DECLARE ===
struct Ball {
	float x;
	float y;
	float g=10.0f;	//Gravity
	float v=0.0f;	//Velocity
	// Add something if you need
};

struct Wall {
	float spaceX;
	float spaceY;
	bool passed = 0;
};

// === GLOBAL VARIABLES DECLARE===
bool gameOver;
int score;
int bestScore;
int SectionHeigth;
Ball ball;
Wall Obstacle[NUMBER_OF_WALLS];



// === FUNCTION DECLARE ===
void Intro();
int Menu();
void Init();
void Depose();
void ResetGame();
void onGameUpdate(float elapsedTime);



int main(int argc, char* argv[]) {
	Init();
	Intro();

	while ( Menu() ) {

		ResetGame();
		auto t1 = std::chrono::system_clock::now();
		auto t2 = t1;
		while (!gameOver) {
			t2 = std::chrono::system_clock::now();
			std::chrono::duration<float> elapsedTime = t2 - t1;
			t1 = t2;
			float fElapsedTime = elapsedTime.count();
			onGameUpdate(fElapsedTime);
			ScreenBuffer::drawToConsole();
		}
	}
	Depose();
	// TODO: Close Window
	return 0;
}

// -------- FUNCTIONS FOR STARTING STATE --------

/*
	Initialize global variables, loading game assets, records or something that will need to init once in program
*/
Sprite Logo_Outline;

void LoadMenuData();

void Init() {
	ScreenBuffer::SetupBufferScreen(SCREEN_WIDTH, SCREEN_HEIGHT, FONT_WIDTH, FONT_HEIGHT);
	// TODO: Init AudioPLayer
	// TODO: Load game asset
	// Load Intro and Menu
	LoadSprite(Logo_Outline, "Bound-Console-Game/GameData/Logo/Logo_Outline.dat");
	LoadMenuData();
}

/*
	Depose global variables, game assets or something need to clean up when the program is done
*/
void Depose() {
	ScreenBuffer::deposeBuffer();
	// TODO: Free AudioPLayer
	// TODO: Free game asset
}

// === INTRO ===

void Intro() {

}

// === MENU ===

Sprite
Menu_Play,
Menu_Options,
Menu_Credits,
Menu_Help,
Menu_Exit;

void LoadMenuData()
{
	LoadSprite(Menu_Play, "Bound-Console-Game/GameData/Menu/Menu_Play.dat");
	LoadSprite(Menu_Options, "Bound-Console-Game/GameData/Menu/Menu_Options.dat");
	LoadSprite(Menu_Credits, "Bound-Console-Game/GameData/Menu/Menu_Credits.dat");
	LoadSprite(Menu_Help, "Bound-Console-Game/GameData/Menu/Menu_Help.dat");
	LoadSprite(Menu_Exit, "Bound-Console-Game/GameData/Menu/Menu_Exit.dat");
}

void DrawMenu()
{
	DrawSprite(Logo_Outline, 10, 5);
	DrawSprite(Menu_Play, 20, 30);
	DrawSprite(Menu_Options, 20, 39);
	DrawSprite(Menu_Credits, 20, 48);
	DrawSprite(Menu_Help, 20, 57);
	DrawSprite(Menu_Exit, 20, 66);
	ScreenBuffer::drawString(35, 25, "BEST: ", 10);
	std::string Score = "";
	int Temp = score;
	while (Temp > 0)
	{
		Score = (char)(Temp % 10 + 48) + Score;
		Temp /= 10;
	}
	ScreenBuffer::drawString(41, 25, Score, 10);
	ScreenBuffer::drawToConsole();
}

void Options()
{

}

void Credits()
{

}

void Help()
{

}

int Menu() {
	int Key;
	while (true)
	{
		DrawMenu();
		Key = _getch();
		switch (Key)
		{
		case KEY_E + 32:
		case KEY_E:
			return 0;

		case KEY_P + 32:
		case KEY_P:
			return 1;

		case KEY_O + 32:
		case KEY_O:
			Options();
			break;

		case KEY_C + 32:
		case KEY_C:
			Credits();
			break;

		case KEY_H + 32:
		case KEY_H:
			Help();
			break;
		}
	}
}



// -------- FUNCTIONS FOR PLAYING STATE --------

/*
	Getting ready to start the game again. Reset/ Init ball XY or score,...
*/
void ResetGame() {
	// Initialize global variables
	gameOver = false;
	ball.x = 1 / 2.0f * GAME_WIDTH;
	ball.y = 1 / 3.0f * GAME_HEIGHT;

	//Section Heigth is just to know where to put the Obstacle in the first place. So if you change the Game Height, it won't appeared in weird position.
	SectionHeigth = ((float)GAME_HEIGHT / NUMBER_OF_WALLS + 1) + 5;
	
	for (int i = 0; i < 3; i++)
	{
		Obstacle[i].spaceX = rand() % (GAME_WIDTH - SPACE_WIDTH);
		Obstacle[i].spaceY = (i + 1)*SectionHeigth;
		while (Obstacle[i].spaceX < 3)
		{
			Obstacle[i].spaceX = rand() % (GAME_WIDTH - SPACE_WIDTH);
		}
	}
	Obstacle[3].spaceY = -15;
	}

// === HANDLE PLAY INPUT ===
void GameHandleInput() {
	leftPressed=0;
	rightPressed=0;

	next_space=GetAsyncKeyState(VK_SPACE)&0x8000;
	if(next_space&&(!pre_space))
		spacePressed=1;	
	if(next_space&&pre_space)
		spacePressed=0;	
	if(!next_space)
		spacePressed=0;
	pre_space=next_space;

	if(GetAsyncKeyState(VK_LEFT)&0x8000)
		leftPressed=1;

	if(GetAsyncKeyState(VK_RIGHT)&0x8000)
		rightPressed=1;
}

// === PLAY LOGIC ===
void ObstacleLogic(float fElapsedTime);
void controlBall(float elapsedTime); 
void Collision();
void DrawLogic();

void GameLogic(float elapsedTime) {
	controlBall(elapsedTime); //@ThanhViet: Logic Error
	ObstacleLogic(elapsedTime);
	//Collision(); // hide for the purpose of seeing bug
	DrawLogic();
}


void ObstacleLogic(float fElapsedTime)
{
	srand(time(NULL));
	for (int i = 0; i < 3; i++)
	{
		if (Obstacle[i].spaceY <= 1)
		{
			Obstacle[3].spaceX = Obstacle[i].spaceX;
			Obstacle[3].spaceY = Obstacle[i].spaceY;
			Obstacle[i].spaceX = rand() % (GAME_WIDTH - SPACE_WIDTH);
			Obstacle[i].spaceY = GAME_HEIGHT - 1 + Obstacle[3].spaceY;
		}
		while (Obstacle[i].spaceX < 3)
		{
			Obstacle[i].spaceX = rand() % (GAME_WIDTH - SPACE_WIDTH);
		}
		Obstacle[i].spaceY -= 8.0f*fElapsedTime;

	
			
			Obstacle[3].spaceY -= 6.0f*fElapsedTime;
		


	}

}

void controlBall(float elapsedTime)
{
	if(spacePressed)
		ball.v=-ball.g/2.0f;
	ball.y+=ball.v*elapsedTime;
	ball.v+=ball.g*elapsedTime;

	if(leftPressed)
		ball.x-=10*elapsedTime;

	if(rightPressed)
		ball.x+=10*elapsedTime;
}

void DrawLogic()
{
	if (ball.y > BALL_LIMIT*1.0f)
	{
		for (int i = 0; i < NUMBER_OF_WALLS; i++)


			Obstacle[i].spaceY -= ball.y - BALL_LIMIT*1.0f;
		ball.y = BALL_LIMIT*1.0f;




	}
}

void Collision()
{
	for (int i = 0; i < NUMBER_OF_WALLS; i++)
	{
		if (Obstacle[i].spaceY - ball.y <= BALL_HEIGHT/2 && Obstacle[i].spaceY - ball.y >= -WALL_HEIGHT - BALL_HEIGHT/2 + 1)
		{
			if (!(ball.x - Obstacle[i].spaceX >= BALL_HEIGHT/2 && ball.x - Obstacle[i].spaceX <= SPACE_WIDTH - BALL_HEIGHT/2 - 1))
				gameOver = 1;
		}
	}
	if (ball.y < 1) gameOver = 1;
	for (int i = 0; i < NUMBER_OF_WALLS; i++)
	{
		if (ball.y == Obstacle[i].spaceY + 4 && Obstacle[i].passed == 0)
		{
			score++;
			Obstacle[i].passed = 1;
		}
	}
}

// === PLAY DRAW ===
void drawHUD() {

	ScreenBuffer::drawLine(0, 0, SCREEN_WIDTH - 1, 0, ' ', Color::BG_WHITE);
	ScreenBuffer::drawLine(0, 0, 0, SCREEN_HEIGHT - 1, ' ', Color::BG_WHITE);
	ScreenBuffer::drawLine(0, SCREEN_HEIGHT - 1, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, ' ', Color::BG_WHITE);
	ScreenBuffer::drawLine(SCREEN_WIDTH - 1, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, ' ', Color::BG_WHITE);

	ScreenBuffer::drawLine(GAME_WIDTH + 1, 0, GAME_WIDTH + 1, SCREEN_HEIGHT - 1, ' ', Color::BG_WHITE);
}

void drawStage(int originX, int originY, int maxX, int maxY) {

	ScreenBuffer::fillRect(originX + ball.x-BALL_RADIUS + 0.5f, originY + ball.y-BALL_RADIUS + 0.5f, originX + ball.x + BALL_RADIUS + 0.5f, originY + ball.y + BALL_RADIUS + 0.5f,' ', Color::BG_RED);
	for (int i = 0; i < NUMBER_OF_WALLS; i++) {
		int drawSpaceX = Obstacle[i].spaceX + 0.5f;
		int drawSpaceY = Obstacle[i].spaceY + 0.5f;
		ScreenBuffer::fillRect(originX, originY + drawSpaceY, originX + drawSpaceX - 1, originY + drawSpaceY + WALL_HEIGHT - 1, ' ', Color::BG_DARK_GREY);
		ScreenBuffer::fillRect(originX + drawSpaceX + SPACE_WIDTH, originY + drawSpaceY, maxX, originY + drawSpaceY + WALL_HEIGHT - 1, ' ', Color::BG_DARK_GREY);
	}
}

void GameDraw() {
	// TODO: Add a padding variables. Too many mysterious numbers

	//Clear the whole screen
	ScreenBuffer::fillBuffer(' ');

	drawStage(1, 1, GAME_WIDTH, GAME_HEIGHT);

	drawHUD();
}

/*
	The update loop of our game
*/
void onGameUpdate(float elapsedTime) {
	GameHandleInput();
	GameLogic(elapsedTime);
	GameDraw();
}


// -------- end --------