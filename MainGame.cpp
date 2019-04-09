#include "MainGame.h"

int main(int argc, char* argv[]) {
	Init();
	Intro();

	while ( Menu() ) {

		ResetGame();
		auto t1 = std::chrono::system_clock::now();
		auto t2 = t1;
		while (!gameOver) {
			t2 = std::chrono::system_clock::now();
			std::chrono::duration<float> elapsedTime = t2 - t1;
			t1 = t2;
			float fElapsedTime = elapsedTime.count();
			onGameUpdate(fElapsedTime);
			ScreenBuffer::drawToConsole();
		}
		AudioPlayer::PlayEffect("Bound-Console-Game/GameData/Music/Die.wav");
		GameOver();
	}
	Depose();
	// TODO: Close Window
	return 0;
}

void Init() {
	ScreenBuffer::SetupBufferScreen(SCREEN_WIDTH, SCREEN_HEIGHT, FONT_WIDTH, FONT_HEIGHT);
	// TODO: Load game asset
	// Load Intro and Menu
	AudioPlayer::initPlayer();
	LoadSprite(Logo_Outline, "Bound-Console-Game/GameData/Logo/Logo_Outline.dat");
	LoadGameplayData();
	LoadMenuData();
	LoadSkin();

}

void Intro() {
	Sprite Black;
	LoadSprite(Black, "Bound-Console-Game/GameData/Logo/Black.dat");
	Sprite Logo_Inner;
	LoadSprite(Logo_Inner, "Bound-Console-Game/GameData/Logo/Logo_Inner.dat");
	Sleep(500);
	DrawSprite(Logo_Inner, 11, 35);
	ScreenBuffer::drawToConsole();
	AudioPlayer::PlayEffect("Bound-Console-Game/GameData/Music/Intro.wav");
	Sleep(500);
	for (int i = 0; i < 6; i++)
	{
		Sleep(50);
		DrawSprite(Logo_Outline, 10, 34);
		for (int j = 0; j < 5; j++)
			if (j != i)
				DrawSprite(Black, 12 * j + 10, 34);
		DrawSprite(Logo_Inner, 11, 35);
		ScreenBuffer::drawToConsole();
	}
	for (int i = 0; i < 5; i++)
	{
		Sleep(50);
		DrawSprite(Logo_Outline, 10, 34);
		for (int j = i + 1; j < 5; j++)
			if (j != i)
				DrawSprite(Black, 12 * j + 10, 34);
		DrawSprite(Logo_Inner, 11, 35);
		ScreenBuffer::drawToConsole();
	}
	Sleep(1000);
	FreeSprite(Logo_Inner);
	FreeSprite(Black);
}

int Menu() {
	AudioPlayer::PlayBackgroundMusic("Bound-Console-Game/GameData/Music/Menu.wav");
	DrawMenu();
	int Key;
	while (true)
	{
		while (_kbhit()) {
			_getch();//clear the input buffer
		}
		Key = _getch();
		switch (Key)
		{
		case KEY_E + 32:
		case KEY_E:
			return 0;

		case KEY_P + 32:
		case KEY_P:
			AudioPlayer::PauseMusic();
			return 1;

		case KEY_O + 32:
		case KEY_O:
			Options();
			DrawMenu();
			break;

		case KEY_C + 32:
		case KEY_C:
			Credits();
			DrawMenu();
			break;

		case KEY_H + 32:
		case KEY_H:
			Help();
			DrawMenu();
			break;
		}
		
	}
}

void ResetGame() {
	// Initialize global variables
	gameOver = false;
	ControlBall::ResetBall(1 / 2.0f * GAME_WIDTH, 1 / 3.0f * GAME_HEIGHT);
	score = 0; bonus = 0;
	Obstacleupdate = NUMBER_OF_WALLS;
	CaseUpdate = 1;

	// Reset Obstacles

	//Section Height is just to know where to put the Obstacle in the first place. So if you change the Game Height, it won't appeared in weird position.
	SectionHeight = ((float)GAME_HEIGHT / NUMBER_OF_WALLS + 1) + 5;
	srand(time(NULL));
	for (int i = 0; i < NUMBER_OF_WALLS - 1; i++)
	{
		Obstacle[i].passed = 0;
		Obstacle[i].spaceX = rand() % (GAME_WIDTH - SPACE_WIDTH);
		Obstacle[i].spaceY = (i + NUMBER_OF_WALLS - 1)*SectionHeight;
		while (Obstacle[i].spaceX < 3)
		{
			Obstacle[i].spaceX = rand() % (GAME_WIDTH - SPACE_WIDTH);
		}
	}
	Obstacle[NUMBER_OF_WALLS - 1].spaceY = -15;
	Obstacle[NUMBER_OF_WALLS - 1].passed = 1;
}

void onGameUpdate(float elapsedTime) {
	GameHandleInput();
	GameLogic(elapsedTime);
	onGameDraw();
}

void GameOver()
{
	SaveBestScore();
	while (gameOver)
	{
		int Temp;
		std::string StrScore = "";
		Temp = score;
		if (score == 0)
			StrScore = '0';
		else
		{
			while (Temp > 0)
			{
				StrScore = (char)(Temp % 10 + 48) + StrScore;
				Temp /= 10;
			}
		}

		std::string StrBest = getStrFromInt(bestScore);

		DrawSprite(Game_over, 3, 32);
		ScreenBuffer::drawString(19, 40, "SCORE", BG_YELLOW);
		ScreenBuffer::drawString(26, 40, StrScore, BG_YELLOW);
		ScreenBuffer::drawString(20, 42, "BEST", BG_YELLOW);
		ScreenBuffer::drawString(26, 42, StrBest, BG_YELLOW);
		ScreenBuffer::drawLine(4, 44, 45, 44, 223, FG_YELLOW);
		ScreenBuffer::drawString(4, 45, "         PRESS ENTER TO CONTINUE          ", FG_YELLOW);
		ScreenBuffer::drawToConsole();
		while (_kbhit()) _getch(); //clear the input buffer
		if (GetAsyncKeyState(VK_RETURN) & 0x8000) gameOver = 0;
	}
}

void Depose() {
	ScreenBuffer::deposeBuffer();
	FreeGameplayAsset();
	FreeMenuAsset();
}

// -------- FUNCTIONS FOR STARTING STATE --------



const char* scoreNumberAssetPath[10] = 
{
"Bound-Console-Game/GameData/Numbers/0.dat",
"Bound-Console-Game/GameData/Numbers/1.dat",
"Bound-Console-Game/GameData/Numbers/2.dat",
"Bound-Console-Game/GameData/Numbers/3.dat",
"Bound-Console-Game/GameData/Numbers/4.dat",
"Bound-Console-Game/GameData/Numbers/5.dat",
"Bound-Console-Game/GameData/Numbers/6.dat",
"Bound-Console-Game/GameData/Numbers/7.dat",
"Bound-Console-Game/GameData/Numbers/8.dat",
"Bound-Console-Game/GameData/Numbers/9.dat"
};
void LoadGameplayData() {
	for(int i=0;i<10;i++)
		LoadSprite(scoreNumbers[i], scoreNumberAssetPath[i]);

	LoadSprite(Game_over, "Bound-Console-Game/GameData/GameOver/GameOver.dat");
}


void LoadMenuData()
{
	LoadSprite(Menu_Play, "Bound-Console-Game/GameData/Menu/Menu_Play.dat");
	LoadSprite(Menu_Options, "Bound-Console-Game/GameData/Menu/Menu_Options.dat");
	LoadSprite(Menu_Credits, "Bound-Console-Game/GameData/Menu/Menu_Credits.dat");
	LoadSprite(Menu_Help, "Bound-Console-Game/GameData/Menu/Menu_Help.dat");
	LoadSprite(Menu_Exit, "Bound-Console-Game/GameData/Menu/Menu_Exit.dat");


	LoadSprite(Label_Credits, "Bound-Console-Game/GameData/Credits/Credits.dat");
	LoadSprite(Label_Help, "Bound-Console-Game/GameData/Help/Help.dat");
	LoadSprite(Label_Options, "Bound-Console-Game/GameData/Options/Options.dat");
}


/*
	Depose global variables, game assets or something need to clean up when the program is done
*/


void FreeMenuAsset() {
	FreeSprite(Logo_Outline);
	FreeSprite(Menu_Play);
	FreeSprite(Menu_Options);
	FreeSprite(Menu_Credits);
	FreeSprite(Menu_Help);
	FreeSprite(Menu_Exit);

	FreeSprite(Label_Credits);
	FreeSprite(Label_Help);
	FreeSprite(Label_Options);
}

void FreeGameplayAsset() {
	for (int i = 0; i < 10; i++)
		FreeSprite(scoreNumbers[i]);

	FreeSprite(Game_over);

	FreeSprite(Skin1_Ball);
	FreeSprite(Skin1_LeftObs);
	FreeSprite(Skin1_RightObs);
	FreeSprite(Skin2_Ball);
	FreeSprite(Skin2_LeftObs);
	FreeSprite(Skin2_RightObs);
	FreeSprite(Skin3_Ball);
	FreeSprite(Skin3_LeftObs);
	FreeSprite(Skin3_RightObs);
}

// === MENU ===
std::string getStrFromInt(int number) {
	std::string Score = "";
	int Temp = bestScore;
	if (bestScore == 0)
		Score = '0';
	else
	{
		while (Temp > 0)
		{
			Score = (char)(Temp % 10 + 48) + Score;
			Temp /= 10;
		}
	}
	return Score;
}

void DrawMenu()
{
	ScreenBuffer::fillBuffer(32, 0);
	ScreenBuffer::drawToConsole();
	DrawSprite(Logo_Outline, 10, 5);
	DrawSprite(Menu_Play, 20, 30);
	DrawSprite(Menu_Options, 20, 39);
	DrawSprite(Menu_Credits, 20, 48);
	DrawSprite(Menu_Help, 20, 57);
	DrawSprite(Menu_Exit, 20, 66);
	ScreenBuffer::drawString(35, 25, "BEST: ", 10);

	//Get Best Score from file bestScore to show in Menu
	ReadBestScore();

	std::string Score = getStrFromInt(bestScore);
	ScreenBuffer::drawString(41, 25, Score, 10);
	ScreenBuffer::drawToConsole();
}

void LoadSkin()
{
	//Flappy Bird Skin
	LoadSprite(Skin1_Ball, "Bound-Console-Game/GameData/Skins/FlappyBird/Ball.dat");
	LoadSprite(Skin1_LeftObs, "Bound-Console-Game/GameData/Skins/FlappyBird/Left_Obs.dat");
	LoadSprite(Skin1_RightObs, "Bound-Console-Game/GameData/Skins/FlappyBird/Right_Obs.dat");
	//Super Mario Skin
	LoadSprite(Skin2_Ball, "Bound-Console-Game/GameData/Skins/SuperMario/Ball.dat");
	LoadSprite(Skin2_LeftObs, "Bound-Console-Game/GameData/Skins/SuperMario/Left_Obs.dat");
	LoadSprite(Skin2_RightObs, "Bound-Console-Game/GameData/Skins/SuperMario/Right_Obs.dat");
	//Death Skin
	LoadSprite(Skin3_Ball, "Bound-Console-Game/GameData/Skins/Death/Ball.dat");
	LoadSprite(Skin3_LeftObs, "Bound-Console-Game/GameData/Skins/Death/Left_Obs.dat");
	LoadSprite(Skin3_RightObs, "Bound-Console-Game/GameData/Skins/Death/Right_Obs.dat");
	//Load Saved Data
	std::ifstream SavedSkin("Bound-Console-Game/GameData/Skins/SavedSkin");
	if (!SavedSkin.is_open())
	{
		std::ofstream out("Bound-Console-Game/GameData/Skins/SavedSkin");
		out << 1 << std::endl;
		out.close();
		DefaultSkin = 1;
	}else
		SavedSkin >> DefaultSkin;
	switch (DefaultSkin)
	{
	case 1:
		SBall = &Skin1_Ball;
		SLeftObs = &Skin1_LeftObs;
		SRightObs = &Skin1_RightObs;
		SkinName = "FLAPPY BIRD";
		break;
	case 2:
		SBall = &Skin2_Ball;
		SLeftObs = &Skin2_LeftObs;
		SRightObs = &Skin2_RightObs;
		SkinName = "SUPER MARIO";
		break;
	case 3:
		SBall = &Skin3_Ball;
		SLeftObs = &Skin3_LeftObs;
		SRightObs = &Skin3_RightObs;
		SkinName = "DEATH";
		break;
	}
	SavedSkin.close();
}

void Options()// Changed saveSkin directory to GameData/Skins 
{
	ScreenBuffer::fillBuffer(32, 0);
	DrawSprite(Label_Options, 24, 5);
	//Volume
	ScreenBuffer::drawLine(2, 15, 77, 15, 196, Color::FG_WHITE);
	ScreenBuffer::drawString(8, 15, " VOLUME ", Color::FG_GREEN);
	ScreenBuffer::drawString(10, 23, "MUSIC:", FG_BLUE);
	ScreenBuffer::drawLine(20, 23, 70, 23, 249, FG_YELLOW);
	for (int i = 20; i <= 70; i += 10) ScreenBuffer::draw(i, 23, 4, FG_YELLOW);
	ScreenBuffer::drawString(20, 21, "0", FG_YELLOW);
	ScreenBuffer::drawString(69, 21, "100", FG_YELLOW);
	ScreenBuffer::drawString(10, 31, "SFX:", FG_BLUE);
	ScreenBuffer::drawLine(20, 31, 70, 31, 249, FG_YELLOW);
	for (int i = 20; i <= 70; i += 10) ScreenBuffer::draw(i, 31, 4, FG_YELLOW);
	ScreenBuffer::drawString(20, 29, "0", FG_YELLOW);
	ScreenBuffer::drawString(69, 29, "100", FG_YELLOW);
	ScreenBuffer::drawString(2, 37, "USE ARROW KEYS TO ADJUST", FG_DARK_CYAN);
	int SorM = 1; // User is choosing sound or music
	//Skin
	ScreenBuffer::drawLine(2, 40, 77, 40, 196, Color::FG_WHITE);
	ScreenBuffer::drawString(9, 40, " SKIN ", Color::FG_GREEN);
	ScreenBuffer::drawRect(2, 44, 24, 69, 249, Color::FG_WHITE);
	ScreenBuffer::drawRect(28, 44, 77, 69, 249, Color::FG_WHITE);
	ScreenBuffer::drawString(4, 46, "[1] FLAPPY BIRD", Color::FG_YELLOW);
	ScreenBuffer::drawString(4, 48, "[2] SUPER MARIO", Color::FG_YELLOW);
	ScreenBuffer::drawString(4, 50, "[3] DEATH", Color::FG_YELLOW);
	ScreenBuffer::drawString(2, 71, "PRESS NUMBER KEYS TO SELECT", FG_DARK_CYAN);
	ScreenBuffer::drawString(52, 75, "[ENTER]: RETURN TO MENU", FG_DARK_CYAN);
	ScreenBuffer::drawToConsole();
	//Default
	ScreenBuffer::draw((int)((float)MLevel / 100.0 * 50.0) + 20, 24, 127, FG_GREEN);
	ScreenBuffer::draw((int)((float)SLevel / 100.0 * 50.0) + 20, 32, 127, FG_GREEN);
	ScreenBuffer::draw(8, 23, 16, FG_GREEN);

	DrawSprite(*SBall, 50, 49);
	DrawCrop(*SLeftObs, 29, 58, 36, 0, 49, 6);
	DrawCrop(*SRightObs, 53, 58, 0, 0, 23, 6);
	ScreenBuffer::drawString(30, 46, SkinName, Color::FG_YELLOW);
	ScreenBuffer::drawToConsole();
	//
	while (_kbhit()) _getch();
	int Key;
	while (true)
	{
		Key = _getch();
		switch (Key)
		{
		case 0x31:// User press button 1
			SBall = &Skin1_Ball;
			SLeftObs = &Skin1_LeftObs;
			SRightObs = &Skin1_RightObs;
			SkinName = "FLAPPY BIRD";
			DefaultSkin = 1;
			break;
		case 0x32:// User press button 2
			SBall = &Skin2_Ball;
			SLeftObs = &Skin2_LeftObs;
			SRightObs = &Skin2_RightObs;
			SkinName = "SUPER MARIO";
			DefaultSkin = 2;
			break;
		case 0x33:// User press button 3 
			SBall = &Skin3_Ball;
			SLeftObs = &Skin3_LeftObs;
			SRightObs = &Skin3_RightObs;
			SkinName = "DEATH";
			DefaultSkin = 3;
			break;
		case 77: // User press Right arrow key
			if (SorM)
			{
				MLevel += 20;
				if (MLevel > 100) MLevel = 100;
				ScreenBuffer::drawLine(20, 24, 70, 24, 219, 0);
				ScreenBuffer::draw((int)((float)MLevel / 100.0 * 50.0) + 20, 24, 127, FG_GREEN);
				AudioPlayer::SetMusicVolume(MLevel);
			}
			else
			{
				SLevel += 20;
				if (SLevel > 100) SLevel = 100;
				ScreenBuffer::drawLine(20, 32, 70, 32, 219, 0);
				ScreenBuffer::draw((int)((float)SLevel / 100.0 * 50.0) + 20, 32, 127, FG_GREEN);
				AudioPlayer::SetSFXVolume(SLevel);
			}
			break;
		case 75:// User press Left arrow key
			if (SorM)
			{
				MLevel -= 20;
				if (MLevel < 0) MLevel = 0;
				ScreenBuffer::drawLine(20, 24, 70, 24, 219, 0);
				ScreenBuffer::draw((int)((float)MLevel / 100.0 * 50.0) + 20, 24, 127, FG_GREEN);
				AudioPlayer::SetMusicVolume(MLevel);
			}
			else
			{
				SLevel -= 20;
				if (SLevel < 0) SLevel = 0;
				ScreenBuffer::drawLine(20, 32, 70, 32, 219, 0);
				ScreenBuffer::draw((int)((float)SLevel / 100.0 * 50.0) + 20, 32, 127, FG_GREEN);
				AudioPlayer::SetSFXVolume(SLevel);
			}
			break;
		case 72:
		case 80: // user press Up/Down arrow key
			SorM = 1 - SorM;
			if (SorM)
			{
				ScreenBuffer::drawLine(8, 23, 8, 31, 219, 0);
				ScreenBuffer::draw(8, 23, 16, FG_GREEN);
			}
			else
			{
				ScreenBuffer::drawLine(8, 23, 8, 31, 219, 0);
				ScreenBuffer::draw(8, 31, 16, FG_GREEN);
			}
			break;
		case 13:
			std::ofstream SavedSkin("SavedSkin");
			SavedSkin << DefaultSkin;
			SavedSkin.close();
			return;
		}
		ScreenBuffer::fillRect(29, 45, 76, 68, 219, Color::FG_BLACK);
		DrawSprite(*SBall, 50, 49);
		DrawCrop(*SLeftObs, 29, 58, 36, 0, 49, 6);
		DrawCrop(*SRightObs, 53, 58, 0, 0, 23, 6);
		ScreenBuffer::drawString(30, 46, SkinName, Color::FG_YELLOW);
		ScreenBuffer::drawToConsole();
	}
}

void Credits()
{
	ScreenBuffer::fillBuffer(32, 0);
	DrawSprite(Label_Credits, 25, 5);
	ScreenBuffer::drawString(10, 12, "____________________________________________________________");
	ScreenBuffer::drawString(16, 14, "THIS GAME IS A SCHOOL PROJECT CREATED BY A GROUP", FG_GREEN);
	ScreenBuffer::drawString(28, 15, "OF FIRST-YEAR STUDENTS", FG_GREEN);
	ScreenBuffer::drawString(16, 17, "BOUND IS 100% A COMMAND LINE GAME WHICH GRAPHICS", FG_GREEN);
	ScreenBuffer::drawString(19, 18, "ARE ALL CHARACTERS AND BACKGROUND COLORING", FG_GREEN);
	ScreenBuffer::drawString(20, 20, "CONSOLE RENDERING WAS INSPIRED BY JAVIDX9'S", FG_GREEN);
	ScreenBuffer::drawString(27, 21, "OLCCONSOLEGAMEENGINE PROJECT", FG_GREEN);
	ScreenBuffer::drawString(31, 30, "LEADER", FG_DARK_CYAN);
	ScreenBuffer::drawString(39, 30, "PHAM HONG VINH");
	ScreenBuffer::drawString(29, 35, "DESIGNER", FG_DARK_CYAN);
	ScreenBuffer::drawString(39, 35, "NGUYEN TRAN TRUNG");
	ScreenBuffer::drawString(27, 40, "PROGRAMMER", FG_DARK_CYAN);
	ScreenBuffer::drawString(39, 40, "VO TRONG GIA VINH");
	ScreenBuffer::drawString(39, 42, "LE THANH VIET");
	ScreenBuffer::drawString(39, 44, "BUI THANH UY");
	ScreenBuffer::drawString(21, 50, "MUSIC/SFX SOURCE", FG_DARK_CYAN);
	ScreenBuffer::drawString(39, 50, "FREESOUND.COM");
	ScreenBuffer::drawString(25, 55, "LIBRARY USED", FG_DARK_CYAN);
	ScreenBuffer::drawString(39, 55, "SDL2/MIXER");
	ScreenBuffer::drawString(52, 75, "[ENTER]: RETURN TO MENU", FG_DARK_CYAN);
	ScreenBuffer::drawToConsole();
	while (true)
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x8000) break;
	}
}

void Help()
{
	ScreenBuffer::fillBuffer(32, 0);
	DrawSprite(Label_Help, 32, 5);
	ScreenBuffer::drawString(10, 12, "____________________________________________________________");
	ScreenBuffer::drawString(10, 20, "ABOUT BOUND:", FG_GREEN);
	ScreenBuffer::drawString(24, 25, "Bound is an endless game, in which the goal is");
	ScreenBuffer::drawString(24, 26, "to keep your character alive while navigating");
	ScreenBuffer::drawString(24, 27, "through a series of obstacles. You wll get");
	ScreenBuffer::drawString(24, 28, "point when passed an you obstacle. Game over");
	ScreenBuffer::drawString(24, 29, "when your character hit the obstacles or the");
	ScreenBuffer::drawString(24, 30, "game border.");
	ScreenBuffer::drawString(10, 35, "HOW TO PLAY:", FG_GREEN);
	ScreenBuffer::drawString(24, 40, "[ ]/[ ]: MOVE TO LEFT/RIGHT");
	ScreenBuffer::draw(25, 40, 27); //draw left arrow
	ScreenBuffer::draw(29, 40, 26); //draw right arrow
	ScreenBuffer::drawString(24, 44, "[SPACE]: TO JUMP");
	ScreenBuffer::drawString(10, 49, "TIPS:", FG_GREEN);
	ScreenBuffer::drawString(24, 54, "JUMP TO REDUCE THE CHARACTER'S FALLING SPEED");
	ScreenBuffer::drawString(24, 58, "GO THROUGH THE GAP WITHOUT JUMPING TO GET MORE");
	ScreenBuffer::drawString(24, 59, "POINTS");
	ScreenBuffer::drawString(24, 63, "YOU CAN CHANGE YOUR CHARACTER AND OBSTACLES");
	ScreenBuffer::drawString(24, 64, "APPEARANCE AT THE OPTIONS MENU");
	ScreenBuffer::drawString(52, 75, "[ENTER]: RETURN TO MENU", FG_DARK_CYAN);
	ScreenBuffer::drawToConsole();
	while (true)
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x8000) break;
	}
}



// -------- FUNCTIONS FOR PLAYING STATE --------

/*
	Getting ready to start the game again. Reset/ Init ball XY or score,...
*/
// === HANDLE PLAY INPUT ===
bool next_space = false;
bool pre_space = false;
bool spacePressed;
bool leftPressed;
bool rightPressed;

void GameHandleInput() {
	leftPressed=false;
	rightPressed=false;

	next_space=GetAsyncKeyState(VK_SPACE)&0x8000;
	if(next_space&&(!pre_space))
		spacePressed=true;	
	if(next_space&&pre_space)
		spacePressed=false;	
	if(!next_space)
		spacePressed=false;
	pre_space=next_space;

	if(GetAsyncKeyState(VK_LEFT)&0x8000)
		leftPressed=true;

	if(GetAsyncKeyState(VK_RIGHT)&0x8000)
		rightPressed=1;
}

// === PLAY LOGIC ===

void GameLogic(float elapsedTime) {
	BallLogic(elapsedTime);
	ObstacleLogic(elapsedTime);
	DrawLogic();
	Collision();
}

void BallLogic(float elapsedTime) {
	if (spacePressed) {
		ControlBall::Jump();
		AudioPlayer::PlayEffect("Bound-Console-Game/GameData/Music/Jump.wav");
		bonus = 0;
	}
	if (leftPressed)
		ControlBall::GoLeft(elapsedTime);
	if (rightPressed)
		ControlBall::GoRight(elapsedTime);
	ControlBall::Fall(elapsedTime);
}

void UpdateObstacle(float elapsedTime)
{
	
	if (Obstacleupdate == NUMBER_OF_WALLS || Obstacle[Obstacleupdate].spaceY + WALL_HEIGHT <= ControlBall::getBall().y) //this is to make the Obstacle choosed won't be replace if we have go over it.
	{
		for (int i = 0; i < NUMBER_OF_WALLS; i++) //this is to take the name of the Obstacle that is lower than our Ball
		{
			if (Obstacle[i].spaceY > ControlBall::getBall().y)
			{
				Obstacleupdate = i;
				break;
			}
		}
	}
	if (Obstacle[Obstacleupdate].spaceX + SPACE_WIDTH  >= GAME_WIDTH) CaseUpdate = 2; 
	if (Obstacle[Obstacleupdate].spaceX <= 0) CaseUpdate = 1;
	
	switch (CaseUpdate)
	{
		case 1: 
			Obstacle[Obstacleupdate].spaceX += elapsedTime*5.0f;
			break;
		case 2: 
			Obstacle[Obstacleupdate].spaceX -= elapsedTime*5.0f; 
			break;
	}
	
}

void ObstacleLogic(float fElapsedTime)
{
	for (int i = 0; i < NUMBER_OF_WALLS - 1; i++)
	{
		Obstacle[i].spaceY -= 8.0f*fElapsedTime; //this is to keep the Obstacle (Wall) going up
		
		
		if (Obstacle[i].spaceY <= 0) // this to make the transition of wall which touch the top border to the bottom border smother.
		{
			Obstacle[i].passed = 0; //this is to know that this wall is not passed.(after recycled)
			Obstacle[NUMBER_OF_WALLS - 1].spaceX = Obstacle[i].spaceX;
			Obstacle[NUMBER_OF_WALLS - 1].spaceY = Obstacle[i].spaceY;
			Obstacle[i].spaceX = 1 + rand() % (GAME_WIDTH - SPACE_WIDTH - 1);
			Obstacle[i].spaceY = GAME_HEIGHT - 1 + Obstacle[NUMBER_OF_WALLS - 1].spaceY;
		}
		Obstacle[NUMBER_OF_WALLS - 1].spaceY -= 6.0f*fElapsedTime;
		
	}
	if (score >= MAKE_IT_HARDER && movingObstacle) 
		UpdateObstacle(fElapsedTime);
}

void DrawLogic()
{
	ControlBall::Ball& ball = ControlBall::getBall();
	if (ball.y > BALL_LIMIT*1.0f)
	{
		for (int i = 0; i < NUMBER_OF_WALLS; i++)
			Obstacle[i].spaceY -= ball.y - BALL_LIMIT*1.0f;
		ball.y = BALL_LIMIT*1.0f;
	}
	if (ball.x < ControlBall::BALL_RADIUS) ball.x = ControlBall::BALL_RADIUS;
	if (ball.x >= GAME_WIDTH - ControlBall::BALL_RADIUS - 1) ball.x = GAME_WIDTH - ControlBall::BALL_RADIUS - 1;
}

void Collision()
{
	ControlBall::Ball ball = ControlBall::getBall();
	int xball, yball, xspace, yspace;
	for (int i = 0; i < NUMBER_OF_WALLS; i++)
	{
		xball = ball.x + 0.5f;
		yball = ball.y + 0.5f;
		xspace = Obstacle[i].spaceX + 0.5f;
		yspace = Obstacle[i].spaceY + 0.5f;
		if (yspace - yball <= ControlBall::BALL_RADIUS && yspace - yball > -WALL_HEIGHT - ControlBall::BALL_RADIUS)
		{
			if (!(xball - xspace > ControlBall::BALL_RADIUS - 1 && xball - xspace < SPACE_WIDTH - ControlBall::BALL_RADIUS))
				gameOver = true;
		}
	}

	if (ball.y < 0) gameOver = true;

	for (int i = 0; i < NUMBER_OF_WALLS; i++)
	{
		if (ball.y >= Obstacle[i].spaceY + WALL_HEIGHT && Obstacle[i].passed == 0)
		{
			AudioPlayer::PlayEffect("Bound-Console-Game/GameData/Music/Point.wav");
			score+=pow(2, bonus);
			Obstacle[i].passed = 1;
			if (score >= MAKE_IT_HARDER) {
				movingObstacle = rand() % 2;
				CaseUpdate = 1 + rand() % 2;
			}
			bonus++;
		}
	}
}

// === PLAY DRAW ===



/*
	The update loop of our game
*/

void onGameDraw() {

	//Clear the whole screen
	ScreenBuffer::fillBuffer(' ');

	drawStage(1, 1, GAME_WIDTH, GAME_HEIGHT);

	drawHUD();
}

void drawHUD() {

	ScreenBuffer::drawRect(0, 0, GAME_WIDTH + 1, GAME_HEIGHT + 1, 219, 15);

	ScreenBuffer::drawLine(52, 1, 77, 1, 4, Color::FG_GREEN);
	ScreenBuffer::drawString(61, 1, " SCORE ", Color::FG_GREEN);
	std::string bestScoreStr = getStrFromInt(bestScore);
	ScreenBuffer::drawString(52, 15, "BEST: "+bestScoreStr, 10);

	ScreenBuffer::drawLine(52, 19, 77, 19, 4, Color::FG_CYAN);
	ScreenBuffer::drawString(60, 19, " CONTROL ", Color::FG_CYAN);
	ScreenBuffer::drawString(52, 23, "[SPACE] TO JUMP", 11);
	ScreenBuffer::drawString(52, 26, "[ ]/[ ] TO MOVE LEFT/RIGHT", 11);
	ScreenBuffer::draw(53, 26, 27, 11); ScreenBuffer::draw(57, 26, 26, 11);

	ScreenBuffer::drawLine(52, 30, 77, 30, 4, Color::FG_YELLOW);
	ScreenBuffer::drawString(62, 30, " TIPS ", Color::FG_YELLOW);
	ScreenBuffer::drawString(52, 34, "GO THROUGH THE GAP WITHOUT", Color::FG_YELLOW);
	ScreenBuffer::drawString(52, 36, "JUMPING TO GET MORE POINT", Color::FG_YELLOW);
	ScreenBuffer::drawString(52, 39, "JUMP TO REDUCE THE", Color::FG_YELLOW);
	ScreenBuffer::drawString(52, 41, "CHARACTER'S FALLING SPEED", Color::FG_YELLOW);
	ScreenBuffer::drawString(52, 44, "YOU CAN CHANGE YOUR", Color::FG_YELLOW);
	ScreenBuffer::drawString(52, 46, "CHARACTER'S SKIN AT", Color::FG_YELLOW);
	ScreenBuffer::drawString(52, 48, "MENU/OPTIONS", Color::FG_DARK_YELLOW);

	ScreenBuffer::drawString(59, 78, "BOUND GAME VER. 1.0", Color::FG_RED);

	UpdateAndShowScore();
}

void drawStage(int originX, int originY, int maxX, int maxY) {

	ControlBall::DrawBall(*SBall,originX,originY);
	for (int i = 0; i < NUMBER_OF_WALLS; i++) {
		int drawSpaceX = Obstacle[i].spaceX + 0.5f;
		int drawSpaceY = Obstacle[i].spaceY + 0.5f;
		DrawCrop(*SLeftObs, originX, originY + drawSpaceY, 50 - (originX + drawSpaceX - 1), 0, 49, 6);
		DrawCrop(*SRightObs, originX + drawSpaceX + SPACE_WIDTH, originY + drawSpaceY, 0, 0, 49 - (originX + drawSpaceX + SPACE_WIDTH), 6);
	}
}


void UpdateAndShowScore()
{
	int temp_score;
	int t;
	int x = 78;
	temp_score = score;
	if (temp_score == 0)
		DrawScore(temp_score, x - 3, 5);
	else
	{
		while (temp_score > 0)
		{
			t = temp_score % 10;
			DrawScore(t, x - 3, 5);
			x -= 4;
			temp_score /= 10;
		}
	}
}

void DrawScore(int temp, int x, int y)
{
	DrawSprite(scoreNumbers[temp], x, y);
}

int ReadBestScore()
{
	std::ifstream infile("Bound-Console-Game/GameData/bestScore");
	if (!infile.is_open())
	{
		std::ofstream out("Bound-Console-Game/GameData/bestScore");
		out << 0 << std::endl;
		out.close();
		bestScore = 0;
	}
	else
		infile >> bestScore;
	infile.close();
	return bestScore;
}

void SaveBestScore()
{
	if (score > bestScore)
	{
		bestScore = score;
		std::ofstream outfile("Bound-Console-Game/GameData/bestScore");
		if (outfile.is_open())
			outfile << bestScore;
		outfile.close();
	}
}

// -------- end --------