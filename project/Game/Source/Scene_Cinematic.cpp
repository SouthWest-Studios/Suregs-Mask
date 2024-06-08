#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene_Intro.h"
#include "Scene_Cinematic.h"
#include "Scene_Testing.h"
#include "ModuleFadeToBlack.h"
#include "Scene_Pueblo.h"
#include "Optick/include/optick.h"
#include "Scene_Pueblo_Tutorial.h"
#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "Menu.h"

Scene_Cinematic::Scene_Cinematic(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name = ("scene_cinematic");
}

// Destructor
Scene_Cinematic::~Scene_Cinematic()
{}

// Called before render is available
bool Scene_Cinematic::Awake(pugi::xml_node config)
{
	LOG("Loading Scene_Cinematic");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene_Cinematic::Start()
{
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	config = configFile.child("config").child(name.GetString()).child("sceneLogos");

	TSprite = config.attribute("Tsprite").as_int();
	SpriteX = config.attribute("sprite_x").as_int();
	SpriteY = config.attribute("sprite_y").as_int();
	Photowidth = config.attribute("Pwidth").as_int();
	spritePositions = SPosition.SpritesPos(TSprite, SpriteX, SpriteY, Photowidth);

	for (int j = 0; j < 10; ++j) {
		SDL_Texture* currentSceneLogosTexture = nullptr;


		switch (j) {
		case 0:
			currentSceneLogosTexture = app->tex->Load(config.attribute("texturepath1").as_string());;
			break;
		case 1:
			currentSceneLogosTexture = app->tex->Load(config.attribute("texturepath2").as_string());
			break;
		case 2:
			currentSceneLogosTexture = app->tex->Load(config.attribute("texturepath3").as_string());
			break;
		case 3:
			currentSceneLogosTexture = app->tex->Load(config.attribute("texturepath4").as_string());
			break;
		case 4:
			currentSceneLogosTexture = app->tex->Load(config.attribute("texturepath5").as_string());
			break;
		case 5:
			currentSceneLogosTexture = app->tex->Load(config.attribute("texturepath6").as_string());
			break;
		case 6:
			currentSceneLogosTexture = app->tex->Load(config.attribute("texturepath7").as_string());
			break;
		case 7:
			currentSceneLogosTexture = app->tex->Load(config.attribute("texturepath8").as_string());
			break;
		case 8:
			currentSceneLogosTexture = app->tex->Load(config.attribute("texturepath9").as_string());
			break;
		case 9:
			currentSceneLogosTexture = app->tex->Load(config.attribute("texturepath10").as_string());
			break;
		default:
			break;
		}

		if (currentSceneLogosTexture == nullptr) {
			continue;
		}

		for (int i = 0; i < TSprite; ++i) {

			SDL_Rect framePosition = spritePositions[i];


			SDL_Texture* frameTexture = SDL_CreateTexture(app->render->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, framePosition.w, framePosition.h);


			SDL_SetRenderTarget(app->render->renderer, frameTexture);


			SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, 0);
			SDL_RenderClear(app->render->renderer);


			SDL_Rect sourceRect = { framePosition.x, framePosition.y, framePosition.w, framePosition.h };
			SDL_RenderCopy(app->render->renderer, currentSceneLogosTexture, &sourceRect, NULL);


			SDL_SetRenderTarget(app->render->renderer, NULL);


			frameTextures.push_back(frameTexture);
		}

		app->tex->UnLoad(currentSceneLogosTexture);
	}


	app->tex->UnLoad(sceneLogosTexture);

	timerIntro.Start();

	//sus = app->audio->LoadAudioFx("");
	app->audio->StopMusic();
	intro_fx = app->audio->LoadAudioFx("cinematic_fx");
	app->audio->PlayFx(intro_fx);

	app->render->camera.x = 0;
	app->render->camera.y = 0;


	return true;
}

// Called each loop iteration
bool Scene_Cinematic::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene_Cinematic::Update(float dt)
{

	OPTICK_EVENT();

	//if (timerIntro.ReadSec() < 90) {

	//}
	//else {
	//	app->fadeToBlack->FadeToBlack(this, app->scene_intro, 90);

	//}
	if (app->input->GetButton(CONFIRM) == KEY_REPEAT ) {
		if (timerIntro.ReadMSec() >= 5000) {
		app->fadeToBlack->FadeToBlack(this, app->scene_pueblo_tutorial, 90);
		}
	}
	else {
		timerIntro.Start();
	}


	printf("\n%f", timerIntro.ReadMSec());
	/*if (app->input->GetButton(CONFIRM) == KEY_DOWN && !app->scene_pueblo->GetRod()->fishing.rodReady && !app->scene_pueblo_tutorial->GetRod()->fishing.rodReady) {
		app->fadeToBlack->FadeToBlack(this, app->scene_intro, 90);

	}*/
	return true;
}

// Called each loop iteration
bool Scene_Cinematic::PostUpdate()
{

	SDL_Texture* currentFrameTexture = frameTextures[currentFrameIndex];
	SDL_Rect rect = { 0,0,600,338 };

	app->render->DrawTexture(currentFrameTexture, 0, 0, 2.15, SDL_FLIP_NONE, &rect, 0);

	currentFrameIndex += 0.16;
	if (currentFrameIndex >= frameTextures.size()) {
		app->fadeToBlack->FadeToBlack(this, app->scene_pueblo_tutorial, 90);
		currentFrameIndex = frameTextures.max_size();
	}


	bool ret = true;

	return ret;
}

// Called before quitting
bool Scene_Cinematic::CleanUp()
{
	LOG("Freeing Scene_intro");
	app->tex->UnLoad(sceneLogosTexture);
	frameTextures.clear();
	RELEASE(spritePositions);
	delete spritePositions;
	currentFrameIndex = 0;
	return true;
}