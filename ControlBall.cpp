#include "ControlBall.h"


namespace ControlBall {

	Ball ball;

	void Jump() {
		ball.velocityY = -g / 1.3f;
	}

	void GoLeft(float elapsedTime) {
		ball.x -= 10 * elapsedTime;
	}

	void GoRight(float elapsedTime) {
		ball.x += 10 * elapsedTime;
	}

	void Fall(float elapsedTime)
	{
		ball.y += ball.velocityY*elapsedTime;
		ball.velocityY += g * elapsedTime;
	}

	void DrawBall(Sprite &ballSkin, int originX, int originY)
	{
		DrawSprite(ballSkin, originX + ball.x - BALL_RADIUS + 0.5f, originY + ball.y - BALL_RADIUS + 0.5f);
	}

	void ResetBall(float initX, float initY)
	{
		ball.x = initX;
		ball.y = initY;
		ball.velocityY = 0;
	}


	Ball& getBall()
	{
		return ball;
	}
}