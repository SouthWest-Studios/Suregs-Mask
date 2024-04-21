#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
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


Scene_Menu::Scene_Menu(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("scene_menu");
}

// Destructor
Scene_Menu::~Scene_Menu()
{}

// Called before render is available
bool Scene_Menu::Awake(pugi::xml_node config)
{
	LOG("Loading scene_menu");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene_Menu::Start()
{
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	config = configFile.child("config").child(name.GetString());

	menuMain_tp= config.child("menuMain").attribute("texturepath").as_string(); 
	menuMain2_tp = config.child("menuMain2").attribute("texturepath").as_string();
	settings_tp = config.child("settings").attribute("texturepath").as_string();
	credits_tp = config.child("credits").attribute("texturepath").as_string();
	logo_tp = config.child("logo").attribute("texturepath").as_string();

	menuMain = app->tex->Load(menuMain_tp);
	menuMain2 = app->tex->Load(menuMain2_tp);
	settings = app->tex->Load(settings_tp);
	credits = app->tex->Load(credits_tp);
	logo = app->tex->Load(logo_tp);

	//AQU?CARGAR TODAS LAS TEXTURAS DEL MEN?(cuando las tengamos xd)
	

	//Get window size
	app->win->GetWindowSize(windowW, windowH);

	//Añadir los controles a una lista 
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "NUEVA PARTIDA", SDL_Rect{ 75, 403,	136,46 }, this));
	controlsScene.end->data->selected = true;
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "CONTINUAR", SDL_Rect{ 75,457,136,46 }, this));
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "AJUSTES", SDL_Rect{ 75, 512,136,46 }, this));
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "CRÉDITOS", SDL_Rect{75, 565,136,46 }, this));
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "SALIR", SDL_Rect{75, 622,136,46 }, this));


	app->audio->LoadAudioMusic("menu", 10.0f);
	menu_fx = app->audio->LoadAudioFx("menu_fx");
	app->audio->PlayFx(menu_fx);

	app->guiManager->minId = 1;
	app->guiManager->maxId = 5;
	app->guiManager->columnSize = 0;

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return true;
}

// Called each loop iteration
bool Scene_Menu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene_Menu::Update(float dt)
{

	OPTICK_EVENT();
	app->render->DrawTexture(menuMain, 0, 0);
	app->render->DrawTexture(menuMain2, 550, 0);
	app->render->DrawTexture(logo, 130, 100);
	/*if (showSettings && !_showSettings) {
		SettingsInterface();
	}*/
	if (showSettings) { app->render->DrawTexture(settings, 0, 0); }

	if (showCredits) { ShowCredits(); }

	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		app->fadeToBlack->FadeToBlack(this, app->scene_testing, 90);
		app->menu->active = true;
	}
	if (fullscreen != nullptr && fullscreen->click) {
		Fullscreen();
	}

	if (ajustes == false && showCredits == false)
	{

		ListItem<GuiControl*>* control;
		for (control = controlsScene.start; control != NULL; control = control->next)
		{
			control->data->state = GuiControlState::NORMAL;
		}
	}
	

	if (ajustes)
	{
		SettingsInterface();
	}

	if (showSettings == false)
	{
		ajustes = false;

	}
	if (showSettings == true)
	{
		ajustes = true;
	}
	return true;
}

// Called each loop iteration
bool Scene_Menu::PostUpdate()
{
	
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene_Menu::CleanUp()
{
	LOG("Freeing Scene_Menu");
	
	ListItem<GuiControl*>* control;
	for (control = controlsScene.start; control != NULL; control = control->next) {
		app->guiManager->DestroyGuiControl(control->data);
	}

	app->tex->UnLoad(menuMain);
	app->tex->UnLoad(menuMain2);
	app->tex->UnLoad(settings);
	app->tex->UnLoad(credits);

	return true;
}

bool Scene_Menu::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Press Gui Control: %d", control->id);

	switch (control->id)
	{

	case 1:
		app->fadeToBlack->FadeToBlack(this, app->scene_pueblo, 90);
		app->menu->active = true;
		break;

	case 2:
		app->fadeToBlack->FadeToBlack(this, app->scene_pueblo, 90);
		app->LoadRequest();
		app->menu->active = true;
		break;

	case 3:
		showSettings = true;
		control->selected = false;
		app->guiManager->minId = 7;
		app->guiManager->maxId = 11;
		app->guiManager->pointerId = 7;
		break;

	case 4:
		showCredits = true;
		control->selected = false;
		app->guiManager->minId = 12;
		app->guiManager->maxId = 12;
		app->guiManager->pointerId = 12;
		break;

	case 5:
		app->closeApplication = true; 
		break;

	case 11:
		showSettings = false;
		_showSettings = false;
		/*DestroySettingsInterface();*/
		app->guiManager->minId = 1;
		app->guiManager->maxId = 5;
		app->guiManager->pointerId = 1;
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
		app->guiManager->pointerId = 1;
		break;

	case 1011:

		/*app->audio->musicVolumne = ((GuiControlSlider*)control)->value;*/

		break;

	case 1021:
		/*app->audio->sfvVolumne = ((GuiControlSlider*)control)->value;*/
		break;

	case 9:
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

void Scene_Menu::SettingsInterface()
{
	uint windowWidth, windowHeight;
	app->win->GetWindowSize(windowWidth, windowHeight);

	app->render->DrawTexture(settings, 0, 0);

	ListItem<GuiControl*>* control;
	for (control = controlsScene.start; control != NULL; control = control->next)
	{
		control->data->state = GuiControlState::DISABLED;
	}

	//SETTINGS
	if (showSettings)
	{
		
		if (music != nullptr)
		{

			fullscreen->state = GuiControlState::NORMAL;
			vsync->state = GuiControlState::NORMAL;
			music->state = GuiControlState::FOCUSED;
			sfx->state = GuiControlState::NORMAL;
		}

		else
		{

			SDL_Rect MusicPos = { windowWidth / 2 - 100 ,windowHeight / 2 - 200, 200, 50 };
			music = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 7, "MUSIC", MusicPos, this, { 0, 0, 20, 20 }, { 0,0,0,0 }, 0, 100);

			SDL_Rect SfxPos = { windowWidth / 2 - 100 ,windowHeight / 2 - 150, 200, 50 };
			sfx = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 8, "SFX", SfxPos, this, { 0, 0, 20, 20 }, { 0,0,0,0 }, 0, 100);

			SDL_Rect FullScreen = { windowWidth / 2 + 100 ,windowHeight / 2 - 50, 230,50 };
			fullscreen = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 9, "FULLSCREEN", FullScreen, this, { 0,0,0,0 }, { -50,0,0,0 });

			SDL_Rect vSyncpos = { windowWidth / 2 + 100 ,windowHeight / 2 + 100, 200, 50 };
			vsync = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 10, "VSYNC", vSyncpos, this, { 0, 0, 20, 20 });

			SDL_Rect TitlePos = { 550, 600,	136,46 };
			atras = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "ATRAS", TitlePos, this, { 0,0,0,0 });

			/*controlsSettings.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "ATRÁS", SDL_Rect{ 550, 600,	136,46 }, this));*/

			if (app->win->fullscreen)
			{
				fullscreen->click = true;
			}
			if (vsyncActive)
			{
				vsync->click = true;
			}
		}

		
	}
	else
	{
		

		if (music != nullptr)
		{
			
			/*title->state = GuiControlState::DISABLED;
			title = nullptr;*/
			app->guiManager->DestroyGuiControl(fullscreen);
			/*fullScreen->state = GuiControlState::DISABLED;*/
			
			app->guiManager->DestroyGuiControl(vsync);
			/*vsync->state = GuiControlState::DISABLED;
			vsync = nullptr;*/
			app->guiManager->DestroyGuiControl(music);
			/*music->state = GuiControlState::DISABLED;*/
			music = nullptr;
			app->guiManager->DestroyGuiControl(sfx);
			/*sfx->state = GuiControlState::DISABLED;
			sfx = nullptr;*/
			app->guiManager->DestroyGuiControl(atras);

			
		}


	}
	if (atras->click == true)
	{
		if (music != nullptr)
		{

			/*title->state = GuiControlState::DISABLED;
			title = nullptr;*/
			app->guiManager->DestroyGuiControl(fullscreen);
			/*fullScreen->state = GuiControlState::DISABLED;*/

			app->guiManager->DestroyGuiControl(vsync);
			/*vsync->state = GuiControlState::DISABLED;
			vsync = nullptr;*/
			app->guiManager->DestroyGuiControl(music);
			/*music->state = GuiControlState::DISABLED;*/
			music = nullptr;
			app->guiManager->DestroyGuiControl(sfx);
			/*sfx->state = GuiControlState::DISABLED;
			sfx = nullptr;*/


		}
	}
	if (showSettings)
	{
		if (music != nullptr)
		{
			int newVolume = ((GuiControlSlider*)music)->value;
			// Asegurarse de que el nuevo volumen esté dentro de los límites válidos (0-128 para SDL Mixer)
			newVolume = std::max(0, std::min(128, newVolume));
			// Establecer el volumen de la música
			Mix_VolumeMusic(newVolume);
		}


		if (fullscreen->selected && app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			fullscreen->click = !fullscreen->click;
		}
		if (vsync->selected && app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			vsync->click = !vsync->click;
		}
		
		if (fullscreen != nullptr && fullscreen->click == true && fullScreenActive == false)
		{
			fullScreenActive = true;
			app->win->ToggleFullscreen();
		}

		if (fullscreen != nullptr && fullscreen->click == false && fullScreenActive == true)
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
	}
	//controlsSettings.Add(app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 7, "MUSIC", SDL_Rect{ 550,100,120,20 }, this));
	///*((GuiControlSlider*)(controlsSettings.At(controlsSettings.Count() - 1)->data))->value = app->audio->musicVolume;*/
	//controlsSettings.Add(app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 8, "SFX", SDL_Rect{ 550, 200, 120,20 }, this));
	///*((GuiControlSlider*)(controlsSettings.At(controlsSettings.Count() - 1)->data))->value = app->audio->sfvVolume;*/
	//fullscreen = (GuiCheckBox*)controlsSettings.Add(app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 9, "FULLSCREEN", SDL_Rect{ 550, 300, 20,20 }, this));


	/*if (app->fullscreen)
	{
		((GuiCheckBox*)(controlsSettings.At(controlsSettings.Count() - 1)->data))->click = true;
	}
	else
	{
		((GuiCheckBox*)(controlsSettings.At(controlsSettings.Count() - 1)->data))->click = false;
	}*/

	/*controlsSettings.Add(app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 10, "VSYNC", SDL_Rect{ 650, 300,	20,20 }, this));

	if (app->render->vsync)
	{
		((GuiCheckBox*)(controlsSettings.At(controlsSettings.Count() - 1)->data))->click = true;
	}
	else
	{
		((GuiCheckBox*)(controlsSettings.At(controlsSettings.Count() - 1)->data))->click = false;
	}*/

	/*if (fullscreen->selected && app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN && fullscreen != nullptr) {
		fullscreen->click = !fullscreen->click;
	}*/
	

	_showSettings = true;
}

void Scene_Menu::ShowCredits()
{
	if (showCredits && !_showCredits) {
		ListItem<GuiControl*>* control;
		for (control = controlsScene.start; control != NULL; control = control->next)
		{
			control->data->state = GuiControlState::DISABLED; 
		}

		gcCloseCredits = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 12, "ATRÁS", SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH - 100,	136,46 }, this);
		_showCredits = true;
	}

	app->render->DrawTexture(credits, 0, 0);
}

void Scene_Menu::DestroySettingsInterface()
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

void Scene_Menu::Fullscreen()
{
}

bool Scene_Menu::LoadState(pugi::xml_node node)
{
	return true;
}

bool Scene_Menu::SaveState(pugi::xml_node node)
{
	return true;
}
