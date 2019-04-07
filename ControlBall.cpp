#include "ControlBall.h"

int next_space=0;
int pre_space=0;
int spacePressed;
int leftPressed;
int rightPressed;

Ball ball;

void ControlAndUpdateBall(float elapsedTime)
{
	if (spacePressed)
	{
		ball.passed = 0;
		ball.v = -g / 1.3f;
		AudioPlayer::PlayEffect("Bound-Console-Game/GameData/Music/Jump.wav");
	}
	ball.y += ball.v*elapsedTime;
	ball.v += g * elapsedTime;

	if (leftPressed)
		ball.x -= 10 * elapsedTime;

	if (rightPressed)
		ball.x += 10 * elapsedTime;
}

void LoadBall(Sprite &ballSkin, std::string path)
{
	LoadSprite(ballSkin, path);
}

void DrawBall(Sprite &ballSkin, int originX, int originY)
{
	DrawSprite(ballSkin, originX + ball.x - BALL_RADIUS + 0.5f, originY + ball.y - BALL_RADIUS + 0.5f);
}

void FreeBall(Sprite &ballSkin)
{
	FreeSprite(ballSkin);
}

void ResetBall()
{
	ball.x = 1 / 2.0f * GAME_WIDTH;
	ball.y = 1 / 3.0f * GAME_HEIGHT;
	ball.v = 0;
	ball.passed = 0;
}

Ball* getBall()
{
	Ball *getBall;
	getBall = &ball;
	return getBall;
}