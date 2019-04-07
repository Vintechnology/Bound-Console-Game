#include <iostream>
#include <windows.h>
#include "ControlBall.h"
#include "ScreenBuffer\ScreenBuffer.h"
namespace WallObstacle
{
	struct Wall
	{
		int SpaceX;
		int SpaceY;
		bool isMove;
		bool toLeft;
	};

	//Set up Obstacle Array
	void SetupObstacle(int gameWidth, int gameHeight, int space_width, int wallHeight, int padding, Sprite* skin=nullptr);
	//Set level for Updated mode 
	void setLevel(int level);
	//Logic
	void ObstacleLogic(float fElapsedTime);
	//Draw 
	void drawObstacle(int originX, int originY, int maxX, int maxY);
	//get wall index (return Obstacle[i])
	int getNumberOfWall();
	Wall* getWall(int index);
	//Free memories
	void deposedObstacle();

}
