#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene_Menu.h"
#include "Scene_Testing.h"
#include "Scene_Pueblo.h"
#include "Scene_Pueblo_Tutorial.h"
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
	name = ("scene_menu");
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
	app->entityManager->active = false;
	app->physics->active = false;
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	config = configFile.child("config").child(name.GetString());

	menuMain_tp = config.child("menuMain").attribute("texturepath").as_string();
	menuMain2_tp = config.child("menuMain2").attribute("texturepath").as_string();
	settings_tp = config.child("settings").attribute("texturepath").as_string();
	credits_tp = config.child("credits").attribute("texturepath").as_string();
	logo_tp = config.child("logo").attribute("texturepath").as_string();
	savedGames_tp = config.child("savedGames").attribute("texturepath").as_string();
	controls_tp = config.child("controls").attribute("texturepath").as_string();
	coin_tp = config.child("coin").attribute("texturepath").as_string();
	clock_tp = config.child("clock").attribute("texturepath").as_string();

	menuMain = app->tex->Load(menuMain_tp);
	menuMain2 = app->tex->Load(menuMain2_tp);
	settings = app->tex->Load(settings_tp);
	credits = app->tex->Load(credits_tp);
	logo = app->tex->Load(logo_tp);
	savedGames = app->tex->Load(savedGames_tp);
	controls = app->tex->Load(controls_tp);
	coin = app->tex->Load(coin_tp);
	clock = app->tex->Load(clock_tp);

	pugi::xml_document saveOneFile;
	pugi::xml_node game_stateOne;
	pugi::xml_parse_result parseResultSaveOne = saveOneFile.load_file("save_game.xml");
	game_stateOne = saveOneFile.child("game_state");

	coinQuantityOne = game_stateOne.child("iventorymanager").child("inventory").child("money").attribute("quantity").as_int();

	pugi::xml_document saveTwoFile;
	pugi::xml_node game_stateTwo;
	pugi::xml_parse_result parseResultSaveTwo = saveTwoFile.load_file("save_game2.xml");
	game_stateTwo = saveTwoFile.child("game_state");

	coinQuantityTwo = game_stateTwo.child("iventorymanager").child("inventory").child("money").attribute("quantity").as_int();

	pugi::xml_document saveThreeFile;
	pugi::xml_node game_stateThree;
	pugi::xml_parse_result parseResultSaveThree = saveThreeFile.load_file("save_game3.xml");
	game_stateThree = saveThreeFile.child("game_state");

	coinQuantityThree = game_stateThree.child("iventorymanager").child("inventory").child("money").attribute("quantity").as_int();

	//Get window size
	app->win->GetWindowSize(windowW, windowH);

	//A�adir los controles a una lista 
	/*SDL_Rect TitlePos = { 75, 403,	136,46 };
	NuevaPartida = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "NUEVA PARTIDA", TitlePos, this, { 0,0,0,0 });*/
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "NUEVA PARTIDA", SDL_Rect{ 537, 360,	136,25 }, this));
	///*controlsScene.end->data->selected = true;*/
	///*SDL_Rect TitlePos2 = { 75,457,136,46 };
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "CONTINUAR", SDL_Rect{ 549,390,100,25 }, this));
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "AJUSTES", SDL_Rect{ 557, 420,80,25 }, this));
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "CONTROLES", SDL_Rect{ 549, 450,100,25 }, this));
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "CRÉDITOS", SDL_Rect{ 554, 480,90,25 }, this));
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, "SALIR", SDL_Rect{ 564, 510,60,25 }, this));

	/*fPoint ePos(500.0f, 500.0f);*/
	/*eMenu = app->psystem->AddEmiter(ePos, EmitterType::EMITTER_TYPE_SPARK);*/

	app->audio->LoadAudioMusic("menu", 10.0f);
	menu_fx = app->audio->LoadAudioFx("menu_fx");
	app->audio->PlayFx(menu_fx);

	app->guiManager->minId = 1;
	app->guiManager->maxId = 6;
	app->guiManager->pointerId = 1;
	app->guiManager->columnSize = 0;

	fPoint pos(605.0f, 335.0f);
	eMenu = app->psystem->AddEmiter(pos, EMITTER_TYPE_BURST);

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	/*if (Continuar != nullptr)
	{
		NuevaPartida->selected = false;
		Continuar->selected = false;
	}*/

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

	int mx, my;

	if (app->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN) {
		app->input->GetMousePosition(mx, my);
		fPoint pos((float)mx, (float)my);
		app->psystem->AddEmiter(pos, EMITTER_TYPE_SMOKE);
	}

	//if (app->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN) {
	//	app->input->GetMousePosition(mx, my);
	//	fPoint pos((float)mx, (float)my);
	//	app->psystem->AddEmiter(pos, EMITTER_TYPE_SPARK);
	//}

	if (showSettings == false)
	{
		app->render->DrawTexture(menuMain, 0, 0);
		app->render->DrawTexture(logo, 500, 100);
	}

	if (showSavedGames) {
		ShowSavedGames();
	}
	if (showNewGames) { ShowNewGames(); };
	if (showControls) { ShowControls(); };
	/*if (showSettings && !_showSettings) {
		SettingsInterface();
	}*/
	/*if (showSettings) { app->render->DrawTexture(settings, 500, 500); }*/

	if (showCredits) { ShowCredits(); }

	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		app->fadeToBlack->FadeToBlack(this, app->scene_testing, 90);
		app->menu->active = true;
	}
	if (fullscreen != nullptr && fullscreen->click) {
		Fullscreen();
	}

	if (ajustes == false && showCredits == false && showSavedGames == false && showControls == false && showNewGames == false)
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

	showSettings = false;
	showControls = false;
	showCredits = false;
	showNewGames = false;
	showSavedGames = false;

	_showSettings = false;
	_showControls = false;
	_showCredits = false;
	_showNewGames = false;
	_showSavedGames = false;

	eMenu = nullptr;
	app->psystem->RemoveAllEmitters();

	return true;
}

bool Scene_Menu::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Press Gui Control: %d", control->id);

	switch (control->id)
	{

	case 1:
		showNewGames = true;
		control->selected = false;
		app->guiManager->minId = 20;
		app->guiManager->maxId = 23;
		app->guiManager->pointerId = 20;
		/*app->guiManager->DestroyGuiControl(NuevaPartida);
		app->guiManager->DestroyGuiControl(Continuar);*/

		break;

	case 2:
		showSavedGames = true;
		control->selected = false;
		app->guiManager->minId = 13;
		app->guiManager->maxId = 16;
		app->guiManager->pointerId = 13;
		/*app->guiManager->DestroyGuiControl(NuevaPartida);
		app->guiManager->DestroyGuiControl(Continuar);*/
		break;

	case 3:
		showSettings = true;
		control->selected = false;
		app->guiManager->minId = 7;
		app->guiManager->maxId = 11;
		app->guiManager->pointerId = 7;
		/*	app->guiManager->DestroyGuiControl(NuevaPartida);
			app->guiManager->DestroyGuiControl(Continuar);*/
		break;

	case 4:
		showControls = true;
		control->selected = false;
		app->guiManager->minId = 17;
		app->guiManager->maxId = 17;
		app->guiManager->pointerId = 17;
		break;

	case 5:
		showCredits = true;
		control->selected = false;
		app->guiManager->minId = 12;
		app->guiManager->maxId = 12;
		app->guiManager->pointerId = 12;
		/*app->guiManager->DestroyGuiControl(NuevaPartida);
		app->guiManager->DestroyGuiControl(Continuar);*/
		app->audio->LoadAudioMusic("credits", 1.0f);
		break;

	case 6:
		app->closeApplication = true;
		break;

	case 11:
		showSettings = false;
		_showSettings = false;
		/*DestroySettingsInterface();*/
		app->guiManager->minId = 1;
		app->guiManager->maxId = 6;
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
		app->guiManager->maxId = 6;
		app->guiManager->pointerId = 1;
		app->audio->StopMusic(1.0);
		app->audio->LoadAudioMusic("menu", 1.0f);
		break;

	case 13:
		if (app->fadeToBlack->currentStep == 0) {
			app->savedGame = 1;
			/*app->LoadRequest();*/
			app->fadeToBlack->FadeToBlack(this, app->scene_pueblo, 90);
			app->menu->active = true;
			app->guiManager->pointerId = 100;
			app->guiManager->DestroyGuiControl(gcCloseSavedGames);
			app->guiManager->DestroyGuiControl(partida1);
			app->guiManager->DestroyGuiControl(partida2);
			app->guiManager->DestroyGuiControl(partida3);
		}
		break;

	case 14:
		if (app->fadeToBlack->currentStep == 0) {
			app->savedGame = 2;
			//app->LoadRequest();
			app->fadeToBlack->FadeToBlack(this, app->scene_pueblo, 90);
			app->menu->active = true;
			app->guiManager->pointerId = 100;
			app->guiManager->DestroyGuiControl(gcCloseSavedGames);
			app->guiManager->DestroyGuiControl(partida1);
			app->guiManager->DestroyGuiControl(partida2);
			app->guiManager->DestroyGuiControl(partida3);
		}

		break;

	case 15:
		if (app->fadeToBlack->currentStep == 0) {
			app->savedGame = 3;
			//app->LoadRequest();
			app->fadeToBlack->FadeToBlack(this, app->scene_pueblo, 90);
			app->menu->active = true;
			app->guiManager->pointerId = 100;
			app->guiManager->DestroyGuiControl(gcCloseSavedGames);
			app->guiManager->DestroyGuiControl(partida1);
			app->guiManager->DestroyGuiControl(partida2);
			app->guiManager->DestroyGuiControl(partida3);
		}

		break;

	case 16:
		showSavedGames = false;
		_showSavedGames = false;
		ListItem<GuiControl*>* controlB;
		for (controlB = controlsScene.start; controlB != NULL; controlB = controlB->next)
		{
			controlB->data->state = GuiControlState::NORMAL;
		}
		app->guiManager->DestroyGuiControl(gcCloseSavedGames);
		app->guiManager->DestroyGuiControl(partida1);
		app->guiManager->DestroyGuiControl(partida2);
		app->guiManager->DestroyGuiControl(partida3);
		app->guiManager->minId = 1;
		app->guiManager->maxId = 6;
		app->guiManager->pointerId = 1;

	case 17:
		showControls = false;
		_showControls = false;
		ListItem<GuiControl*>* controlC;
		for (controlC = controlsScene.start; controlC != NULL; controlC = controlC->next)
		{
			controlC->data->state = GuiControlState::NORMAL;
		}
		app->guiManager->DestroyGuiControl(gcCloseControls);
		app->guiManager->minId = 1;
		app->guiManager->maxId = 6;
		app->guiManager->pointerId = 1;

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
	case 20:
		if (app->fadeToBlack->currentStep == 0) {
			app->fadeToBlack->FadeToBlack(this, app->scene_pueblo_tutorial);
			app->menu->active = true;
			app->guiManager->pointerId = 100;
			app->savedGame = 1;
			app->guiManager->DestroyGuiControl(gcCloseNewGames);
			app->guiManager->DestroyGuiControl(nuevaPartida1);
			app->guiManager->DestroyGuiControl(nuevaPartida2);
			app->guiManager->DestroyGuiControl(nuevaPartida3);
		}
		break;

	case 21:
		if (app->fadeToBlack->currentStep == 0) {
			app->fadeToBlack->FadeToBlack(this, app->scene_pueblo_tutorial);
			app->menu->active = true;
			app->guiManager->pointerId = 100;
			app->savedGame = 2;
			app->guiManager->DestroyGuiControl(gcCloseNewGames);
			app->guiManager->DestroyGuiControl(nuevaPartida1);
			app->guiManager->DestroyGuiControl(nuevaPartida2);
			app->guiManager->DestroyGuiControl(nuevaPartida3);
		}
		break;

	case 22:
		if (app->fadeToBlack->currentStep == 0) {
			app->fadeToBlack->FadeToBlack(this, app->scene_pueblo_tutorial);
			app->menu->active = true;
			app->guiManager->pointerId = 100;
			app->savedGame = 3;
			app->guiManager->DestroyGuiControl(gcCloseNewGames);
			app->guiManager->DestroyGuiControl(nuevaPartida1);
			app->guiManager->DestroyGuiControl(nuevaPartida2);
			app->guiManager->DestroyGuiControl(nuevaPartida3);
		}
		break;

	case 23:
		showNewGames = false;
		_showNewGames = false;
		ListItem<GuiControl*>* controlD;
		for (controlD = controlsScene.start; controlD != NULL; controlD = controlD->next)
		{
			controlD->data->state = GuiControlState::NORMAL;
		}
		app->guiManager->DestroyGuiControl(gcCloseNewGames);
		app->guiManager->DestroyGuiControl(nuevaPartida1);
		app->guiManager->DestroyGuiControl(nuevaPartida2);
		app->guiManager->DestroyGuiControl(nuevaPartida3);
		app->guiManager->minId = 1;
		app->guiManager->maxId = 6;
		app->guiManager->pointerId = 1;

	default:
		break;
	}

	return true;
}

void Scene_Menu::SettingsInterface()
{
	uint windowWidth, windowHeight;
	app->win->GetWindowSize(windowWidth, windowHeight);

	app->render->DrawTexture(menuMain, 0, 0);
	app->render->DrawTexture(settings, 400, 100);

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

			SDL_Rect MusicPos = { windowWidth / 2 - 110 ,windowHeight / 2 - 40, 200, 50 };
			music = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 7, "MUSIC", MusicPos, this, { 0, 0,20, 20 }, { 0,0,0,0 }, 0, 100);

			SDL_Rect SfxPos = { windowWidth / 2 - 110 ,windowHeight / 2 + 20, 200, 50 };
			sfx = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 8, "SFX", SfxPos, this, { 0, 0, 20, 20 }, { 0,0,0,0 }, 0, 100);

			SDL_Rect FullScreen = { windowWidth / 2 + 100 ,windowHeight / 2 + 60, 230,50 };
			fullscreen = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 9, "FULLSCREEN", FullScreen, this, { 0,0,0,0 }, { -50,0,0,0 });

			SDL_Rect vSyncpos = { windowWidth / 2 + 100 ,windowHeight / 2 + 130, 200, 50 };
			vsync = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 10, "VSYNC", vSyncpos, this, { 0, 0, 20, 20 });

			SDL_Rect TitlePos = { 600, 550,	60,25 };
			atras = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "ATRÁS", TitlePos, this, { 0,0,0,0 });

			/*controlsSettings.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "ATR�S", SDL_Rect{ 550, 600,	136,46 }, this));*/

			if (app->win->fullscreen)
			{
				fullscreen->click = true;
				fullScreenActive = true;
			}
			else
			{
				fullscreen->click = false;
				fullScreenActive = false;
			}

			if (vsyncActive)
			{
				vsync->click = true;
				vsyncActive = true;
			}
			else
			{
				vsync->click = false;
				vsyncActive = false;
			}

			//Cargar la barra de audio del save_game
			((GuiControlSlider*)music)->value = app->audio->volumeMusic;
			((GuiControlSlider*)sfx)->value = app->audio->volumeFx;
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
	if (showSettings)
	{
		if (music != nullptr)
		{
			int newVolume = ((GuiControlSlider*)music)->value;
			// Asegurarse de que el nuevo volumen est� dentro de los l�mites v�lidos (0-128 para SDL Mixer)
			newVolume = std::max(0, std::min(128, newVolume));
			// Establecer el volumen de la m�sica
			Mix_VolumeMusic(newVolume);
			app->audio->volumeMusic = newVolume;
		}

		if (sfx != nullptr)
		{
			int newSFXVolume = ((GuiControlSlider*)sfx)->value;
			newSFXVolume = std::max(0, std::min(128, newSFXVolume));

			// Iterar sobre cada canal activo y ajustar su volumen
			for (const auto& pair : app->audio->activeChannels)
			{
				int channel = pair.second; // Obtener el canal asociado al ID del efecto de sonido
				Mix_Volume(channel, newSFXVolume);
			}

			app->audio->volumeFx = newSFXVolume;
		}


		if (fullscreen->selected && (app->input->GetButton(CONFIRM) == KEY_DOWN || app->input->GetButton(SELECT) == KEY_DOWN))
		{
			fullscreen->click = !fullscreen->click;
		}
		if (vsync->selected && (app->input->GetButton(CONFIRM) == KEY_DOWN || app->input->GetButton(SELECT) == KEY_DOWN))
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


	/*_showSettings = true;*/
}

void Scene_Menu::ShowCredits()
{
	if (showCredits && !_showCredits) {
		ListItem<GuiControl*>* control;
		for (control = controlsScene.start; control != NULL; control = control->next)
		{
			control->data->state = GuiControlState::DISABLED;
		}

		gcCloseCredits = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 12, "ATRÁS", SDL_Rect{ (int)windowW / 2 - 500,	(int)windowH - 50,	60,25 }, this);
		_showCredits = true;
	}

	/*if (app->entityManager->isFinishi && closeshowCredits.ReadMSec() >= 10000) {
		showCredits = false;
		app->entityManager->canShowFinal = false;
		app->entityManager->isFinishi = false;
	}*/
	app->render->DrawTexture(credits, 0, 0);
}

void Scene_Menu::ShowSavedGames()
{
	ListItem<GuiControl*>* control;
	for (control = controlsScene.start; control != NULL; control = control->next)
	{
		control->data->state = GuiControlState::DISABLED;
	}
	if (showSavedGames && !_showSavedGames) {
		partida1 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 13, "PARTIDA GUARDADA 1", SDL_Rect{ (int)windowW / 2 - 110,	(int)windowH - 505,	180,25 }, this);
		partida2 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 14, "PARTIDA GUARDADA 2", SDL_Rect{ (int)windowW / 2 - 110,	(int)windowH - 415,	180,25 }, this);
		partida3 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 15, "PARTIDA GUARDADA 3", SDL_Rect{ (int)windowW / 2 - 110,	(int)windowH - 320,	180,25 }, this);
		gcCloseSavedGames = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 16, "ATRÁS", SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH - 200,	60,25 }, this);
		_showSavedGames = true;
	}

	app->render->DrawTexture(savedGames, 0, 0);
	app->render->DrawTexture(coin, 770, 252, 0.35f);
	app->render->DrawTexture(coin, 770, 346, 0.35f);
	app->render->DrawTexture(coin, 770, 440, 0.35f);

	app->render->DrawTexture(clock, 700, 257, 0.65f);
	app->render->DrawTexture(clock, 700, 351, 0.65f);
	app->render->DrawTexture(clock, 700, 445, 0.65f);

	std::string quantityStrOne = std::to_string(coinQuantityOne);
	app->render->DrawText(quantityStrOne.c_str(), 730, 255, 45, 27, 0, 0, 0, 0, true);

	std::string quantityStrTwo = std::to_string(coinQuantityTwo);
	app->render->DrawText(quantityStrTwo.c_str(), 730, 349, 45, 27, 0, 0, 0, 0, true);

	std::string quantityStrThree = std::to_string(coinQuantityThree);
	app->render->DrawText(quantityStrThree.c_str(), 730, 443, 45, 27, 0, 0, 0, 0, true);
}

void Scene_Menu::ShowNewGames()
{
	ListItem<GuiControl*>* control;
	for (control = controlsScene.start; control != NULL; control = control->next)
	{
		control->data->state = GuiControlState::DISABLED;
	}
	if (showNewGames && !_showNewGames) {
		nuevaPartida1 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 20, "NUEVA PARTIDA 1", SDL_Rect{ (int)windowW / 2 - 110,	(int)windowH - 485,	160,25 }, this);
		nuevaPartida2 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 21, "NUEVA PARTIDA 2", SDL_Rect{ (int)windowW / 2 - 110,	(int)windowH - 395,	160,25 }, this);
		nuevaPartida3 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 22, "NUEVA PARTIDA 3", SDL_Rect{ (int)windowW / 2 - 110,	(int)windowH - 300,	160,25 }, this);
		gcCloseNewGames = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 23, "ATRÁS", SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH - 200,	60,25 }, this);
		_showNewGames = true;
	}
	app->render->DrawTexture(savedGames, 0, 0);
}

void Scene_Menu::ShowControls()
{
	ListItem<GuiControl*>* control;
	for (control = controlsScene.start; control != NULL; control = control->next)
	{
		control->data->state = GuiControlState::DISABLED;
	}
	if (showControls && !_showControls) {
		gcCloseControls = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 17, "ATRÁS", SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH - 100,	60,25 }, this);
		_showControls = true;
	}
	app->render->DrawTexture(controls, 0, 0);
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
