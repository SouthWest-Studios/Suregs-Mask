#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene_intro.h"
#include "Scene.h"
#include "ModuleFadeToBlack.h"
#include "Optick/include/optick.h"

#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"

Scene_intro::Scene_intro(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("Scene_intro");
}

// Destructor
Scene_intro::~Scene_intro()
{}

// Called before render is available
bool Scene_intro::Awake(pugi::xml_node config)
{
	LOG("Loading Scene_intro");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene_intro::Start()
{
	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	placeholder = app->tex->Load("Assets/Textures/martiVSnosotros.jpg");


	return true;
}

// Called each loop iteration
bool Scene_intro::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene_intro::Update(float dt)
{

	OPTICK_EVENT();
	

	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		app->fadeToBlack->FadeToBlack(this, app->scene, 90);
	}
	return true;
}

// Called each loop iteration
bool Scene_intro::PostUpdate()
{
	app->render->DrawTexture(placeholder, 0, 0);
	bool ret = true;

	return ret;
}

// Called before quitting
bool Scene_intro::CleanUp()
{
	LOG("Freeing Scene_intro");

	return true;
}