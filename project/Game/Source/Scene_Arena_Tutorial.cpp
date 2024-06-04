#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene_Pueblo_Tutorial.h"
#include "Scene_Arena_Tutorial.h"
#include "Scene_Pueblo.h"
#include "Map.h"
#include "Item.h"
#include "ModuleFadeToBlack.h"
#include "Optick/include/optick.h"

#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "Elevator.h"

Scene_Arena_Tutorial::Scene_Arena_Tutorial(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name = ("arena_tutorial");
}

// Destructor
Scene_Arena_Tutorial::~Scene_Arena_Tutorial()
{}

// Called before render is available
bool Scene_Arena_Tutorial::Awake(pugi::xml_node config)
{
	LOG("Loading Scene_Arena_Tutorial");
	bool ret = true;
	if(!active){
		return ret;
	}

	

	return ret;
}

// Called before the first frame
bool Scene_Arena_Tutorial::Start()
{
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	config = configFile.child("config").child(name.GetString());
	//L03: DONE 3b: Instantiate the player using the entity manager
	//L04 DONE 7: Get player paremeters
	/*player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);*/
	////Assigns the XML node to a member in player
	/*player->config = config.child("player");*/

	//Get the map name from the config file and assigns the value in the module
	app->map->nameMazmorra = config.child("map").attribute("name").as_string();
	app->map->path = config.child("map").attribute("path").as_string();
	app->map->pathTextures = config.child("map").attribute("pathTextures").as_string();
	
	// Stop the music from previous scenes
	app->audio->StopMusic();
	app->audio->LoadAudioAmbience("town_fx");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);
	
	// Texture to highligh mouse position 
	//mouseTileTex = app->tex->Load("Assets/Mapas/tileSelection.png");

	// L15: DONE 2: Instantiate a new GuiControlButton in the Scene_Arena_Tutorial

	app->entityManager->Enable();

	app->SaveRequest();

	return true;
}

// Called each loop iteration
bool Scene_Arena_Tutorial::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene_Arena_Tutorial::Update(float dt)
{
   	OPTICK_EVENT();

	//L02 DONE 3: Make the camera movement independent of framerate
	float camSpeed = 1; 

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();
	
	if (app->audio->playingAmbience == false)
	{
		app->audio->LoadAudioAmbience("town_fx");
	}
	if (app->audio->playingMusic == true && app->audio->musicTimer.ReadMSec() >= app->audio->musicDuration)
	{
		app->audio->StopMusic(0.0f);
		app->audio->playingAmbience = false;
	}
	if (app->audio->playingMusic == false)
	{
		app->audio->PlayMusicAfterRandomDelay("town");
	}

	if (app->entityManager->GetPlayer()->currentStats.currentHealth <= 0) {
		app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_pueblo_tutorial, 90);
		app->tutorialHasFinished = true;
	}

	return true;
}

// Called each loop iteration
bool Scene_Arena_Tutorial::PostUpdate()
{
	bool ret = true;


	return ret;
}

// Called before quitting
bool Scene_Arena_Tutorial::CleanUp()
{
	LOG("Freeing Scene_Arena_Tutorial");
	return true;
}



bool Scene_Arena_Tutorial::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	return true;
}
