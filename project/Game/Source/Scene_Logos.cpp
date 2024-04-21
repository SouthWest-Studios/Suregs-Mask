#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene_Intro.h"
#include "Scene_Logos.h"
#include "ModuleFadeToBlack.h"
#include "Optick/include/optick.h"

#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "Menu.h"
Scene_Logos::Scene_Logos(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("scene_logos");
}

// Destructor
Scene_Logos::~Scene_Logos()
{}

// Called before render is available
bool Scene_Logos::Awake(pugi::xml_node config)
{
	LOG("Loading Scene_intro");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene_Logos::Start()
{
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	config = configFile.child("config").child(name.GetString());

	logoGamePath = config.child("logoGame").attribute("texturepath").as_string();
	logoUpcPath = config.child("logoUpc").attribute("texturepath").as_string();
	logoStudioPath = config.child("logoStudio").attribute("texturepath").as_string();
	backgroundPath = config.child("background").attribute("texturepath").as_string();
	logoGame = app->tex->Load(logoGamePath);
	logoUpc = app->tex->Load(logoUpcPath);
	logoStudio = app->tex->Load(logoStudioPath);
	background = app->tex->Load(backgroundPath);
	timerIntro.Start();

	//sus = app->audio->LoadAudioFx("");
	intro_fx = app->audio->LoadAudioFx("intro_fx");
	app->audio->PlayFx(intro_fx);

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	

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
	
	if (timerIntro.ReadSec() < 5) {

	}
	else {
		app->fadeToBlack->FadeToBlack(this, app->scene_intro, 90);
	}

	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		app->fadeToBlack->FadeToBlack(this, app->scene_intro, 90);
	}
	return true;
}

// Called each loop iteration
bool Scene_Logos::PostUpdate()
{
	app->render->DrawTexture(background, 0, 0);
	app->render->DrawTexture(logoGame, 500, 100);
	app->render->DrawTexture(logoUpc, 600, 400);
	app->render->DrawTexture(logoStudio, 300, 350);
	bool ret = true;

	return ret;
}

// Called before quitting
bool Scene_Logos::CleanUp()
{
	LOG("Freeing Scene_intro");
	app->tex->UnLoad(logoGame);
	return true;
}