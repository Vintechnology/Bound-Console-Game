#pragma once
#include "AudioPlayer/AudioPlayer.h"
#include "SpriteRenderer/SpriteRenderer.h"
#include <string>

namespace ControlBall {
	struct Ball
	{
		float x;
		float y;
		float velocityY = 0.0f;
	};



	const int BALL_RADIUS = 2;
	const float g = 30.0f;

	void Jump();
	void GoLeft(float elapsedTime);
	void GoRight(float elapsedTime);
	void Fall(float elapsedTime);
	void DrawBall(Sprite &ballSkin, int originX, int originY);
	void ResetBall(float initX, float initY);
	Ball& getBall();
}