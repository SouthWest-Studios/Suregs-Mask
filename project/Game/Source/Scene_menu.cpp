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
#include "Menu.h"

#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "GuiControlSlider.h"
#include "GuiCheckBox.h"


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
	placeholderMenu = app->tex->Load("Assets/Textures/suscat.jpg");
	placeholderSettings = app->tex->Load("Assets/Textures/suscat2.jpg");
	placeholderCredits = app->tex->Load("Assets/Textures/suscat3.jpg");

	//AQUÍ CARGAR TODAS LAS TEXTURAS DEL MENÚ (cuando las tengamos xd)
	

	//Get window size
	app->win->GetWindowSize(windowW, windowH);

	//Añadir los controles a una lista 
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "NUEVA PARTIDA", SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH / 2 - 30 - 30,	136,46 }, this));
	controlsScene.end->data->selected = true;
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "CONTINUAR", SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH / 2 + 40 - 30,	136,46 }, this));
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "AJUSTES", SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH / 2 + 110 - 30,	136,46 }, this));
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "CRÉDITOS", SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH / 2 + 180 - 30,	136,46 }, this));
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "SALIR", SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH / 2 + 250 - 30,	136,46 }, this));


	app->audio->LoadAudioMusic("menu", 1.0f);
	//app->audio->PlayMusic("PATH DE MUSICA MENU AQUÍ")

	app->guiManager->minId = 1;
	app->guiManager->maxId = 5;
	app->guiManager->columnSize = 0;

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
	app->render->DrawTexture(placeholderMenu, 0, 0);
	if (showSettings && !_showSettings) {
		SettingsInterface();
	}
	if (showSettings) { app->render->DrawTexture(placeholderSettings, 0, 0); }

	if (showCredits) { ShowCredits(); }

	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		app->fadeToBlack->FadeToBlack(this, app->scene_testing, 90);
		app->menu->active = true;
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
	LOG("Press Gui Control: %d", control->id);

	switch (control->id)
	{

	case 1:
		app->fadeToBlack->FadeToBlack(this, app->scene_testing, 90);
		app->menu->active = true;
		break;

	case 2:
		app->fadeToBlack->FadeToBlack(this, app->scene_testing, 90);
		app->menu->active = true;
		break;

	case 3:
		showSettings = true;
		app->guiManager->minId = 6;
		app->guiManager->maxId = 11;
		break;

	case 4:
		showCredits = true;
		app->guiManager->minId = 12;
		app->guiManager->maxId = 12;
		break;

	case 5:
		app->closeApplication = true; 
		break;

	case 11:
		showSettings = false;
		_showSettings = false;
		DestroySettingsInterface();
		app->guiManager->minId = 1;
		app->guiManager->maxId = 5;
		break;

	case 12:
		showCredits = false;
		_showCredits = false;
		ListItem<GuiControl*>* controlA;
		for (controlA = controlsScene.start; controlA != NULL; controlA = controlA->next)
		{
			controlA->data->state = GuiControlState::NORMAL;
		}
		app->guiManager->DestroyGuiControl(gcCloseCredits);
		app->guiManager->minId = 1;
		app->guiManager->maxId = 5;
		break;

	case 1011:

		/*app->audio->musicVolumne = ((GuiControlSlider*)control)->value;*/

		break;

	case 1021:
		/*app->audio->sfvVolumne = ((GuiControlSlider*)control)->value;*/
		break;

	case 1031:
		Fullscreen();
		break;

	case 1041:
		/*if (app->render->vsync)
		{

			((GuiCheckBox*)(controlsSettings.At(controlsSettings.Count() - 1)->data))->click = false;
			
		}*/

	default:
		break;
	}

	return true;
}

void Scene_menu::SettingsInterface()
{
	app->render->DrawTexture(placeholderSettings, 0, 0);

	ListItem<GuiControl*>* control;
	for (control = controlsScene.start; control != NULL; control = control->next)
	{
		control->data->state = GuiControlState::DISABLED;
	}

	//SETTINGS

	controlsSettings.Add(app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 7, "", SDL_Rect{ (int)windowW / 2 + 60,	(int)windowH / 2 - 10,	120,20 }, this));
	/*((GuiControlSlider*)(controlsSettings.At(controlsSettings.Count() - 1)->data))->value = app->audio->musicVolumne;*/
	controlsSettings.Add(app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 8, "", SDL_Rect{ (int)windowW / 2 + 60,	(int)windowH / 2 + 50,	120,20 }, this));
	/*((GuiControlSlider*)(controlsSettings.At(controlsSettings.Count() - 1)->data))->value = app->audio->sfvVolumne;*/
	controlsSettings.Add(app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 9, "", SDL_Rect{ (int)windowW / 2 - 110,	(int)windowH / 2 + 180,	20,20 }, this));

	if (app->fullscreen)
	{
		((GuiCheckBox*)(controlsSettings.At(controlsSettings.Count() - 1)->data))->click = true;
	}
	else
	{
		((GuiCheckBox*)(controlsSettings.At(controlsSettings.Count() - 1)->data))->click = false;
	}

	controlsSettings.Add(app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 10, "", SDL_Rect{ (int)windowW / 2 + 80,	(int)windowH / 2 + 180,	20,20 }, this));

	if (app->render->vsync)
	{
		((GuiCheckBox*)(controlsSettings.At(controlsSettings.Count() - 1)->data))->click = true;
	}
	else
	{
		((GuiCheckBox*)(controlsSettings.At(controlsSettings.Count() - 1)->data))->click = false;
	}

	controlsSettings.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "ATRÁS", SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH - 150,	136,46 }, this));

	_showSettings = true;
}

void Scene_menu::ShowCredits()
{
	if (showCredits && !_showCredits) {
		ListItem<GuiControl*>* control;
		for (control = controlsScene.start; control != NULL; control = control->next)
		{
			control->data->state = GuiControlState::DISABLED; 
		}

		gcCloseCredits = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 12, "ATRÁS", SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH - 150,	136,46 }, this);
		_showCredits = true;
	}

	app->render->DrawTexture(placeholderCredits, 0, 0);
}

void Scene_menu::DestroySettingsInterface()
{
	ListItem<GuiControl*>* control;
	for (control = controlsSettings.start; control != NULL; control = control->next)
	{
		app->guiManager->DestroyGuiControl(control->data);
	}
	controlsSettings.Clear();



	for (control = controlsScene.start; control != NULL; control = control->next)
	{
		control->data->state = GuiControlState::NORMAL;
	}
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
