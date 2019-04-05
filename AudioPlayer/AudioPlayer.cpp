#include"AudioPlayer.h"

using namespace std;

namespace AudioPlayer {
	void initPlayer()
	{ 
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		Mix_Volume(-1, 128);
		Mix_VolumeMusic(128);
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
	void SetMusicVolume(int volume)
	{
		Mix_VolumeMusic(volume*1.28);
	}
	void SetSFXVolume(int volume)
	{
		Mix_Volume(-1, volume*128);
	}
}