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
	name.Create("Scene_intro");
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
	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	placeholder = app->tex->Load("Assets/Textures/Interfaz/intro_textura.png");
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
	
	if (timerIntro.ReadSec() < 10) {
		app->render->DrawTexture(placeholder, 0, 0);
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
	app->render->DrawTexture(placeholder, 0, 0);
	bool ret = true;

	return ret;
}

// Called before quitting
bool Scene_Intro::CleanUp()
{
	LOG("Freeing Scene_intro");
	app->tex->UnLoad(placeholder);
	return true;
}