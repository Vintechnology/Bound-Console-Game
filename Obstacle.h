#include <iostream>
#include <windows.h>
#include "ControlBall.h"
#include "ScreenBuffer\ScreenBuffer.h"
namespace WallObstacle
{
	const int *nUMBER_OF_WALLS;
	int sectionHeigth;
	int obstacleupdate;
	int wALL_HEIGHT;
	int sPACE_WIDTH;
	int gAME_WIDTH;
	int gAME_HEIGHT;
	int ORIGINX;
	int ORIGINY;
	int CaseUpdate;
	struct WALL
	{
		int SpaceX;
		int SpaceY;
		int passed;
	};
	WALL *OBSTACLE;

	//Set up Obstacle Array
	void SetupObstacle(const int &number_of_walls_input, int originX, int originY, int maxX, int maxY, int space_width);
	//Set level for Updated mode 
	float SetLevel(int n);
	//Logic
	void UpdateObstacle(float elapsedTime, int ball_y, int originX, int originY, int maxX, int maxY, int space_width, int level);
	void ObstacleLogic(float fElapsedTime, int originX, int originY, int maxX, int maxY, int space_width);
	//Draw 
	void drawObstacle(int originX, int originY, int maxX, int maxY, int space_width, int wall_height);
	//get wall index (return Obstacle[i])
	WALL getwallIndex(int Obstacle_y);
	//Free memories
	void deposedObstacle();

}
