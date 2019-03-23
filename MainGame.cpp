#include <iostream>
#include <chrono>
#include <SDL_mixer.h>

#include "ScreenBuffer/ScreenBuffer.h"
#include "ScreenBuffer/Color.h"
#include "AudioPlayer/AudioPlayer.h"

// === CONSTANTS DECLARE ===
const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 80;

const int FONT_WIDTH = 8;
const int FONT_HEIGHT = 8;

const int  GAME_WIDTH = 48;
const int GAME_HEIGHT = 78;

const int BALL_RADIUS = 0;

const int SPACE_WIDTH = 5;
const int WALL_HEIGHT = 3;

const int NUMBER_OF_WALLS = 1;// Replace this

// === ENUM DECLARE ===

// === STRUCT DECLARE ===
struct Ball {
	float x;
	float y;
	// Add something if you need
};

struct Wall {
	float spaceX;
	float spaceY;
};

// === GLOBAL VARIABLES DECLARE===
bool gameOver;
int score;
int bestScore;

Ball ball;
Wall walls[NUMBER_OF_WALLS];


// === FUNCTION DECLARE ===
void Intro();
void Menu();
void Init();
void Depose();
void ResetGame();
void onGameUpdate(float elapsedTime);



int main(int argc, char* argv[]) {
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
	ScreenBuffer::SetupBufferScreen(SCREEN_WIDTH, SCREEN_HEIGHT, FONT_WIDTH, FONT_HEIGHT);
	// TODO: Init AudioPLayer
	// TODO: Load game asset
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
void Menu() {

}



// -------- FUNCTIONS FOR PLAYING STATE --------

/*
	Getting ready to start the game again. Reset/ Init ball XY or score,...
*/
void ResetGame() {
	// Initialize global variables
	gameOver = false;
	ball.x = 1 / 2 * GAME_WIDTH;
	ball.y = 1 / 3 * GAME_HEIGHT;
	// @ThanhUy TODO: Init Walls
}

// === HANDLE PLAY INPUT ===
void GameHandleInput() {

}

// === PLAY LOGIC ===
void GameLogic(float elapsedTime) {
	// TODO: Update ball
	// TODO: Update Walls
	// TODO: Check for collision, increase score or game over
	// TODO: Update Game Camera
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