#include<iostream>
#include<string>
#include<SDL.h>
#include"SDL_mixer.h"

using namespace std;
namespace AudioPlayer {
	void initPlayer();
	int PlayBackgroundMusic(string file);
	int PlayEffect(string file);
	void PauseMusic();
	void ResumeMusic();
	void CleanUp();
}