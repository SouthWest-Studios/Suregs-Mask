#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Module.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class Audio : public Module
{
public:

	Audio(App* app, bool start_enabled = true);

	// Destructor
	virtual ~Audio();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int channel = -1, int repeat = 0);

	// Play a music from the config
	bool LoadAudioMusic(const char* name, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	// Play a WAV from the config
	unsigned int LoadAudioFx(const char* name);

public:

	int volumeMusic;
	int volumeFx;

private:

	_Mix_Music* music;
	List<Mix_Chunk *>	fx;
};

#endif // __AUDIO_H__