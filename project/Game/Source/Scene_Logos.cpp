#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene_Intro.h"
#include "Scene_Logos.h"
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
#include "Animation.h"
Scene_Logos::Scene_Logos(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name = ("scene_logos");
}

// Destructor
Scene_Logos::~Scene_Logos()
{}

// Called before render is available
bool Scene_Logos::Awake(pugi::xml_node config)
{
	LOG("Loading Scene_logos");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene_Logos::Start()
{
	
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	config = configFile.child("config").child(name.GetString()).child("sceneLogos");

	int TSprite = config.attribute("Tsprite").as_int();
	int SpriteX = config.attribute("sprite_x").as_int();
	int SpriteY = config.attribute("sprite_y").as_int();
	int Photowidth = config.attribute("Pwidth").as_int();
	spritePositions = SPosition.SpritesPos(TSprite, SpriteX, SpriteY, Photowidth);
	sceneLogosTexture = app->tex->Load(config.attribute("texturepath").as_string());

	sceneLogos.LoadAnim("scene_logos", "idleAnim", spritePositions);
	timerIntro.Start();
	app->entityManager->canShowFinal = false;
	app->entityManager->showFinal = false;
	
	app->audio->StopMusic();
	intro_fx = app->audio->LoadAudioFx("intro_fx");
	app->audio->PlayFx(intro_fx);

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	//app->render->DrawTexture(lastFrame, 0, 0);
	currentAnimation = &sceneLogos;

	return true;
}

// Called each loop iteration
bool Scene_Logos::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene_Logos::Update(float dt)
{
	
	OPTICK_EVENT();
	//app->fadeToBlack->FadeToBlack(this, app->scene_testing, 10); /*BORRAR AL TERMINAR*/
	//SDL_Rect currentFrameRect = sceneLogos.GetCurrentFrame();
	//app->render->DrawTexture(sceneLogosTexture, currentFrameRect.x, currentFrameRect.y);
	////printf("holal");

	//sceneLogos.Reset();
	//printf("\nFinish");
	if (timerIntro.ReadSec() < 10) {

	}
	else {
		//sceneLogos.Reset();
		app->fadeToBlack->FadeToBlack(this, app->scene_intro, 90);
		
	}

	if (app->input->GetButton(CONFIRM) == KEY_DOWN && !app->scene_pueblo->GetRod()->fishing.rodReady  && !app->scene_pueblo_tutorial->GetRod()->fishing.rodReady) {
		app->fadeToBlack->FadeToBlack(this, app->scene_intro, 90);
		
	}

	currentAnimation->Update();
	return true;
}

// Called each loop iteration
bool Scene_Logos::PostUpdate()
{
	
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(sceneLogosTexture, 0,0, 1, SDL_FLIP_NONE, &rect, 0);


	//if (timerRecibirDanioColor.ReadMSec() <= 100) {
	//	float alpha = (100 - timerRecibirDanioColor.ReadMSec()) / 100;
	//	SDL_SetTextureAlphaMod(texture, static_cast<Uint8>(255 * alpha)); // Ajusta la opacidad

	//}
	//else {
	//	SDL_SetTextureAlphaMod(texture, 255);
	//}

		
	//app->render->DrawTexture(background, 0, 0);
	//app->render->DrawTexture(logoGame, 500, 100);
	//app->render->DrawTexture(lastFrame, 0, 0);
	//app->render->DrawTexture(logoStudio, 300, 350);
	bool ret = true;

	return ret;
}

// Called before quitting
bool Scene_Logos::CleanUp()
{
	LOG("Freeing Scene_intro");
	//printf("\nanular");
	app->tex->UnLoad(sceneLogosTexture);
	RELEASE(spritePositions);
	delete spritePositions;
	
	return true;
}