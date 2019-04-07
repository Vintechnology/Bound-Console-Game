#include <iostream>
#include <windows.h>
#include <time.h>
#include "SpriteRenderer/SpriteRenderer.h"
#include "ControlBall.h"
#include "ScreenBuffer\ScreenBuffer.h"
namespace WallObstacle
{
	
	struct WALL
	{
		int SpaceX;
		int SpaceY;
		int passed;
		bool ismove;
		bool toLeft;
	};


	//Set up Obstacle Array
	void SetupObstacle(int maxX_input, int maxY_input, int space_width_input, int wall_height_input);
	//Set level for Updated mode 
	float SetLevel(int n);
	//Logic
	void UpdateObstacle(float elapsedTime, float level, int ball_y);
	void ObstacleLogic(float fElapsedTime);
	//Draw 
	void DrawObstacle(int ORIGINX, int ORIGINY, int maxX, int maxY, Sprite &leftsprite, Sprite &rightsprite);
	//get wall index (return Obstacle[i])
	WALL getObstacle(int i);
	int getNumberOfWall();
	//Free memories
	void deposedObstacle();

}