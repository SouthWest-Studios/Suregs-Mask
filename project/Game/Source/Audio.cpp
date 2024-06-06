#include "App.h"
#include "Audio.h"

#include "Defs.h"
#include "Log.h"

// NOTE: Recommended using: Additional Include Directories,
// instead of 'hardcoding' library location path in code logic
#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include <iostream>
#include <fstream>

// NOTE: Library linkage is configured in Linker Options
//#pragma comment(lib, "../Game/Source/External/SDL_mixer/libx86/SDL2_mixer.lib")

Audio::Audio(App* app, bool start_enabled) : Module(app, start_enabled)
{
	music = NULL;
	name = ("audio");

	// Inicializar lista de NPCs
	NPCs = {
		{"Jakov", 1},
		{"Dorothy", 2},
		{"Hely", 2},
		{"Pathy", 2},
		{"Habyg", 3},
		{"Marthyn", 3},
		{"Patzo", 1},
		{"Patza", 2},
		{"Hemyl", 3},
		{"Huhan", 2},
		{"Xiby", 2},
		{"Ferny", 3},
		{"Lixy", 2},
		{"Phylippa", 2},
		{"Rapho", 3},
		{"Sukhy", 2},
		{"Zupho", 1},
		{"Zuphe", 1},
		{"Fukho", 1},
		{"Xinnoh", 1},
		{"Phoska", 2},
		{"Phrumo", 2},
		{"Zhyuka", 2},
		{"Sphar", 3},
		{"Igorv", 3},
	};

	songs = {
		{"menu", 2},
		{"town", 127000},
		{"credits", 2},				 // Falta ajustar
		{"dungeon", 2}
	};
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

	playingMusic = true;

	LOG("Successfully playing %s", path);
	return ret;
}

bool Audio::PlayAmbience(const char* path, float fadeTime)
{
	bool ret = true;

	if (!active)
		return false;

	if (music != NULL)
	{
		if (fadeTime > 0.0f)
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

	if (music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if (fadeTime > 0.0f)
		{
			if (Mix_FadeInMusic(music, -1, (int)(fadeTime * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if (Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	Mix_VolumeMusic(volumeMusic);

	playingAmbience = true;

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
bool Audio::PlayFx(unsigned int id, int repeat, int channel)
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

bool Audio::PlayFxTimed(unsigned int id, int time, int channel, int repeat)
{
	if (!active) {
		return false; // Audio no activo
	}

	if (id > 0 && id <= fx.Count())
	{
		Mix_PlayChannelTimed(channel, fx[id - 1], repeat, time);
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

bool Audio::LoadAudioAmbience(const char* name, float fadeTime)
{
	pugi::xml_document config;
	pugi::xml_parse_result parseResult = config.load_file("config.xml");
	pugi::xml_node audioNode;
	audioNode = config.child("config").child("sounds").child("music").child(name);

	return PlayAmbience(audioNode.attribute("path").as_string(), fadeTime);
}

unsigned int Audio::LoadAudioFx(const char* name)
{
	pugi::xml_document config;
	pugi::xml_parse_result parseResult = config.load_file("config.xml");
	pugi::xml_node audioNode;
	audioNode = config.child("config").child("sounds").child("fx").child(name);


	return LoadFx(audioNode.attribute("path").as_string());
}

void Audio::FxGroup(int tag, int channel) 
{

}

void Audio::PlayMusicAfterDelay(const char* name, float delayInSeconds, float fadeTime)
{
	// Esperar el tiempo especificado
	if (musicTimer.ReadSec() >= delayInSeconds) {
		// Una vez que haya pasado el tiempo especificado, reproducir la canción
		LoadAudioMusic(name, fadeTime);
		playingMusic = true;
	}
}

void Audio::PlayMusicAfterRandomDelay(const char* name, float fadeTime)
{
	// Esperar el tiempo especificado
	if (musicTimerRand.ReadSec() >= 5) {

		int music = rand() % 5;
		//printf("Music: %d\n", music);

		switch (music)
		{
		case 3:
			StopMusic(0.0f);
			LoadAudioMusic(name, fadeTime);
			playingMusic = true;
			musicTimer.Start();

			// Buscar el nombre de la canción en la lista de canciones
			musicDuration = -1; // Inicializar el tiempo de la canción como -1 (indicando que no se encontró la canción)
			for (const auto& song : songs) {
				if (song.name == name) {
					musicDuration = song.time;
					break;
				}
			}

			if (musicDuration == -1) {
				LOG("Error: La canción '%s' no se encontró en la lista de canciones.", name);
			}
			break;
		}
		musicTimerRand.Start();
	}
}

void Audio::PlayRandomFx(unsigned int name, unsigned int name2, unsigned int name3)
{
	if (name3 == NULL) {
		int fx = rand() % 2 + 1;

		switch (fx)
		{
		case 1:
			PlayFx(name);
			break;
		case 2:
			PlayFx(name2);
			break;
		}
	}
	else {
		int fx = rand() % 3 + 1;

		switch (fx)
		{
		case 1:
			PlayFx(name);
			break;
		case 2:
			PlayFx(name2);
			break;
		case 3:
			PlayFx(name3);
			break;
		}
	}
}

void Audio::PlayRunFx(unsigned int name, unsigned int name2, unsigned int name3)
{
	if (runTimer.ReadMSec() > runTime) {

		int run = rand() % 3 + 1;

		switch (run)
		{
		case 1:
			PlayFx(name, 0, 1);
			break;
		case 2:
			PlayFx(name2, 0, 1);
			break;
		case 3:
			PlayFx(name3, 0, 1);
			break;
		}
		runTimer.Start();
	}
}

void Audio::PlayTimedFx(unsigned int name, int time) 
{
	if (timer.ReadMSec() > time) {

		PlayFx(name);
		timer.Start();
	}
}

void Audio::PlayRandomTimedFx(unsigned int name, unsigned int name2, unsigned int name3, int time) 
{

	if (altTimer.ReadMSec() > time) {

		if (name3 == NULL) {
			int fx = rand() % 2 + 1;

			switch (fx)
			{
			case 1:
				PlayFx(name);
				altTimer.Start();
				break;
			case 2:
				PlayFx(name2);
				altTimer.Start();
				break;
			}
		}
		else {
			int fx = rand() % 3 + 1;

			switch (fx)
			{
			case 1:
				PlayFx(name);
				altTimer.Start();
				break;
			case 2:
				PlayFx(name2);
				altTimer.Start();
				break;
			case 3:
				PlayFx(name3);
				altTimer.Start();
				break;
			}
		}

	}

}

bool Audio::LoadState(pugi::xml_node node)
{
	pugi::xml_document saveFile;
	pugi::xml_parse_result result;

	result = saveFile.load_file("save_general.xml");

	volumeMusic = saveFile.child("game_state").child("audio").child("music").attribute("volume").as_int(64);
	volumeFx = saveFile.child("game_state").child("audio").child("fx").attribute("volume").as_int(64);

	Mix_VolumeMusic(volumeMusic);
	Mix_Volume(-1, volumeFx);


	if (app->savedGame == 1) {
		app->tiempoDeJuego = saveFile.child("game_state").child("tiempoDeJuego").child("slot1").attribute("tiempo").as_llong(0);
	}
	else if (app->savedGame == 2) {
		app->tiempoDeJuego = saveFile.child("game_state").child("tiempoDeJuego").child("slot2").attribute("tiempo").as_llong(0);
	}
	else  if (app->savedGame == 3) {
		app->tiempoDeJuego = saveFile.child("game_state").child("tiempoDeJuego").child("slot3").attribute("tiempo").as_llong(0);
	}

	app->tiempoDeJuegoMostrarSlot1 = saveFile.child("game_state").child("tiempoDeJuego").child("slot1").attribute("tiempo").as_llong(0);
	app->tiempoDeJuegoMostrarSlot2 = saveFile.child("game_state").child("tiempoDeJuego").child("slot2").attribute("tiempo").as_llong(0);
	app->tiempoDeJuegoMostrarSlot3 = saveFile.child("game_state").child("tiempoDeJuego").child("slot3").attribute("tiempo").as_llong(0);


	

	return true;
}

bool Audio::SaveState(pugi::xml_node node)
{

	pugi::xml_document saveFile;
	pugi::xml_node gameState = saveFile.append_child("game_state");
	pugi::xml_node audio = gameState.append_child("audio");

	pugi::xml_node tiempoDeJuego = gameState.append_child("tiempoDeJuego");

	audio.append_child("music").append_attribute("volume").set_value(volumeMusic);
	audio.append_child("fx").append_attribute("volume").set_value(volumeFx);

	if (app->savedGame == 1) {
		tiempoDeJuego.append_child("slot1").append_attribute("tiempo").set_value(app->tiempoDeJuego);
		tiempoDeJuego.append_child("slot2").append_attribute("tiempo").set_value(app->tiempoDeJuegoMostrarSlot2);
		tiempoDeJuego.append_child("slot3").append_attribute("tiempo").set_value(app->tiempoDeJuegoMostrarSlot3);
	}
	else if (app->savedGame == 2) {
		tiempoDeJuego.append_child("slot1").append_attribute("tiempo").set_value(app->tiempoDeJuegoMostrarSlot1);
		tiempoDeJuego.append_child("slot2").append_attribute("tiempo").set_value(app->tiempoDeJuego);
		tiempoDeJuego.append_child("slot3").append_attribute("tiempo").set_value(app->tiempoDeJuegoMostrarSlot3);
	}
	else  if (app->savedGame == 3) {
		tiempoDeJuego.append_child("slot1").append_attribute("tiempo").set_value(app->tiempoDeJuegoMostrarSlot1);
		tiempoDeJuego.append_child("slot2").append_attribute("tiempo").set_value(app->tiempoDeJuegoMostrarSlot2);
		tiempoDeJuego.append_child("slot3").append_attribute("tiempo").set_value(app->tiempoDeJuego);
	}
	


	saveFile.save_file("save_general.xml");

	return true;
}