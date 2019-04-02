#include <iostream>
#include <conio.h>
#include <chrono>
#include <fstream>
#include <SDL_mixer.h>
#include <SDL.h>
#include "ScreenBuffer/ScreenBuffer.h"
#include "ScreenBuffer/Color.h"
#include "AudioPlayer/AudioPlayer.h"
#include "SpriteRenderer/SpriteRenderer.h"

// === CONSTANTS DECLARE ===
const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 80;

const int FONT_WIDTH = 8;
const int FONT_HEIGHT = 8;

const int  GAME_WIDTH = 48;
const int GAME_HEIGHT = 78;

const int BALL_RADIUS = 1;

const int SPACE_WIDTH = 5;
const int WALL_HEIGHT = 4;

const int NUMBER_OF_WALLS = 4;

const int BALL_LIMIT = 50;

const int KEY_P = 0x50;
const int KEY_O = 0x4F;
const int KEY_C = 0x43;
const int KEY_H = 0x48;
const int KEY_E = 0x45;

const float g=30.0f;	//Gravity

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
int score= 0;
int bestScore=0;
int MaxScore=9999999;
int SectionHeigth;
Ball ball;
Wall Obstacle[NUMBER_OF_WALLS];
//Mix_Chunk *intro = Mix_LoadWAV("Bound-Console-Game/Music/Intro");


// === FUNCTION DECLARE ===
void Intro();
int Menu();
void Init();
void Depose();
void ResetGame();
void onGameUpdate(float elapsedTime);
void UpdateAndShowScore();
void GameOver();

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
		AudioPlayer::PlayEffect("Bound-Console-Game/GameData/Music/Die.wav");
		GameOver();
	}
	Depose();
	// TODO: Close Window
	return 0;
}

// -------- FUNCTIONS FOR STARTING STATE --------

/*
	Initialize global variables, loading game assets, records or something that will need to init once in program
*/
Sprite 
Logo_Outline,
Menu_Play,
Menu_Options,
Menu_Credits,
Menu_Help,
Menu_Exit;

void LoadMenuData();

void Init() {
	ScreenBuffer::SetupBufferScreen(SCREEN_WIDTH, SCREEN_HEIGHT, FONT_WIDTH, FONT_HEIGHT);
	// TODO: Init AudioPLayer
	// TODO: Load game asset
	// Load Intro and Menu
	AudioPlayer::initPlayer();
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
	// Free Menu assets
	FreeSprite(Logo_Outline);
	FreeSprite(Menu_Play);
	FreeSprite(Menu_Options);
	FreeSprite(Menu_Credits);
	FreeSprite(Menu_Help);
	FreeSprite(Menu_Exit);
}

// === INTRO ===

void Intro() {
	Sprite Black;
	LoadSprite(Black, "Bound-Console-Game/GameData/Logo/Black.dat");
	Sprite Logo_Inner;
	LoadSprite(Logo_Inner, "Bound-Console-Game/GameData/Logo/Logo_Inner.dat");
	Sleep(500);
	DrawSprite(Logo_Inner, 11, 35);
	ScreenBuffer::drawToConsole();
	AudioPlayer::PlayEffect("Bound-Console-Game/GameData/Music/Intro.wav");
	Sleep(500);
	for (int i = 0; i < 6; i++)
	{
		Sleep(50);
		DrawSprite(Logo_Outline, 10, 34);
		for (int j = 0; j < 5; j++)
			if (j != i)
				DrawSprite(Black, 12 * j + 10, 34);
		DrawSprite(Logo_Inner, 11, 35);
		ScreenBuffer::drawToConsole();
	}
	for (int i = 0; i < 5; i++)
	{
		Sleep(50);
		DrawSprite(Logo_Outline, 10, 34);
		for (int j = i + 1; j < 5; j++)
			if (j != i)
				DrawSprite(Black, 12 * j + 10, 34);
		DrawSprite(Logo_Inner, 11, 35);
		ScreenBuffer::drawToConsole();
	}
	Sleep(1000);
	FreeSprite(Logo_Inner);
	FreeSprite(Black);
}

// === MENU ===



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
	ScreenBuffer::fillBuffer(32, 0);
	ScreenBuffer::drawToConsole();
	DrawSprite(Logo_Outline, 10, 5);
	DrawSprite(Menu_Play, 20, 30);
	DrawSprite(Menu_Options, 20, 39);
	DrawSprite(Menu_Credits, 20, 48);
	DrawSprite(Menu_Help, 20, 57);
	DrawSprite(Menu_Exit, 20, 66);
	ScreenBuffer::drawString(35, 25, "BEST: ", 10);

	//Get Best Score from file txt to show in Menu
	std::ifstream infile("bestScore.txt");
	if (!infile.is_open())
	{
		std::ofstream out("bestScore.txt");
		out << 0 << std::endl;
		out.close();
		std::ifstream infile("bestScore.txt");
		infile >> bestScore;
	}
	if (infile.is_open())
		infile >> bestScore;
	infile.close();

	std::string Score = "";
	int Temp = bestScore;
	if(bestScore==0)
		Score='0';
	else
	{
		while (Temp > 0)
		{
			Score = (char)(Temp % 10 + 48) + Score;
			Temp /= 10;
		}
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
	AudioPlayer::PlayBackgroundMusic("Bound-Console-Game/GameData/Music/Menu.wav");
	int Key;
	while (true)
	{
		while (_kbhit()) _getch(); //clear the input buffer
		DrawMenu();
		Key = _getch();
		switch (Key)
		{
		case KEY_E + 32:
		case KEY_E:
			AudioPlayer::PauseMusic();
			return 0;

		case KEY_P + 32:
		case KEY_P:
			AudioPlayer::PauseMusic();
			return 1;

		case KEY_O + 32:
		case KEY_O:
			AudioPlayer::PauseMusic();
			Options();
			break;

		case KEY_C + 32:
		case KEY_C:
			AudioPlayer::PauseMusic();
			Credits();
			break;

		case KEY_H + 32:
		case KEY_H:
			AudioPlayer::PauseMusic();
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
	score = 0;
	ball.v = 0;

	//Section Heigth is just to know where to put the Obstacle in the first place. So if you change the Game Height, it won't appeared in weird position.
	SectionHeigth = ((float)GAME_HEIGHT / NUMBER_OF_WALLS + 1) + 5;
	
	for (int i = 0; i < 3; i++)
	{
		Obstacle[i].passed = 0;
		Obstacle[i].spaceX = rand() % (GAME_WIDTH - SPACE_WIDTH);
		Obstacle[i].spaceY = (i + 3)*SectionHeigth;
		while (Obstacle[i].spaceX < 3)
		{
			Obstacle[i].spaceX = rand() % (GAME_WIDTH - SPACE_WIDTH);
		}
	}
	Obstacle[3].spaceY = -15;
	Obstacle[3].passed = 1;
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
int BestScore();
void DrawScore(int temp,int x,int y);

void GameLogic(float elapsedTime) {
	controlBall(elapsedTime);
	ObstacleLogic(elapsedTime);
	Collision(); // hide for the purpose of seeing bug
	DrawLogic();
}


void ObstacleLogic(float fElapsedTime)
{
	srand(time(NULL));
	for (int i = 0; i < 3; i++)
	{
		Obstacle[i].spaceY -= 8.0f*fElapsedTime; //this is to keep the Obstacle (Wall) going up
		
		
		if (Obstacle[i].spaceY <= 0) // this to make the transition of wall which touch the top border to the bottom border smother.
		{
			Obstacle[i].passed = 0; //this is to know that this wall is not passed.(after recycled)
			Obstacle[3].spaceX = Obstacle[i].spaceX;
			Obstacle[3].spaceY = Obstacle[i].spaceY;
			Obstacle[i].spaceX = rand() % (GAME_WIDTH - SPACE_WIDTH);
			Obstacle[i].spaceY = GAME_HEIGHT - 1 + Obstacle[3].spaceY;
		}
		Obstacle[3].spaceY -= 6.0f*fElapsedTime;
		
		
		while (Obstacle[i].spaceX < 3) //this is to make sure the Obstall Space wont touch the Left boder.
		{
			Obstacle[i].spaceX += 2;
		}
	}
}

void controlBall(float elapsedTime)
{
	if (spacePressed)
	{
		ball.v = -g / 1.3f;
		AudioPlayer::PlayEffect("Bound-Console-Game/GameData/Music/Jump.wav");
	}
	ball.y+=ball.v*elapsedTime;
	ball.v+=g*elapsedTime;

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
		if (Obstacle[i].spaceY - ball.y < BALL_RADIUS + 1 && Obstacle[i].spaceY - ball.y > -WALL_HEIGHT - BALL_RADIUS)
		{
			if (!(ball.x - Obstacle[i].spaceX > BALL_RADIUS - 1 && ball.x - Obstacle[i].spaceX < SPACE_WIDTH - BALL_RADIUS))
				gameOver = 1;
		}
	}
	if (ball.y < 1) gameOver = 1;
	for (int i = 0; i < NUMBER_OF_WALLS; i++)
	{
		if (ball.y >= Obstacle[i].spaceY + WALL_HEIGHT && Obstacle[i].passed == 0)
		{
			AudioPlayer::PlayEffect("Bound-Console-Game/GameData/Music/Point.wav");
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
	//TODO: Draw score
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
int BestScore()
{
	std::ifstream infile("bestScore.txt");
	if (!infile.is_open())
	{
		std::ofstream out("bestScore.txt");
		out << 0 << std::endl;
		out.close();
		std::ifstream infile("bestScore.txt");
		infile >> bestScore;
	}
	if (infile.is_open())
		infile >> bestScore;
	infile.close();
	if (score > bestScore)
	{
		bestScore = score;
		std::ofstream outfile("bestScore.txt");
		if (outfile.is_open())
			outfile << bestScore;
		outfile.close();
	}
	return bestScore;
}
void DrawScore(int temp,int x,int y)
{
	Sprite n_0;
	LoadSprite(n_0, "Bound-Console-Game/GameData/Numbers/0.dat");
	Sprite n_1;
	LoadSprite(n_1, "Bound-Console-Game/GameData/Numbers/1.dat");
	Sprite n_2;
	LoadSprite(n_2, "Bound-Console-Game/GameData/Numbers/2.dat");
	Sprite n_3;
	LoadSprite(n_3, "Bound-Console-Game/GameData/Numbers/3.dat");
	Sprite n_4;
	LoadSprite(n_4, "Bound-Console-Game/GameData/Numbers/4.dat");
	Sprite n_5;
	LoadSprite(n_5, "Bound-Console-Game/GameData/Numbers/5.dat");
	Sprite n_6;
	LoadSprite(n_6, "Bound-Console-Game/GameData/Numbers/6.dat");
	Sprite n_7;
	LoadSprite(n_7, "Bound-Console-Game/GameData/Numbers/7.dat");
	Sprite n_8;
	LoadSprite(n_8, "Bound-Console-Game/GameData/Numbers/8.dat");
	Sprite n_9;
	LoadSprite(n_9, "Bound-Console-Game/GameData/Numbers/9.dat");

	switch (temp)
	{
	case 0:
		DrawSprite(n_0, x, y);
		FreeSprite(n_0);
		break;
	case 1:
		DrawSprite(n_1, x, y);
		FreeSprite(n_1);
		break;
	case 2:
		DrawSprite(n_2, x, y);
		FreeSprite(n_2);
		break;
	case 3:
		DrawSprite(n_3, x, y);
		FreeSprite(n_3);
		break;
	case 4:
		DrawSprite(n_4, x, y);
		FreeSprite(n_4);
		break;
	case 5:
		DrawSprite(n_5, x, y);
		FreeSprite(n_5);
		break;
	case 6:
		DrawSprite(n_6, x, y);
		FreeSprite(n_6);
		break;
	case 7:
		DrawSprite(n_7, x, y);
		FreeSprite(n_7);
		break;
	case 8:
		DrawSprite(n_8, x, y);
		FreeSprite(n_8);
		break;
	case 9:
		DrawSprite(n_9, x, y);
		FreeSprite(n_9);
		break;
	}
}
void UpdateAndShowScore()
{
	int temp;
	int temp2;
	int temp3;
	int temp4;
	int temp5;
	int temp6;
	int temp7;
	// 0<= score<=9
	if ((0 <= score) && ((score < 10)))
	{
		DrawScore(score, 65,5);
	}

	// 10<= score<= 99
	else if ((10 <= score) && (score < 100))
	{
		temp = score / 10;
		temp2 = score % 10;
		DrawScore(temp, 61,5);
		DrawScore(temp2, 65,5);
	}

	// 100<= score <=999
	else if ((100 <= score) && (score < 1000))
	{
		temp = score / 100;
		temp2 = (score - temp * 100) / 10;
		temp3 = score % 10;
		DrawScore(temp, 61,5);
		DrawScore(temp2, 65,5);
		DrawScore(temp3, 69,5);
	}

	// 1.000<=score<=9.999
	else if ((1000 <= score) && (score < 10000))
	{
		temp = score / 1000;
		temp2 = (score - temp * 1000) / 100;
		temp3 = (score - temp * 1000 - temp2 * 100) / 10;
		temp4 = score % 10;
		DrawScore(temp, 58,5);
		DrawScore(temp2, 62,5);
		DrawScore(temp3, 66,5);
		DrawScore(temp4, 70,5);
	}

	//10.000<= score<99.999
	else if ((10000 <= score) && (score < 100000))
	{
		temp = score / 10000;
		temp2 = (score - temp * 10000) / 1000;
		temp3 = (score - temp * 10000 - temp2 * 1000) / 100;
		temp4 = (score - temp * 10000 - temp2 * 1000 - temp3 * 100) / 10;
		temp5 = score % 10;
		DrawScore(temp, 55,5);
		DrawScore(temp2, 59,5);
		DrawScore(temp3, 63,5);
		DrawScore(temp4, 67,5);
		DrawScore(temp5, 71,5);
	}

	// 100.000<=score<=999.999
	else if ((100000 <= score) && (score < 1000000))
	{
		temp = score / 100000;
		temp2 = (score - temp * 100000) / 10000;
		temp3 = (score - temp * 100000 - temp2 * 10000) / 1000;
		temp4 = (score - temp * 100000 - temp2 * 10000 - temp3 * 1000) / 100;
		temp5 = (score - temp * 100000 - temp2 * 10000 - temp3 * 1000 - temp4 * 100) / 10;
		temp6 = score % 10;
		DrawScore(temp, 53,5);
		DrawScore(temp2, 57,5);
		DrawScore(temp3, 61,5);
		DrawScore(temp4, 65,5);
		DrawScore(temp5, 69,5);
		DrawScore(temp6, 73,5);
	}

	//1.000.000<=score<=9.999.999
	else if ((1000000<=score) && (score<10000000))
	{
		temp = score / 1000000;
		temp2 = (score - temp * 1000000) / 100000;
		temp3 = (score - temp * 1000000 - temp2 * 100000) / 10000;
		temp4 = (score - temp * 1000000 - temp2 * 100000 - temp3 * 10000) / 1000;
		temp5 = (score - temp * 1000000 - temp2 * 100000 - temp3 * 10000 - temp4 * 1000) / 100;
		temp6 = (score - temp * 1000000 - temp2 * 100000 - temp3 * 10000 - temp4 * 1000 - temp5 * 100) / 10;
		temp7 = score % 10;
		DrawScore(temp, 51,5);
		DrawScore(temp2, 55,5);
		DrawScore(temp3, 59,5);
		DrawScore(temp4, 63,5);
		DrawScore(temp5, 67,5);
		DrawScore(temp6, 71,5);
		DrawScore(temp7, 75,5);
	}
}
void onGameUpdate(float elapsedTime) {
	GameHandleInput();
	GameLogic(elapsedTime);
	BestScore();
	GameDraw();
	UpdateAndShowScore();
}
void GameOver()
{
	while (gameOver)
	{
		int Temp;
		std::string StrScore = "";
		Temp = score;
		if (score == 0)
			StrScore = '0';
		else
		{
			while (Temp > 0)
			{
				StrScore = (char)(Temp % 10 + 48) + StrScore;
				Temp /= 10;
			}
		}
		
		std::string StrBest = "";
		Temp = bestScore;
		if (bestScore == 0)
			StrBest = '0';
		else
		{
			while (Temp > 0)
			{
				StrBest = (char)(Temp % 10 + 48) + StrBest;
				Temp /= 10;
			}
		}
		
		Sprite Game_over;
		LoadSprite(Game_over, "Bound-Console-Game/GameData/GameOver/GameOver.dat");
		DrawSprite(Game_over, 3, 32);
		FreeSprite(Game_over);
		ScreenBuffer::drawString(19, 40, "SCORE", 176);
		ScreenBuffer::drawString(26, 40, StrScore, 176);
		ScreenBuffer::drawString(20, 42, "BEST", 176);
		ScreenBuffer::drawString(26, 42, StrBest, 176);
		ScreenBuffer::drawLine(4, 44, 45, 44, 223, 11);
		ScreenBuffer::drawString(4, 45, " [ESC] BACK TO MENU  [SPACE] PLAY AGAIN   ", 11);
		ScreenBuffer::drawToConsole();
		while (_kbhit()) _getch(); //clear the input buffer
		_getch(); break;
	}
}

// -------- end --------