#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene_Mazmorra4.h"
#include "Map.h"
#include "Item.h"
#include "ModuleFadeToBlack.h"
#include "Optick/include/optick.h"

#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"

Scene_Mazmorra4::Scene_Mazmorra4(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("scene_mazmorra4");
}

// Destructor
Scene_Mazmorra4::~Scene_Mazmorra4()
{}

// Called before render is available
bool Scene_Mazmorra4::Awake(pugi::xml_node config)
{
	LOG("Loading Scene_Mazmorra4");
	bool ret = true;
	if(!active){
		return ret;
	}

	return ret;
}

// Called before the first frame
bool Scene_Mazmorra4::Start()
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
	
	//Music is commented so that you can add your own music
	app->audio->LoadAudioMusic("town", 2.0f);

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	// Texture to highligh mouse position 
	//mouseTileTex = app->tex->Load("Assets/Mapas/tileSelection.png");

	// L15: DONE 2: Instantiate a new GuiControlButton in the Scene_Mazmorra4

	/*SDL_Rect btPos = { windowW / 2 - 60,20, 120,20};
	gcButtom = (GuiControlButton*) app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "MyButton", btPos, this);*/

	app->entityManager->Enable();

	return true;
}

// Called each loop iteration
bool Scene_Mazmorra4::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene_Mazmorra4::Update(float dt)
{

   	OPTICK_EVENT();

	//L02 DONE 3: Make the camera movement independent of framerate
	float camSpeed = 1; 

	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	return true;
}

// Called each loop iteration
bool Scene_Mazmorra4::PostUpdate()
{
	bool ret = true;


	return ret;
}

// Called before quitting
bool Scene_Mazmorra4::CleanUp()
{
	LOG("Freeing Scene_Mazmorra4");

	return true;
}



bool Scene_Mazmorra4::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	return true;
}

