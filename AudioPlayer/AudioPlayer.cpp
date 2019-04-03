#include"AudioPlayer.h"

using namespace std;

namespace AudioPlayer {
	void initPlayer()
	{ 
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
		{
			cout << Mix_GetError();// Do something else
		}
	}

	void PlayBackgroundMusic(string file)
	{
		Mix_Music *music = NULL;
		music = Mix_LoadMUS(file.c_str());
		Mix_PlayMusic(music, -1);
		if (!Mix_PlayingMusic()) Mix_FreeMusic(music);
	}
	void PlayEffect(string file)
	{
		Mix_Chunk *effect = NULL;
		effect = Mix_LoadWAV(file.c_str());
		Mix_PlayChannel(-1, effect, 0);
		if (!Mix_Playing(-1)) Mix_FreeChunk(effect);
	}
	void PauseMusic()
	{
		Mix_PauseMusic();
	}
	void ResumeMusic()
	{
		Mix_ResumeMusic();
	}
	void CleanUp()
	{	//call after finished using
		Mix_CloseAudio();
	}
	void IncreaseMusicVolume()
	{
		Mix_VolumeMusic(Mix_VolumeMusic(-1) + 1);
	}
	void DecreaseMusicVolume()
	{
		Mix_VolumeMusic(Mix_VolumeMusic(-1) - 1);
	}
	void IncreaseSFXVolume()
	{
		Mix_Volume(-1, Mix_Volume(-1, -1) + 1);
	}
	void DecreaseSFXVolume()
	{
		Mix_Volume(-1, Mix_Volume(-1, -1) - 1);
	}
}