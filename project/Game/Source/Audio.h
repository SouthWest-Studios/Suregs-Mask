#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Module.h"
#include "Timer.h"

#include <map>

#define DEFAULT_MUSIC_FADE_TIME 2.0f

using namespace std;

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

	// Stop playing a music
	bool StopMusic(float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int channel = -1, int repeat = 0);

	// Stop playing a WAV
	bool StopFx(int channel);

	// Play a music from the config
	bool LoadAudioMusic(const char* name, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	// Play a WAV from the config
	unsigned int LoadAudioFx(const char* name);

	// Play a music after the specified time
	void PlayMusicAfterDelay(const char* name, float delayInSeconds, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	// Play a random WAV
	void PlayRandomFx(unsigned int name, unsigned int name2, unsigned int name3);

	// Play a WAV after the specified time
	void PlayRunFx(unsigned int name, unsigned int name2, unsigned int name3);

	void PlayHitFx(unsigned int name);

public:

	int volumeMusic;
	int volumeFx;

	bool playingMusic = false;
	bool playingRunFx = false;
	Timer timer;

	map<unsigned int, int> activeChannels;

private:

	int runTime = 840;
	Timer runTimer;

	int hitTime = 575;
	Timer hitTimer;

	_Mix_Music* music;
	List<Mix_Chunk *>	fx;
};

#endif // __AUDIO_H__