#include "ControlBall.h"
#include "AudioPlayer/AudioPlayer.h"

int next_space=0;
int pre_space=0;
int spacePressed;
int leftPressed;
int rightPressed;

void controlBall(float elapsedTime, Ball &ball)
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