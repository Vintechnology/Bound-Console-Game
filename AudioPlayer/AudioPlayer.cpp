#include"AudioPlayer.h"

using namespace std;

namespace AudioPlayer {
	void initPlayer()
	{ 
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
		{
			cout << Mix_GetError();
		}
	}

	int PlayBackgroundMusic(string file)
	{
		Mix_Music *music = NULL;
		music = Mix_LoadMUS(file.c_str());
		if (music == NULL)
		{
			cout << Mix_GetError();
			return 0;
		}
		Mix_PlayMusic(music, -1);
		if (!Mix_PlayingMusic()) Mix_FreeMusic(music);
		return 1;
	}
	int PlayEffect(string file)
	{
		Mix_Chunk *effect = NULL;
		effect = Mix_LoadWAV(file.c_str());
		if (effect == NULL)
		{
			cout << Mix_GetError();
			return 0;
		}

		Mix_PlayChannel(-1, effect, 0);

		if (!Mix_Playing(-1)) 
			Mix_FreeChunk(effect);
		return 1;
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
}