#include "Obstacle.h"
namespace WallObstacle
{
	int NUMBER_OF_WALLS = 4;
	int sectionHeigth;
	int obstacleupdate;
	int wALL_HEIGHT;
	int sPACE_WIDTH;
	int gAME_WIDTH;
	int gAME_HEIGHT;
	int MAXX;
	int MAXY;
	int CaseUpdate;
	int PADDING = 1;
	WALL* Obstacle;
	float wallSpeed;

	void SetupObstacle(int maxX_input, int maxY_input, int space_width_input, int wall_height_input)
	{
		wALL_HEIGHT = wall_height_input;
		sPACE_WIDTH = space_width_input;
		MAXX = maxX_input;
		MAXY = maxY_input;
		CaseUpdate = 1;
		Obstacle = new WALL [NUMBER_OF_WALLS];
		obstacleupdate = NUMBER_OF_WALLS;
		sectionHeigth = ((float)maxY_input / (NUMBER_OF_WALLS )) + 5;
		srand(time(NULL));
		for (int i = 0; i < NUMBER_OF_WALLS - 1; i++)
		{
			Obstacle[i].passed = 0;
			Obstacle[i].SpaceX = rand() % (maxX_input - space_width_input);
			Obstacle[i].SpaceY = (i + NUMBER_OF_WALLS - 1)*sectionHeigth;
			while (Obstacle[i].SpaceX < 3)
			{
				Obstacle[i].SpaceX = rand() % (maxX_input - space_width_input);
			}
		}
		Obstacle[NUMBER_OF_WALLS - 1].SpaceY = -15;
		Obstacle[NUMBER_OF_WALLS - 1].passed = 1;
	}

	float SetLevel(int n)
	{
		switch (n)
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
	/*void UpdateObstacle(float elapsedTime, float level, int ball_y)
	{
		if (obstacleupdate == NUMBER_OF_WALLS || Obstacle[obstacleupdate].SpaceY < ball_y) //this is to make the Obstacle choosed won't be replace if we have go over it.
		{
			for (int i = 0; i < NUMBER_OF_WALLS; i++) //this is to take the name of the Obstacle that is lower than our Ball
			{
				if (Obstacle[i].SpaceY > ball_y)
				{
					obstacleupdate = i;
					break;
				}
			}
		}
		Obstacle[obstacleupdate].ismove = true;
	}
	/*void ObstacleLogic(float fElapsedTime)
	{
		for (int i = 0; i < NUMBER_OF_WALLS - 1; i++)
		{
			if (Obstacle[i].ismove) {
				if (Obstacle[i].toLeft) {
					Obstacle[i].SpaceX -= 6.0*fElapsedTime;
					if (Obstacle[i].SpaceX < PADDING) {
						Obstacle[i].SpaceX = PADDING;
						Obstacle[i].toLeft = false;
					}
				}
				else {
					Obstacle[i].SpaceY += 6.0f*fElapsedTime;
					if (Obstacle[i].SpaceX > GAME_WIDTH - PADDING) {
						Obstacle[i].SpaceX = GAME_WIDTH - PADDING - 1;
						Obstacle[i].toLeft = true;
					}
				}
			}

			Obstacle[i].SpaceY -= 6.0f*fElapsedTime; //this is to keep the Obstacle (Wall) going up


			if (Obstacle[i].SpaceY <= 0) // this to make the transition of wall which touch the top border to the bottom border smother.
			{
				Obstacle[i].passed = 0; //this is to know that this wall is not passed.(after recycled)
				Obstacle[NUMBER_OF_WALLS - 1].SpaceX = Obstacle[i].SpaceX;
				Obstacle[NUMBER_OF_WALLS - 1].SpaceY = Obstacle[i].SpaceY;


				Obstacle[i].passed = 0;
				Obstacle[i].ismove = false;
				Obstacle[i].toLeft = false;
				Obstacle[i].SpaceX = PADDING + rand() % ((MAXX)-sPACE_WIDTH - PADDING);
				Obstacle[i].SpaceY = (MAXX)-1 + Obstacle[NUMBER_OF_WALLS - 1].SpaceY;
			}
			Obstacle[NUMBER_OF_WALLS - 1].SpaceY -= 6.0f*fElapsedTime;
		}
	}
	*/
	void UpdateObstacle(float elapsedTime, float level, int ball_y)
	{
		if (obstacleupdate == NUMBER_OF_WALLS || Obstacle[obstacleupdate].SpaceY < ball_y) //this is to make the Obstacle choosed won't be replace if we have go over it.
		{
			for (int i = 0; i < NUMBER_OF_WALLS - 1; i++) //this is to take the name of the Obstacle that is lower than our Ball
			{
				if (Obstacle[i].SpaceY > ball_y)
				{
					obstacleupdate = i;
					break;
				}
			}
		}
		if (Obstacle[obstacleupdate].SpaceX + sPACE_WIDTH >= (MAXX)) CaseUpdate = 2;
		if (Obstacle[obstacleupdate].SpaceX <= 0) CaseUpdate = 1;

		switch (CaseUpdate)
		{
		case 1: Obstacle[obstacleupdate].SpaceX += elapsedTime*level; break;
		case 2: Obstacle[obstacleupdate].SpaceX -= elapsedTime*level; break;
		}

	}
	void ObstacleLogic(float fElapsedTime)
	{
		for (int i = 0; i < NUMBER_OF_WALLS - 1; i++)
		{
			Obstacle[i].SpaceY -= 8.0f*fElapsedTime; //this is to keep the Obstacle (Wall) going up


			if (Obstacle[i].SpaceY <= 0) // this to make the transition of wall which touch the top border to the bottom border smother.
			{
				Obstacle[i].passed = 0; //this is to know that this wall is not passed.(after recycled)
				Obstacle[NUMBER_OF_WALLS - 1].SpaceX = Obstacle[i].SpaceX;
				Obstacle[NUMBER_OF_WALLS - 1].SpaceY = Obstacle[i].SpaceY;
				Obstacle[i].SpaceX = 1 + rand() % (GAME_WIDTH - sPACE_WIDTH - 1);
				Obstacle[i].SpaceY = MAXY - 1 + Obstacle[NUMBER_OF_WALLS - 1].SpaceY;
			}
			Obstacle[NUMBER_OF_WALLS - 1].SpaceY -= 6.0f*fElapsedTime;

		}
		Sleep(100);
	}
		
	void DrawObstacle(int ORIGINX, int ORIGINY, int maxX, int maxY, Sprite &leftsprite, Sprite &rightsprite)
	{
		for (int i = 0; i < NUMBER_OF_WALLS; i++) {
			int drawSpaceX = Obstacle[i].SpaceX + 0.5f;
			int drawSpaceY = Obstacle[i].SpaceY + 0.5f;
			DrawCrop(leftsprite, ORIGINX, ORIGINY + drawSpaceY, 50 - (ORIGINX + drawSpaceX - 1), 0, 49, 6);
			DrawCrop(rightsprite, ORIGINX + drawSpaceX + sPACE_WIDTH, ORIGINY + drawSpaceY, 0, 0, 49 - (ORIGINX + drawSpaceX + sPACE_WIDTH), 6);
		}
	}


	void deposedObstacle()
	{
		delete[] Obstacle;

	}
	int getNumberOfWall() {
		return NUMBER_OF_WALLS;
	}

	WALL getObstacle(int i)
	{
		return Obstacle[i];
	}
}