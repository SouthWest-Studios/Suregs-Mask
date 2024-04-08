#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene_menu.h"
#include "Scene_Testing.h"
#include "ModuleFadeToBlack.h"
#include "Optick/include/optick.h"

#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"

Scene_menu::Scene_menu(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("Scene_menu");
}

// Destructor
Scene_menu::~Scene_menu()
{}

// Called before render is available
bool Scene_menu::Awake(pugi::xml_node config)
{
	LOG("Loading Scene_menu");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene_menu::Start()
{
	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	placeholder = app->tex->Load("Assets/Textures/suscat.jpg");
	//AQUÍ CARGAR TODAS LAS TEXTURAS DEL MENÚ (cuando las tengamos xd)
	//app->guiManager->Disable(); (si se hace la intro en el mismo doc(creo))

	//Get window size
	app->win->GetWindowSize(windowW, windowH);

	//Añadir los controles a una lista 
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "NUEVA PARTIDA", SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH / 2 - 30 - 30,	136,46 }, this));
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "CONTINUAR", SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH / 2 + 40 - 30,	136,46 }, this));
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "AJUSTES", SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH / 2 + 110 - 30,	136,46 }, this));
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "CRÉDITOS", SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH / 2 + 180 - 30,	136,46 }, this));
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "SALIR", SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH / 2 + 250 - 30,	136,46 }, this));


	app->audio->LoadAudioMusic("amogus", 1.0f);
	//app->audio->PlayMusic("PATH DE MUSICA MENU AQUÍ")

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return true;
}

// Called each loop iteration
bool Scene_menu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene_menu::Update(float dt)
{

	OPTICK_EVENT();
	app->render->DrawTexture(placeholder, 0, 0);
	if (showSettings && !_showSettings) {
		SettingsInterface();
	}
	/*if (showSettings) { app->render->DrawTexture(TEXTURA MENÚ SETTINGS, 0, 0); }*/

	if (showCredits) { ShowCredits(); }

	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		app->fadeToBlack->FadeToBlack(this, app->scene_testing, 90);
	}
	return true;
}

// Called each loop iteration
bool Scene_menu::PostUpdate()
{
	
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene_menu::CleanUp()
{
	LOG("Freeing Scene_menu");
	
	ListItem<GuiControl*>* control;
	for (control = controlsScene.start; control != NULL; control = control->next) {
		app->guiManager->DestroyGuiControl(control->data);
	}

	return true;
}

bool Scene_menu::OnGuiMouseClickEvent(GuiControl* control)
{
	return false;
}

void Scene_menu::SettingsInterface()
{
}

void Scene_menu::ShowSettingsInterface()
{
}

void Scene_menu::ShowCredits()
{
}

void Scene_menu::DestroySettingsInterface()
{
}

void Scene_menu::Fullscreen()
{
}

bool Scene_menu::LoadState(pugi::xml_node node)
{
	return false;
}

bool Scene_menu::SaveState(pugi::xml_node node)
{
	return false;
}
