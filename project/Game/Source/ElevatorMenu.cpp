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
#include "Scene_Mazmorra0.h"
#include "Scene_Mazmorra1.h"
#include "Scene_Mazmorra2.h"
#include "Scene_Mazmorra3.h"
#include "Scene_Mazmorra4.h"
#include "Scene_Mazmorra5.h"
#include "Scene_Mazmorra6.h"
#include "Scene_Mazmorra7.h"
#include "Scene_Mazmorra8.h"
#include "Scene_Pueblo.h"
#include "Menu.h"
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
	if (app->ascensor->abierto == true)
	{
		if (final == true)
		{
			plus = 1;
		}
		else
		{
			plus = 0;
		}
		app->entityManager->active = false;
		app->physics->active = false;
		app->menu->active = false;
		OnMovePointer();
		if ((app->input->GetButton(SELECT) == KEY_DOWN && PointerId < mazmorra + plus + 1) /*|| (app->input->GetButton(CONFIRM) == KEY_DOWN && PointerId < mazmorra + plus + 1)*/) {
			UseElevator(PointerId, mazmorra);
		}

		if (app->input->GetButton(APP_EXIT) == KEY_DOWN) {
			
			abierto = false;
		}
	}
	else
	{
		app->menu->active = true;
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

		for (int i = (mazmorra + plus + 1); i < 8 ; i++)
		{
			app->render->DrawTexture(textura_black, PointerPosition.x, -90 + 48*i, SDL_FLIP_NONE, 0, 0);
		}
		
		

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

void ElevatorMenu::UseElevator(int id, int mazmorraa)
{
	switch (id)
	{
	case 0:
	{
		app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra0);
		app->menu->active = true;
		mazmorra = 0;
		break;
	}
	case 1:
	{
		app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra1);
		app->menu->active = true;
		mazmorra = 1;
		break;
	}
	case 2:
	{
		app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra2);
		app->menu->active = true;
		mazmorra = 2;
		break;
	}
	case 3:
	{
		app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra3);
		app->menu->active = true;
		mazmorra = 3;
		break;
	}
	case 4:
	{
		app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra4);
		app->menu->active = true; 
		mazmorra = 4;
		break;
	}
	case 5:
	{
		app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra5);
		app->menu->active = true;
		mazmorra = 5;
		break;
	}
	case 6:
	{
		app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra6);
		app->menu->active = true;
		mazmorra = 6; 
		break;
	}
	case 7:
	{
		app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra7);
		app->menu->active = true;

		mazmorra = 7;
		break;
	}
	case 8:
	{
		app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra8);
		app->menu->active = true;

		mazmorra = 8;
		break;
	}
	default:
		break;
	}

	app->ascensor->abierto = false;
	mazmorraActual = mazmorra;

	if (mazmorra < mazmorraa)
	{
		mazmorra = mazmorraa;
	}

}



