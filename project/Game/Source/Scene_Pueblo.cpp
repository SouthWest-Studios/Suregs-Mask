#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
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

Scene_Pueblo::Scene_Pueblo(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name = ("scene_pueblo");
}

// Destructor
Scene_Pueblo::~Scene_Pueblo()
{}

// Called before render is available
bool Scene_Pueblo::Awake(pugi::xml_node config)
{
	LOG("Loading Scene_Pueblo");
	bool ret = true;
	if(!active){
		return ret;
	}

	

	return ret;
}

// Called before the first frame
bool Scene_Pueblo::Start()
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
	app->map->name = config.child("map").attribute("name").as_string();
	app->map->path = config.child("map").attribute("path").as_string();
	app->map->pathTextures = config.child("map").attribute("pathTextures").as_string();
	
	// Stop the music from previous scenes
	app->audio->StopMusic();
	// Timer for music
	app->audio->musicTimer.Start();

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);


	fishing = (MiniGameFishing*)app->entityManager->CreateEntity(EntityType::ROD);
	fishing->parameters = config.child("minigamefishing");

	

	/*fPoint pos(5728.0f, 416.0f);
	app->psystem->AddEmiter(pos,EMITTER_TYPE_SMOKE);*/

	// Texture to highligh mouse position 
	//mouseTileTex = app->tex->Load("Assets/Mapas/tileSelection.png");

	// L15: DONE 2: Instantiate a new GuiControlButton in the Scene_Pueblo

	/*SDL_Rect btPos = { windowW / 2 - 60,20, 120,20};
	gcButtom = (GuiControlButton*) app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "MyButton", btPos, this);*/
	/*fPoint pos(784.0, 218.0);
	app->psystem->AddEmiter(pos, EMITTER_TYPE_PURPLE_FLAME);*/

	app->entityManager->Enable();

	app->SaveRequest();

	return true;
}

// Called each loop iteration
bool Scene_Pueblo::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene_Pueblo::Update(float dt)
{
	/*int mx, my;
	app->input->GetMousePosition(mx, my);
	if (mx != NULL && my != NULL) {
		printf("MX: %f,  MY: %f", (float)mx, (float)my);
	}
	if (app->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN) {
		app->input->GetMousePosition(mx, my);
		fPoint pos((float)mx, (float)my);
		app->psystem->AddEmiter(pos, EMITTER_TYPE_PURPLE_FLAME);
	}*/
	


   	OPTICK_EVENT();

	//L02 DONE 3: Make the camera movement independent of framerate
	float camSpeed = 1; 

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();


	if (app->audio->playingMusic == false) 
	{
		app->audio->PlayMusicAfterDelay("town", 5.0f);
	}

	return true;
}

// Called each loop iteration
bool Scene_Pueblo::PostUpdate()
{
	bool ret = true;


	return ret;
}

// Called before quitting
bool Scene_Pueblo::CleanUp()
{
	LOG("Freeing Scene_Pueblo");
	app->entityManager->DestroyEntity(fishing);
	delete fishing;
	return true;
}



bool Scene_Pueblo::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	return true;
}


MiniGameFishing* Scene_Pueblo::GetRod()
{
	return fishing;
}

