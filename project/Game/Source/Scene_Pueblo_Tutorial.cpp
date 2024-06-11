#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene_Pueblo_Tutorial.h"
#include "Map.h"
#include "Item.h"
#include "Cofre.h"
#include "ModuleFadeToBlack.h"
#include "Optick/include/optick.h"
#include "Fishing.h"
#include "MiniGameFishing.h"

#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "Elevator.h"
#include "InventoryManager.h"
#include "Estatua.h"

Scene_Pueblo_Tutorial::Scene_Pueblo_Tutorial(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name = ("pueblo_tutorial");
}

// Destructor
Scene_Pueblo_Tutorial::~Scene_Pueblo_Tutorial()
{}

// Called before render is available
bool Scene_Pueblo_Tutorial::Awake(pugi::xml_node config)
{
	LOG("Loading Scene_Pueblo_Tutorial");
	bool ret = true;
	if(!active){
		return ret;
	}

	

	return ret;
}
int startt = 0;
// Called before the first frame
bool Scene_Pueblo_Tutorial::Start()
{

	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	config = configFile.child("config").child(name.GetString());

	pugi::xml_parse_result parseResult2 = configFile.load_file("config.xml");
	if (parseResult2) {
		configNode = configFile.child("config");
	}
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

	// L15: DONE 2: Instantiate a new GuiControlButton in the Scene_Pueblo_Tutorial


	app->entityManager->Enable();

	//app->SaveRequest();


	fishing = (MiniGameFishing*)app->entityManager->CreateEntity(EntityType::ROD);
	fishing->parameters = config.child("minigamefishing");


	Estatua* estatua = (Estatua*)app->entityManager->CreateEntity(EntityType::ESTATUA);
	estatua->config = configNode.child("entities_data").child("estatua");
	estatua->position = iPoint(4501, 3205);
	estatua->Start();

	return true;
}

// Called each loop iteration
bool Scene_Pueblo_Tutorial::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene_Pueblo_Tutorial::Update(float dt)
{
   	OPTICK_EVENT();


	if (app->entityManager->GetPlayer() != nullptr) {
		if (app->entityManager->GetPlayer()->primaryMask != Mask::NOMASK) {
			app->entityManager->GetPlayer()->primaryMask = Mask::NOMASK;
		}

		if (app->entityManager->GetPlayer()->secondaryMask != Mask::NOMASK) {
			app->entityManager->GetPlayer()->secondaryMask = Mask::NOMASK;
		}
		
		

		app->inventoryManager->maskZeroPoints = 0;
		app->inventoryManager->maskOnePoints = 0;
		app->inventoryManager->maskTwoPoints = 0;
		app->inventoryManager->maskThreePoints = 0;
	}


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

	if (app->tutorialHasFinished) {
		if(!chestCreated){
			pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
			if (parseResult) {
				configNode = configFile.child("config");
			}
			cofreTutorial = (Cofre*)app->entityManager->CreateEntity(EntityType::COFRE);
			cofreTutorial->config = configNode.child("entities_data").child("cofre");
			cofreTutorial->position = iPoint(500, 3700);
			cofreTutorial->tutorial = true;
			cofreTutorial->Start();
			chestCreated = true;
		}
	}

	return true;
}

// Called each loop iteration
bool Scene_Pueblo_Tutorial::PostUpdate()
{
	bool ret = true;


	return ret;
}

// Called before quitting
bool Scene_Pueblo_Tutorial::CleanUp()
{
	LOG("Freeing Scene_Pueblo_Tutorial");
	app->entityManager->DestroyEntity(fishing);
	//delete fishing;
	return true;
}



bool Scene_Pueblo_Tutorial::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	return true;
}

MiniGameFishing* Scene_Pueblo_Tutorial::GetRod()
{
	return fishing;
}
