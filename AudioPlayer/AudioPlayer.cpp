#include"AudioPlayer.h"

using namespace std;
namespace AudioPlayer {
	void initPlayer()
	{ // call Mix_CleanUp after using
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
		{
			cout << Mix_GetError();
		}
	}

	int PlayBackgroundMusic(string file)
	{
		int i = 0;
		char *name = new char[file.size() + 1];
		for (int i = 0; i < file.size(); i++) name[i] = file[i];
		name[file.size()] = 0;
		Mix_Music *music = NULL;
		music = Mix_LoadMUS(name);
		delete[] name;
		if (music == NULL)
		{
			cout << Mix_GetError();
			return 0;
		}
		Mix_PlayMusic(music, -1);
		if (!Mix_PlayingMusic()) 
			Mix_FreeMusic(music);
		return 1;
	}
	int PlayEffect(string file)
	{
		Mix_Chunk *effect = NULL;
		char *name = new char[file.size() + 1];
		for (int i = 0; i < file.size(); i++) name[i] = file[i];
		name[file.size()] = 0;
		effect = Mix_LoadWAV(name);
		delete[] name;

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
}