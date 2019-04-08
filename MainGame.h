#pragma once

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
#include "ControlBall.h"

// === CONSTANTS DECLARE ===
const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 80;

const int FONT_WIDTH = 8;
const int FONT_HEIGHT = 8;

const int  GAME_WIDTH = 48;
const int GAME_HEIGHT = 78;

const int SPACE_WIDTH = 10;
const int WALL_HEIGHT = 7;

const int NUMBER_OF_WALLS = 4;

const int BALL_LIMIT = 50;

const int KEY_P = 0x50;
const int KEY_O = 0x4F;
const int KEY_C = 0x43;
const int KEY_H = 0x48;
const int KEY_E = 0x45;


// === STRUCT DECLARE ===

struct Wall {
	float spaceX;
	float spaceY;
	bool passed = 0;
};

// === GLOBAL VARIABLES DECLARE===
bool gameOver;
int score = 0;
int bonus = 0;
int bestScore = 0;
int MaxScore = 9999999;
int SectionHeigth;
Wall Obstacle[NUMBER_OF_WALLS];
int Obstacleupdate;
int CaseUpdate;
int SLevel = 100, MLevel = 100;

//Mix_Chunk *intro = Mix_LoadWAV("Bound-Console-Game/Music/Intro");

Sprite
Skin1_Ball, Skin1_LeftObs, Skin1_RightObs, //Flappy Bird Skin
Skin2_Ball, Skin2_LeftObs, Skin2_RightObs, //Super Mario Skin
Skin3_Ball, Skin3_LeftObs, Skin3_RightObs, //Death Skin
*SBall, *SLeftObs, *SRightObs; //Selected Skin

std::string SkinName;

int DefaultSkin;

void LoadMenuData();
void LoadSkin();

Sprite Logo_Outline,
		Menu_Play,
		Menu_Options,
		Menu_Credits,
		Menu_Help,
		Menu_Exit;

Sprite Game_over;
Sprite scoreNumbers[10];
Sprite Label_Credits;
Sprite Label_Help;
Sprite Label_Options;


// === FUNCTION DECLARE ===
void Intro();
int Menu();
void Init();
void Depose();
void ResetGame();
void onGameUpdate(float elapsedTime);

void GameHandleInput();
void GameLogic(float elapsedTime);
void onGameDraw();

void UpdateAndShowScore();
int ReadBestScore();
void SaveBestScore();
void GameOver();


void LoadMenuData();
void LoadGameplayData();

void FreeGameplayAsset();
void FreeMenuAsset();

std::string getStrFromInt(int number);
void DrawMenu();
void Options();
void Credits();
void Help();


void ObstacleLogic(float fElapsedTime);
void BallLogic(float elapsedTime);
void Collision();
void DrawLogic();
void DrawScore(int temp, int x, int y);

void drawHUD();
void drawStage(int originX, int originY, int maxX, int maxY);

