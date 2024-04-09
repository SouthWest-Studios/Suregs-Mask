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
#include "GuiManager.h"
#include "Menu.h"

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
	}
	if (menuu)
	{
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

		}
	}
	else {
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
		}


		else
		{
	
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

				contadormenu = 0;
			}

		
	}
	if (ventana == 4)
	{
		
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		{
			fullScreen->click = !fullScreen->click;
		}
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			
			vsync->click = !vsync->click;
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
