#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene_Intro.h"
#include "Scene_Menu.h"
#include "ModuleFadeToBlack.h"
#include "Optick/include/optick.h"

#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "Menu.h"
Scene_Intro::Scene_Intro(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("scene_intro");
}

// Destructor
Scene_Intro::~Scene_Intro()
{}

// Called before render is available
bool Scene_Intro::Awake(pugi::xml_node config)
{
	LOG("Loading Scene_intro");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene_Intro::Start()
{
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	config = configFile.child("config").child(name.GetString());

	texturaIntroPath = config.child("texturaIntro").attribute("texturepath").as_string();
	texturaIntro = app->tex->Load(texturaIntroPath);
	timerIntro.Start();
	sus = app->audio->LoadAudioFx("");
	app->audio->PlayFx(sus);

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	

	return true;
}

// Called each loop iteration
bool Scene_Intro::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene_Intro::Update(float dt)
{

	OPTICK_EVENT();
	
	if (timerIntro.ReadSec() < 5 ) {
		app->render->DrawTexture(texturaIntro, 0, 0);
	}
	else {
		app->fadeToBlack->FadeToBlack(this, app->scene_menu, 90);
	}

	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		app->fadeToBlack->FadeToBlack(this, app->scene_menu, 90);
	}
	return true;
}

// Called each loop iteration
bool Scene_Intro::PostUpdate()
{
	app->render->DrawTexture(texturaIntro, 0, 0);
	bool ret = true;

	return ret;
}

// Called before quitting
bool Scene_Intro::CleanUp()
{
	LOG("Freeing Scene_intro");
	app->tex->UnLoad(texturaIntro);
	return true;
}