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
#include "QuestManager.h"
#include "DialogManager.h"
#include "Scene_Pueblo_Tutorial.h"
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
	button_fx = app->audio->LoadAudioFx("button_fx");
	select_fx = app->audio->LoadAudioFx("select_fx");

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
		if (mazmorra > mazmorraActual)
		{
			final = false;
		}
		if (final == true)
		{
			plus = 1;
		}
		else
		{
			plus = 0;
		}
		/*app->entityManager->active = false;*/
		app->physics->active = false;
		app->menu->active = false;
		OnMovePointer();
		if (open)
		{
			if ((app->input->GetButton(SELECT) == KEY_DOWN && !app->scene_pueblo->GetRod()->fishing.rodReady  && !app->scene_pueblo_tutorial->GetRod()->fishing.rodReady  && PointerId < mazmorra + plus + 1) || (app->input->GetButton(CONFIRM) == KEY_DOWN && PointerId < mazmorra + plus + 1) && !app->scene_pueblo->GetRod()->fishing.rodReady  && !app->scene_pueblo_tutorial->GetRod()->fishing.rodReady) {
				app->audio->PlayFx(select_fx);
				int num;
				if (PointerId != mazmorraActual)
				{
					UseElevator(PointerId, mazmorra);
				}
				else
				{

				}
				
			}
		}

		
		open = true;
		if (app->input->GetButton(APP_EXIT) == KEY_DOWN || app->input->GetButton(BACK) == KEY_DOWN) {
			
			abierto = false;
			open = false;
			cerrar = true;
		}
		
	}
	if(cerrar)
	{
		app->menu->active = true;
		app->entityManager->active = true;
		app->physics->active = true;
		open = false;
		cerrar = false;
	}
	return true;
}

// Called each loop iteration
bool ElevatorMenu::PostUpdate()
{
		
	if (abierto == true)
	{
		uint windowWidth, windowHeight;
		app->win->GetWindowSize(windowWidth, windowHeight);
		Uint8 alpha = 188;  // Valor de transparencia (0-255)
		SDL_Texture* transparentTexture = app->menu->CreateTransparentTexture(app->render->renderer, windowWidth, windowHeight, alpha);
		if (transparentTexture != nullptr) {
			SDL_SetTextureBlendMode(transparentTexture, SDL_BLENDMODE_BLEND);
			SDL_RenderCopy(app->render->renderer, transparentTexture, nullptr, nullptr);
			SDL_DestroyTexture(transparentTexture);
		}

		app->render->DrawTexture(Menutexture, 399, 100, SDL_FLIP_NONE, 0, 0);

		

		for (int i = (mazmorra + plus + 1); i < 8 ; i++)
		{
			if (i % 2 == 0)
			{
				app->render->DrawTexture(textura_black, 399, 99 + 45 * i, SDL_FLIP_NONE, 0, 0);
			}
			else
			{
				app->render->DrawTexture(textura_black, 399 + 99, 99 + 45 * (i -1), SDL_FLIP_NONE, 0, 0);
			}
			
		}
		app->render->DrawTexture(PointerTexture, PointerPosition.x, PointerPosition.y, SDL_FLIP_NONE, 0, 0);
		

		totalMazmorras = mazmorra + plus + 1;
	}


	bool ret = true;

	return ret;
}

// Called before quitting
bool ElevatorMenu::CleanUp()
{
	app->tex->UnLoad(textura_black);
	app->tex->UnLoad(PointerTexture);
	app->tex->UnLoad(Menutexture);
	LOG("Freezing ElevatorMenu");

	return true;
}

void ElevatorMenu::OnMovePointer()
{
	if (app->input->GetButton(DOWN) == KEY_DOWN) {
		if (PointerId == -20)
		{
			PointerId = 0;
			PointerPosition.y = 99;
			PointerPosition.x = 399;
			verticalPointerId = 0;
			horitzontalPointerId = 0;

			app->audio->PlayFx(button_fx);
		}
		else if (verticalPointerId + 1 == 4)
		{
			PointerId = -20;
			PointerPosition.y = 19;
			PointerPosition.x = 447;

			app->audio->PlayFx(button_fx);
		}
		else if (verticalPointerId + 1 < 4)
		{
			PointerPosition.y += 90;
			PointerId += 2;
			verticalPointerId += 1;

			app->audio->PlayFx(button_fx);
		}
		
		



	}
	if (app->input->GetButton(UP) == KEY_DOWN) {
		if (PointerId == -20)
		{

			horitzontalPointerId = 0;
			PointerId = 6;
			PointerPosition.y = 99 + 90 * (3);
			PointerPosition.x = 399;
			verticalPointerId = 3;

			app->audio->PlayFx(button_fx);
		}
		else if (verticalPointerId == 0)
		{
			PointerId = -20;
			PointerPosition.y = 19;
			PointerPosition.x = 447;

			app->audio->PlayFx(button_fx);
		}
		else if (verticalPointerId > 0)
		{
			PointerPosition.y -= 90;
			PointerId -= 2;
			verticalPointerId -= 1;

			app->audio->PlayFx(button_fx);
		}


	}
	if (app->input->GetButton(RIGHT) == KEY_DOWN && PointerId != -20) {
		if (horitzontalPointerId + 1 > 1)
		{
			horitzontalPointerId = 0;
			PointerPosition.x = 399;
			PointerId -= 1;

			app->audio->PlayFx(button_fx);
		}
		else
		{
			if (PointerId != -1)
			{
				horitzontalPointerId += 1;
				PointerPosition.x += 98;
			}
			PointerId += 1;

			app->audio->PlayFx(button_fx);
		}

	}
	if (app->input->GetButton(LEFT) == KEY_DOWN && PointerId != -20) {
		if (horitzontalPointerId - 1 < 0)
		{

				PointerId += 1;
				horitzontalPointerId = 1;
				PointerPosition.x = 499;

				app->audio->PlayFx(button_fx);
		}
		else
		{
			horitzontalPointerId -= 1;
			PointerPosition.x -= 98;
			PointerId -= 1;

			app->audio->PlayFx(button_fx);
		}
	}
}

void ElevatorMenu::UseElevator(int id, int mazmorraa)
{
	switch (id)
	{
	case -20:
	{

		app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_pueblo);
		app->menu->active = true;
		mazmorra = 0;
		mazmorraActual = 0;
		break;
	}
	case 0:
	{
		
		app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra0);
		app->menu->active = true;
		mazmorra = 0;
		mazmorraActual = 0;
		break;
	}
	case 1:
	{
		if (app->questManager->GetQuestLineIndex(1) >= 9)
		{
			app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra1);
			app->menu->active = true;
			mazmorra = 1;
			mazmorraActual = 1;
			if(app->inventoryManager->storyLevel < 4)
			{
				app->inventoryManager->storyLevel = 4;
			}
		}
		else
		{
			(app->dialogManager->CreateDialogSinEntity("Tengo cosas pendientes...", "Jakov", "Assets/Textures/Interfaz/Dialogos/jakovFace.png"));
		}
		break;
	}
	case 2:
	{
		if (app->questManager->GetQuestLineIndex(1) >= 12)
		{
			app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra2);
			app->menu->active = true;
			mazmorra = 2;
			mazmorraActual = 2;
			if(app->inventoryManager->storyLevel < 5)
			{
				app->inventoryManager->storyLevel = 5;
			}
		}
		else
		{
			(app->dialogManager->CreateDialogSinEntity("Tengo cosas pendientes...", "Jakov", "Assets/Textures/Interfaz/Dialogos/jakovFace.png"));
		}

		break;
	}
	case 3:
	{
		if (app->questManager->GetQuestLineIndex(1) >= 14)
		{
			app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra3);
			app->menu->active = true;
			mazmorra = 3;
			mazmorraActual = 3;
			if(app->inventoryManager->storyLevel < 6)
			{
				app->inventoryManager->storyLevel = 6;
			}
		}
		else
		{
			(app->dialogManager->CreateDialogSinEntity("Tengo cosas pendientes...", "Jakov", "Assets/Textures/Interfaz/Dialogos/jakovFace.png"));
		}
		break;
	}
	case 4:
	{
		if (app->questManager->GetQuestLineIndex(1) >= 17)
		{
			app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra4);
			app->menu->active = true;
			mazmorra = 4;
			mazmorraActual = 4;
			if(app->inventoryManager->storyLevel < 7)
			{
				app->inventoryManager->storyLevel = 7;
			}
		}
		else
		{
			(app->dialogManager->CreateDialogSinEntity("Tengo cosas pendientes...", "Jakov", "Assets/Textures/Interfaz/Dialogos/jakovFace.png"));
		}
		break;
	}
	case 5:
	{
		if (app->questManager->GetQuestLineIndex(1) >= 19)
		{
			app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra5);
			app->menu->active = true;
			mazmorra = 5;
			mazmorraActual = 5;
			if(app->inventoryManager->storyLevel < 8)
			{
				app->inventoryManager->storyLevel = 8;
			}
		}
		else
		{
			(app->dialogManager->CreateDialogSinEntity("Tengo cosas pendientes...", "Jakov", "Assets/Textures/Interfaz/Dialogos/jakovFace.png"));
		}
		break;
	}
	case 6:
	{
		if (app->questManager->GetQuestLineIndex(1) >= 21)
		{
			app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra6);
			app->menu->active = true;
			mazmorra = 6;
			mazmorraActual = 6;
			if(app->inventoryManager->storyLevel < 9)
			{
				app->inventoryManager->storyLevel = 9;
			}
		}
		else
		{
			(app->dialogManager->CreateDialogSinEntity("Tengo cosas pendientes...", "Jakov", "Assets/Textures/Interfaz/Dialogos/jakovFace.png"));
		}
		break;
	}
	case 7:
	{
		if (app->questManager->GetQuestLineIndex(1) >= 24)
		{
			app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra7);
			app->menu->active = true;

			mazmorra = 7;
			mazmorraActual = 7;
			if(app->inventoryManager->storyLevel < 10)
			{
				app->inventoryManager->storyLevel = 10;
			}
		}
		else
		{
			(app->dialogManager->CreateDialogSinEntity("Tengo cosas pendientes...", "Jakov", "Assets/Textures/Interfaz/Dialogos/jakovFace.png"));
		}
		break;
	}
	case 8:
	{
		if (app->questManager->GetQuestLineIndex(1) >= 24)
		{
			app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra8);
			app->menu->active = true;

			mazmorra = 8;
			mazmorraActual = 8;
			if(app->inventoryManager->storyLevel < 11)
			{
				app->inventoryManager->storyLevel = 11;
			}
		}
		else
		{
			(app->dialogManager->CreateDialogSinEntity("Tengo cosas pendientes...", "Jakov", "Assets/Textures/Interfaz/Dialogos/jakovFace.png"));
		}
		break;
	}
	default:
		break;
	}

	app->ascensor->abierto = false;
	open = false;
	cerrar = true;
	

	if (mazmorra < mazmorraa)
	{
		if (final)
		{
			mazmorra = mazmorraa + 1;
		}
		else
		{
			mazmorra = mazmorraa;
		}
		
	}

}



