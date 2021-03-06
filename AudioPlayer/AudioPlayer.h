#include<iostream>
#include<string>
#include<SDL.h>
#include"SDL_mixer.h"

using namespace std;
namespace AudioPlayer {
	void initPlayer();
	void PlayBackgroundMusic(string file);
	void PlayEffect(string file);
	void PauseMusic();
	void ResumeMusic();
	void CleanUp();
	void SetMusicVolume(int volume);
	void SetSFXVolume(int volume);
}