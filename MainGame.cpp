#include <iostream>
#include <chrono>
#include <SDL_mixer.h>

#include "ScreenBuffer/ScreenBuffer.h"
#include "ScreenBuffer/Color.h"
#include "AudioPlayer/AudioPlayer.h"

// === CONSTANTS DECLARE ===
const int WIDTH = 80;
const int HEIGHT = 80;

const int FONT_WIDTH = 8;
const int FONT_HEIGHT = 8;

const int  GAME_WIDTH = 48;
const int GAME_HEIGHT = 78;

const int BALL_RADIUS = 0;

const int SPACE_WIDTH = 5;
const int WALL_HEIGHT = 3;

// === ENUM DECLARE ===

// === STRUCT DECLARE ===
struct Ball {
	int x;
	int y;
	// Add something if you need
};

struct Wall {
	int spaceX;
	int spaceY;
};

// === GLOBAL VARIABLES DECLARE===
bool gameOver;

// === FUNCTION DECLARE ===
void Intro();
void Menu();
void Init();
void Depose();
void ResetGame();
void onGameUpdate(float elapsedTime);



int main() {
	Init();
	Intro();
	while (1) {
		Menu();
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
	return 0;
}

// -------- FUNCTIONS FOR STARTING STATE --------

/*
	Initialize global variables, loading game assets, records or something that will need to init once in program
*/
void Init() {
	ScreenBuffer::SetupBufferScreen(WIDTH, HEIGHT, FONT_WIDTH, FONT_HEIGHT);
}

/*
	Depose global variables, game assets or something need to clean up when the program is done
*/
void Depose() {
	ScreenBuffer::deposeBuffer();
}

// === INTRO ===
void Intro() {

}

// === MENU ===
void Menu() {

}



// -------- FUNCTIONS FOR PLAYING STATE --------

/*
	Getting ready to start the game again. Reset ball XY or score,...
*/
void ResetGame() {

}

// === HANDLE PLAY INPUT ===
void GameHandleInput() {

}

// === PLAY LOGIC ===
void GameLogic(float elapsedTime) {

}

// === PLAY DRAW ===
void GameDraw() {

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