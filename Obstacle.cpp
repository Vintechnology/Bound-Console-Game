#include <iostream>
#include <windows.h>
#include <time.h>
#include "Obstacle.h"
#include "ScreenBuffer\ScreenBuffer.h"
namespace WallObstacle
{
	
	WALL* initObstacle()
	{
		int number_of_walls = *nUMBER_OF_WALLS;
		WALL* obstacle = new WALL[number_of_walls];
		return obstacle;
	}


	void SetupObstacle(const int &number_of_walls_input,int originX,int originY,int maxX,int maxY,int space_width)
	{
		CaseUpdate = 1;
		nUMBER_OF_WALLS = &number_of_walls_input;
		OBSTACLE = initObstacle();
		obstacleupdate = *nUMBER_OF_WALLS;
		sectionHeigth = ((float)(maxY - originY) / (*nUMBER_OF_WALLS + 1));
		srand(time(NULL));
		for (int i = 0; i < *nUMBER_OF_WALLS - 1; i++)
		{
			OBSTACLE[i].passed = 0;
			OBSTACLE[i].SpaceX = rand() % ((maxX - originX) - space_width);
			OBSTACLE[i].SpaceY = (i + *nUMBER_OF_WALLS - 1)*sectionHeigth;
			while (OBSTACLE[i].SpaceX < 3)
			{
				OBSTACLE[i].SpaceX = rand() % ((maxX - originX) - space_width);
			}
		}
		OBSTACLE[*nUMBER_OF_WALLS - 1].SpaceY = -15;
		OBSTACLE[*nUMBER_OF_WALLS - 1].passed = 1;
	}
	
	float SetLevel(int n)
	{
		switch(n)
		{
		case 1: return 6.0f; break;
		case 2: return 7.0f; break;
		case 3: return 8.0f; break;
		case 4: return 9.0f; break;
		case 5: return 11.0f; break;
		case 6: return 13.0f; break;
		case 7: return 15.0f; break;
		case 8: return 17.0f; break;
		case 9: return 20.0f; break;
		}
	}
	void UpdateObstacle(float elapsedTime, int ball_y, int originX, int originY, int maxX, int maxY, int space_width,int level)
	{
		if (obstacleupdate == *nUMBER_OF_WALLS || OBSTACLE[obstacleupdate].SpaceY < ball_y) //this is to make the Obstacle choosed won't be replace if we have go over it.
		{
			for (int i = 0; i < *nUMBER_OF_WALLS; i++) //this is to take the name of the Obstacle that is lower than our Ball
			{
				if (OBSTACLE[i].SpaceY > ball_y)
				{
					obstacleupdate = i;
					break;
				}
			}
		}
		if (OBSTACLE[obstacleupdate].SpaceX + space_width >= (maxX - originX)) CaseUpdate = 2;
		if (OBSTACLE[obstacleupdate].SpaceX <= 0) CaseUpdate = 1;

		switch (CaseUpdate)
		{
		case 1: OBSTACLE[obstacleupdate].SpaceX += elapsedTime*level; break;
		case 2: OBSTACLE[obstacleupdate].SpaceX -= elapsedTime*level; break;
		}

	}
	void ObstacleLogic(float fElapsedTime,int originX,int originY,int maxX,int maxY,int space_width)
	{
		for (int i = 0; i < *nUMBER_OF_WALLS - 1; i++)
		{
			OBSTACLE[i].SpaceY -= 8.0f*fElapsedTime; //this is to keep the Obstacle (Wall) going up


			if (OBSTACLE[i].SpaceY <= 0) // this to make the transition of wall which touch the top border to the bottom border smother.
			{
				OBSTACLE[i].passed = 0; //this is to know that this wall is not passed.(after recycled)
				OBSTACLE[*nUMBER_OF_WALLS - 1].SpaceX = OBSTACLE[i].SpaceX;
				OBSTACLE[*nUMBER_OF_WALLS - 1].SpaceY = OBSTACLE[i].SpaceY;
				OBSTACLE[i].SpaceX = 1 + rand() % ((maxX - originX) - space_width - 1);
				OBSTACLE[i].SpaceY = (maxY-originY) - 1 + OBSTACLE[*nUMBER_OF_WALLS - 1].SpaceY;
			}
			OBSTACLE[*nUMBER_OF_WALLS - 1].SpaceY -= 6.0f*fElapsedTime;

		}
		
	}
	void drawObstacle(int originX, int originY, int maxX, int maxY, int space_width,int wall_height)
	{
		for (int i = 0; i < *nUMBER_OF_WALLS; i++) {
			int drawSpaceX = OBSTACLE[i].SpaceX + 0.5f;
			int drawSpaceY = OBSTACLE[i].SpaceY + 0.5f;
			ScreenBuffer::fillRect(originX, originY + drawSpaceY, originX + drawSpaceX - 1, originY + drawSpaceY + wall_height - 1, ' ', Color::BG_DARK_GREY);
			ScreenBuffer::fillRect(originX + drawSpaceX + sPACE_WIDTH, originY + drawSpaceY, maxX, originY + drawSpaceY + wALL_HEIGHT - 1, ' ', Color::BG_DARK_GREY);
		}
	}
	WALL getwallIndex(int Obstacle_y)
	{
		for(int i = 0 ; i < *nUMBER_OF_WALLS; i++)
		{
			if (OBSTACLE[i].SpaceY == Obstacle_y) return OBSTACLE[i];
		}
	}
	
	void deposedObstacle()
	{
		delete[] OBSTACLE;
	}
}


	
