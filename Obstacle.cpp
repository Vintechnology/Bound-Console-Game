#include <iostream>
#include <windows.h>
#include <time.h>
#include "Obstacle.h"
#include "ScreenBuffer\ScreenBuffer.h"
namespace WallObstacle
{

	const int NUMBER_OF_WALLS = 4;
	int wallsDistance;
	int obstacleupdate;
	int WALL_HEIGHT;
	int SPACE_WIDTH;
	int PADDING;
	int GAME_WIDTH;
	int GAME_HEIGHT;
	Sprite* wallSkin;
	Wall** Obstacles;
	float wallSpeed;
	int lastWallOnScreenIndex;


	void SetupObstacle(int gameWidth, int gameHeight,int space_width, int wallHeight,int padding, Sprite* skin)
	{
		Obstacles = new Wall* [NUMBER_OF_WALLS];
		obstacleupdate = NUMBER_OF_WALLS;
		GAME_WIDTH = gameWidth;
		GAME_HEIGHT = gameHeight;
		SPACE_WIDTH = space_width;
		WALL_HEIGHT = wallHeight;
		PADDING = padding;

		wallSkin = skin;

		wallsDistance = ((float)(gameHeight) / (NUMBER_OF_WALLS - 1));
		srand(time(NULL));

		for (int i = 0; i < NUMBER_OF_WALLS; i++)
		{
			Obstacles[i]->SpaceX = padding + rand() % (gameWidth - space_width - padding);
			Obstacles[i]->SpaceY = (i + NUMBER_OF_WALLS - 1)*wallsDistance;
			Obstacles[i]->isMove = false;
			Obstacles[i]->toLeft = false;
		}
		lastWallOnScreenIndex = NUMBER_OF_WALLS - 1;
	}
	
	void setLevel(int level)
	{
		switch(level)
		{
		case 1: wallSpeed = 6.0f; break;
		case 2: wallSpeed = 7.0f; break;
		case 3: wallSpeed = 8.0f; break;
		case 4: wallSpeed = 9.0f; break;
		case 5: wallSpeed = 11.0f; break;
		case 6: wallSpeed = 13.0f; break;
		case 7: wallSpeed = 15.0f; break;
		case 8: wallSpeed = 17.0f; break;
		case 9: wallSpeed = 20.0f; break;
		}
	}

	void ObstacleLogic(float fElapsedTime)
	{
		for (int i = 0; i < NUMBER_OF_WALLS - 1; i++)
		{
			if (Obstacles[i]->isMove) {
				if (Obstacles[i]->toLeft) {
					Obstacles[i]->SpaceX -= 6.0*fElapsedTime;
					if (Obstacles[i]->SpaceX < PADDING) {
						Obstacles[i]->SpaceX = PADDING;
						Obstacles[i]->toLeft = false;
					}
				}
				else {
					Obstacles[i]->SpaceY += 6.0f*fElapsedTime;
					if (Obstacles[i]->SpaceX > GAME_WIDTH-PADDING) {
						Obstacles[i]->SpaceX = GAME_WIDTH-PADDING-1;
						Obstacles[i]->toLeft = true;
					}
				}
			}


			Obstacles[i]->SpaceY -= wallSpeed*fElapsedTime; //this is to keep the Obstacle (Wall) going up
			if (Obstacles[i]->SpaceY +WALL_HEIGHT < 0) // this to make the transition of wall which touch the top border to the bottom border smother.
			{
				Obstacles[i]->SpaceX = PADDING + rand() % (GAME_WIDTH - GAME_WIDTH - PADDING);
				Obstacles[i]->SpaceY = Obstacles[lastWallOnScreenIndex]->SpaceY + wallsDistance;
				//todo add moving mode
			}

		}
		
	}
	void drawObstacle(int originX, int originY, int maxX, int maxY)
	{
		for (int i = 0; i < NUMBER_OF_WALLS; i++) {
			int drawSpaceX = Obstacles[i]->SpaceX + 0.5f;
			int drawSpaceY = Obstacles[i]->SpaceY + 0.5f;
			ScreenBuffer::fillRect(originX, originY + drawSpaceY, originX + drawSpaceX - 1, originY + drawSpaceY + WALL_HEIGHT - 1, ' ', Color::BG_DARK_GREY);
			ScreenBuffer::fillRect(originX + drawSpaceX + SPACE_WIDTH, originY + drawSpaceY, maxX, originY + drawSpaceY + WALL_HEIGHT - 1, ' ', Color::BG_DARK_GREY);
		}
	}

	int getNumberOfWall() {
		return NUMBER_OF_WALLS;
	}

	Wall* getWall(int index) {
		return Obstacles[index];
	}
	
	void deposedObstacle()
	{
		delete[] Obstacles;
	}
}


	
