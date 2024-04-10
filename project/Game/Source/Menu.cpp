#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiCheckBox.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "Menu.h"
#include "Scene_menu.h"
#include "Scene_Testing.h"
#include "ModuleFadeToBlack.h"

Menu::Menu(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("Menu");
}

// Destructor
Menu::~Menu()
{}

// Called before render is available
bool Menu::Awake(pugi::xml_node config)
{
	LOG("Menu");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Menu::Start()
{
	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file

	fondoInventario = app->tex->Load("Assets/Textures/Interfaz/Inventario.png");
	fondoEquipo = app->tex->Load("Assets/Textures/Interfaz/Equipo.png");
	fondoDiario = app->tex->Load("Assets/Textures/Interfaz/Diario.png");
	fondoAjustes = app->tex->Load("Assets/Textures/Interfaz/Ajustes.png");

	inventory_audio = app->audio->LoadAudioFx("inventory_fx");
	change_inventory_audio = app->audio->LoadAudioFx("change_inventory_fx");
	

	return true;
}

// Called each loop iteration
bool Menu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Menu::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{

		menuu = !menuu;

		app->audio->PlayFx(inventory_audio);
	}
	if (menuu)
	{
		app->entityManager->active = false;
		app->physics->active = false;
		if (ventana == 0)
		{
			ventana++;
		}
		
		if (app->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
		{
			if (ventana == 4)
			{
				ventana = 1;
			}
			else
			{
				ventana++;
			}

			app->audio->PlayFx(change_inventory_audio);
		}
	}
	else {
		app->entityManager->active = true;
		app->physics->active = true;
		ventana = 0;
	}
	
	

	

	uint windowWidth, windowHeight;
	app->win->GetWindowSize(windowWidth, windowHeight);

	if (ventana == 4)
	{
		contadormenu++;
		if (title != nullptr)
		{
			
			title->state = GuiControlState::NORMAL;
			fullScreen->state = GuiControlState::NORMAL;
			vsync->state = GuiControlState::NORMAL;
			music->state = GuiControlState::NORMAL;
			sfx->state = GuiControlState::NORMAL;
		}


		else
		{

			SDL_Rect MusicPos = { windowWidth / 2 - 400 ,windowHeight / 2 -100, 200, 50 };
			music = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 1, "MUSIC", MusicPos, this, { 0, 0, 20, 20 }, { 0,0,0,0 },0 ,100);

			SDL_Rect SfxPos = { windowWidth / 2 - 400 ,windowHeight / 2 - 50, 200, 50 };
			sfx = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 1, "SFX", SfxPos, this, { 0, 0, 20, 20 }, { 0,0,0,0 }, 0, 100);
	
			SDL_Rect vSyncpos = { windowWidth / 2 -100 ,windowHeight / 2 + 200, 200, 50 };
			vsync = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 1, "VSYNC", vSyncpos, this, { 0, 0, 20, 20 } );

			SDL_Rect FullScreen = { windowWidth / 2 - 100 ,windowHeight / 2 + 50, 230,50 };
			fullScreen = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 1, "FULLSCREEN", FullScreen, this, { 0,0,0,0 }, { -50,0,0,0 });

			SDL_Rect TitlePos = { windowWidth / 2 + 100 ,windowHeight / 2 +  50, 230,50 };
			title = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "VOLVER AL MENÚ", TitlePos, this, { 0,0,0,0 });
		}





	}
	else
	{
		

			if (title != nullptr)
			{
				
				title->state = GuiControlState::DISABLED;
				title = nullptr;
				fullScreen->state = GuiControlState::DISABLED;
				fullScreen = nullptr;
				vsync->state = GuiControlState::DISABLED;
				vsync = nullptr;
				music->state = GuiControlState::DISABLED;
				music = nullptr;
				sfx->state = GuiControlState::DISABLED;
				sfx = nullptr;

				contadormenu = 0;
			}

		
	}
	if (ventana == 4)
	{
		if (music != nullptr)
		{
			app->audio->volumeMusic = ((GuiControlSlider*)music)->value;
		}
		

		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		{
			fullScreen->click = !fullScreen->click;
		}
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			vsync->click = !vsync->click;
		}
		if (app->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
		{
			title->click = true;
		}
	}
	
	if (fullScreen != nullptr && fullScreen->click == true && fullScreenActive == false)
	{
		fullScreenActive = true;
		app->win->ToggleFullscreen();
	}

	if (fullScreen != nullptr && fullScreen->click == false && fullScreenActive == true)
	{
		fullScreenActive = false;
		app->win->ToggleFullscreen();
	}

	if (vsync != nullptr && vsync->click == true)
	{
		vsyncActive = true;
	}

	if (vsync != nullptr && vsync->click == false)
	{
		vsyncActive = false;
	}
	if (title != nullptr && title->click == true)
	{
		/*app->scene_menu->active = true;*/
		menuu = false;
		/*app->guiManager->active = false;*/
		/*app->scene_testing->active = false;*/
		app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_menu);
		title->click = false;
	}


	return true;
}

// Called each loop iteration
bool Menu::PostUpdate()
{
	uint windowWidth, windowHeight;
	app->win->GetWindowSize(windowWidth, windowHeight);
	if (ventana == 1)
	{
		app->render->DrawTexture(fondoInventario, windowWidth / 8, windowHeight / 8, SDL_FLIP_NONE, 0, 0);
	}
	if (ventana == 2)
	{
		app->render->DrawTexture(fondoEquipo, windowWidth / 8, windowHeight / 8, SDL_FLIP_NONE, 0, 0);
	}
	if (ventana == 3)
	{
		app->render->DrawTexture(fondoDiario, windowWidth / 8, windowHeight / 8, SDL_FLIP_NONE, 0, 0);
	}
	if (ventana == 4)
	{
		app->render->DrawTexture(fondoAjustes, windowWidth / 8, windowHeight / 8, SDL_FLIP_NONE, 0, 0);
	}
	
	
	bool ret = true;

	return ret;
}

// Called before quitting
bool Menu::CleanUp()
{
	LOG("Freeing Scene_intro");

	return true;
}

bool Menu::OnGuiMouseClickEvent(GuiControl* control)
{
	return false;
}

void Menu::SettingsInterface()
{
}

void Menu::ShowSettingsInterface()
{
}

void Menu::ShowCredits()
{
}

void Menu::DestroySettingsInterface()
{
}

void Menu::Fullscreen()
{
}

bool Menu::LoadState(pugi::xml_node node)
{
	return false;
}

bool Menu::SaveState(pugi::xml_node node)
{
	return false;
}
