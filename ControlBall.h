#pragma once
#include "AudioPlayer/AudioPlayer.h"
#include "SpriteRenderer/SpriteRenderer.h"
#include <string>

const int BALL_RADIUS = 2;
const float g = 30.0f;

extern const int  GAME_WIDTH;
extern const int GAME_HEIGHT;

extern int next_space;
extern int pre_space;
extern int spacePressed;
extern int leftPressed;
extern int rightPressed;

struct Ball
{
	float x;
	float y;
	float v = 0.0f;
	int passed = 0;
};

extern Ball ball;

void ControlAndUpdateBall(float elapsedTime);
void LoadBall(Sprite &ballSkin, std::string path);
void DrawBall(Sprite &ballSkin, int originX, int originY);
void FreeBall(Sprite &ballSkin);
void ResetBall();
Ball* getBall();