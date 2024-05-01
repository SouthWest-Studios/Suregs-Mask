#include "App.h"
#include "Audio.h"

#include "Defs.h"
#include "Log.h"

// NOTE: Recommended using: Additional Include Directories,
// instead of 'hardcoding' library location path in code logic
#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

// NOTE: Library linkage is configured in Linker Options
//#pragma comment(lib, "../Game/Source/External/SDL_mixer/libx86/SDL2_mixer.lib")

Audio::Audio(App* app, bool start_enabled) : Module(app, start_enabled)
{
	music = NULL;
	name.Create("audio");
}

// Destructor
Audio::~Audio()
{}

// Called before render is available
bool Audio::Awake(pugi::xml_node config)
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// Load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	// Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	volumeMusic = config.child("music").attribute("volume").as_int();
	volumeFx = config.child("fx").attribute("volume").as_int();

	return ret;
}

// Called before quitting
bool Audio::CleanUp()
{
	if(!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	ListItem<Mix_Chunk*>* item;
	for(item = fx.start; item != NULL; item = item->next)
		Mix_FreeChunk(item->data);

	fx.Clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool Audio::PlayMusic(const char* path, float fadeTime)
{
	bool ret = true;

	if(!active)
		return false;

	if(music != NULL)
	{
		if(fadeTime > 0.0f)
		{
			Mix_FadeOutMusic(int(fadeTime * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if(music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fadeTime > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fadeTime * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	Mix_VolumeMusic(volumeMusic);

	LOG("Successfully playing %s", path);
	return ret;
}

bool Audio::StopMusic(float fadeTime)
{
	if (!active || music == NULL) {
		return false;
	}

	if (fadeTime > 0.0f) {
		Mix_FadeOutMusic(int(fadeTime * 1000.0f));
	}
	else {
		Mix_HaltMusic();
	}

	Mix_FreeMusic(music);
	music = NULL;

	playingMusic = false;

	return true;
}

// Load WAV
unsigned int Audio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if(!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.Add(chunk);
		ret = fx.Count();
	}

	return ret;
}

// Play WAV
bool Audio::PlayFx(unsigned int id, int channel, int repeat)
{
	if (!active) {
		return false; // Audio no activo
	}

	if (id > 0 && id <= fx.Count())
	{
		Mix_PlayChannel(channel, fx[id - 1], repeat);
	}

	// Asociar el ID del efecto de sonido con el canal de reproducción
	activeChannels[id] = channel;

	return true;
}

bool Audio::StopFx(int channel)
{
	Mix_HaltChannel(channel);
	return false;
}

bool Audio::LoadAudioMusic(const char* name, float fadeTime)
{
	pugi::xml_document config;
	pugi::xml_parse_result parseResult = config.load_file("config.xml");
	pugi::xml_node audioNode;
	audioNode = config.child("config").child("sounds").child("music").child(name);

	return PlayMusic(audioNode.attribute("path").as_string(), fadeTime);
}

unsigned int Audio::LoadAudioFx(const char* name)
{
	pugi::xml_document config;
	pugi::xml_parse_result parseResult = config.load_file("config.xml");
	pugi::xml_node audioNode;
	audioNode = config.child("config").child("sounds").child("fx").child(name);


	return LoadFx(audioNode.attribute("path").as_string());
}

void Audio::PlayMusicAfterDelay(const char* name, float delayInSeconds, float fadeTime)
{
	// Esperar el tiempo especificado
	if (timer.ReadSec() >= delayInSeconds) {
		// Una vez que haya pasado el tiempo especificado, reproducir la canción
		LoadAudioMusic(name, fadeTime);
		playingMusic = true;
	}
}

void Audio::PlayRunFx(unsigned int name, unsigned int name2, unsigned int name3)
{
	if (runTimer.ReadMSec() > runTime) {

		int run = rand() % 3 + 1;

		switch (run)
		{
		case 1:
			PlayFx(name, 1);
			break;
		case 2:
			PlayFx(name2, 1);
			break;
		case 3:
			PlayFx(name3, 1);
			break;
		}
		runTimer.Start();
	}
}

void Audio::PlayHitFx(unsigned int name)
{
	if (hitTimer.ReadMSec() > hitTime) {

		PlayFx(name);
		hitTimer.Start();
	}
}