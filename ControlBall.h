#pragma once

const float g = 30.0f;

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
void controlBall(float elapsedTime, Ball &ball);