#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene_GameOver.h"
#include "Scene_Menu.h"
#include "Scene_Testing.h"
#include "Scene_Pueblo.h"
#include "ModuleFadeToBlack.h"
#include "Optick/include/optick.h"
#include "Menu.h"
#include "SDL_mixer/include/SDL_mixer.h"

#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "GuiControlSlider.h"
#include "GuiCheckBox.h"


Scene_GameOver::Scene_GameOver(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name = ("scene_gameover");
}

// Destructor
Scene_GameOver::~Scene_GameOver()
{}

// Called before render is available
bool Scene_GameOver::Awake(pugi::xml_node config)
{
	LOG("Loading scene_gameover");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene_GameOver::Start()
{
	app->entityManager->active = false;
	app->physics->active = false;
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	config = configFile.child("config").child(name.GetString());

	gameOverBackground_tp= config.child("gameoverbackground").attribute("texturepath").as_string();
	logo_tp = config.child("logo").attribute("texturepath").as_string();

	gameOverBackground = app->tex->Load(gameOverBackground_tp);
	logo = app->tex->Load(logo_tp);

	//AQU?CARGAR TODAS LAS TEXTURAS DEL MEN?(cuando las tengamos xd)
	

	//Get window size
	app->win->GetWindowSize(windowW, windowH);

	
	VolverAlMenu = (GuiControlButton*)(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 200, "VOLVER AL MENÚ", SDL_Rect{ 537, 360,	136,25 }, this));
	Continuar = (GuiControlButton*)(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 201, "CONTINUAR", SDL_Rect{ 537, 460,	136,25 }, this));

	app->guiManager->minId = 200;
	app->guiManager->maxId = 201;
	app->guiManager->pointerId = 201;
	app->guiManager->columnSize = 0;

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return true;
}

// Called each loop iteration
bool Scene_GameOver::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene_GameOver::Update(float dt)
{
	app->render->DrawTexture(gameOverBackground, 0, 0, 1);
	app->render->DrawTexture(logo, 200, 200, 1);
	
	return true;
}

// Called each loop iteration
bool Scene_GameOver::PostUpdate()
{
	
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene_GameOver::CleanUp()
{
	LOG("Freeing Scene_GameOver");

	app->tex->UnLoad(logo);
	app->tex->UnLoad(gameOverBackground);

	return true;
}

bool Scene_GameOver::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Press Gui Control: %d", control->id);

	switch (control->id)
	{

	case 200:
		app->fadeToBlack->FadeToBlack(this, app->scene_menu, 90);
		app->menu->active = true;
		app->guiManager->pointerId = 1;
		app->guiManager->DestroyGuiControl(VolverAlMenu);
		app->guiManager->DestroyGuiControl(Continuar);

		break;

	case 201:
		app->fadeToBlack->FadeToBlack(this, app->scene_pueblo, 90);
		app->menu->active = true;
		app->guiManager->pointerId = 100;
		app->guiManager->DestroyGuiControl(VolverAlMenu);
		app->guiManager->DestroyGuiControl(Continuar);
		break;

	default:

		break;
	}

	return true;
}

void Scene_GameOver::Fullscreen()
{
}

bool Scene_GameOver::LoadState(pugi::xml_node node)
{
	return true;
}

bool Scene_GameOver::SaveState(pugi::xml_node node)
{
	return true;
}
