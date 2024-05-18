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
#include "InventoryManager.h"
#include "NotesManager.h"
#include "BestiarioManager.h"
#include "ElevatorMenu.h"
#include "Scene_Testing.h"
#include "ModuleFadeToBlack.h"
#include "Elevator.h"

#include "SDL_mixer/include/SDL_mixer.h"

ElevatorMenu::ElevatorMenu(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name = ("Ascensormenu");
}

// Destructor
ElevatorMenu::~ElevatorMenu()
{}

// Called before render is available
bool ElevatorMenu::Awake(pugi::xml_node config)
{
	LOG("menu");
	bool ret = true;

	

	return ret;
}

// Called before the first frame
bool ElevatorMenu::Start()
{

	
	

	return true;
}

// Called each loop iteration
bool ElevatorMenu::PreUpdate()
{
	
	return true;
}

// Called each loop iteration
bool ElevatorMenu::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		app->ascensor->abierto = !app->ascensor->abierto;
		
	}
	if (app->ascensor->abierto == true)
	{
		app->entityManager->active = false;
		app->physics->active = false;
		OnMovePointer();

		if (app->input->GetButton(APP_EXIT) == KEY_DOWN) {
			
			abierto = false;
		}
	}
	
	return true;
}

// Called each loop iteration
bool ElevatorMenu::PostUpdate()
{
		
	if (abierto == true)
	{
		app->render->DrawTexture(Menutexture, 400, 100, SDL_FLIP_NONE, 0, 0);

		app->render->DrawTexture(PointerTexture, PointerPosition.x, PointerPosition.y, SDL_FLIP_NONE, 0, 0);

	}

	bool ret = true;

	return ret;
}

// Called before quitting
bool ElevatorMenu::CleanUp()
{
	
	LOG("Freeing Scene_Intro");

	return true;
}

void ElevatorMenu::OnMovePointer()
{
	if (app->input->GetButton(DOWN) == KEY_DOWN) {
		if (PointerId + 1 < 8)
		{
			PointerPosition.y += 48;
			PointerId += 1;

		}
		else
		{
			PointerId = 0;
			PointerPosition.y = -90;
		}



	}
	if (app->input->GetButton(UP) == KEY_DOWN) {
		if (PointerId > 0)
		{
			PointerPosition.y -= 48;
			PointerId -= 1;

		}
		else
		{
			PointerId = 7;
			PointerPosition.y = -90 + 48 * (7);
		}


	}
}



