#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene_Testing.h"
#include "Map.h"
#include "Item.h"
#include "ModuleFadeToBlack.h"
#include "Optick/include/optick.h"

#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"

Scene_testing::Scene_testing(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("scene_testing");
}

// Destructor
Scene_testing::~Scene_testing()
{}

// Called before render is available
bool Scene_testing::Awake(pugi::xml_node config)
{
	LOG("Loading Scene_testing");
	bool ret = true;
	if(!active){
		return ret;
	}

	return ret;
}

// Called before the first frame
bool Scene_testing::Start()
{
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	config = configFile.child("config").child("scene_testing");
	//L03: DONE 3b: Instantiate the player using the entity manager
	//L04 DONE 7: Get player paremeters
	//player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	////Assigns the XML node to a member in player
	//player->config = config.child("player");

	//Get the map name from the config file and assigns the value in the module
	app->map->name = config.child("map").attribute("name").as_string();
	app->map->path = config.child("map").attribute("path").as_string();
	app->map->pathTextures = config.child("map").attribute("pathTextures").as_string();

	// iterate all items in the Scene_testing
	// Checkhttps://pugixml.org/docs/quickstart.html#access

	/*for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}*/

	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	img = app->tex->Load("Assets/Textures/test.png");
	mapaFondo = app->tex->Load("Assets/Textures/Mapas/Testing/background.png");
	//Music is commented so that you can add your own music
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	// Texture to highligh mouse position 
	mouseTileTex = app->tex->Load("Assets/Mapas/tileSelection.png");

	// L15: DONE 2: Instantiate a new GuiControlButton in the Scene_testing

	SDL_Rect btPos = { windowW / 2 - 60,20, 120,20};
	gcButtom = (GuiControlButton*) app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "MyButton", btPos, this);

	app->entityManager->Enable();

	return true;
}

// Called each loop iteration
bool Scene_testing::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene_testing::Update(float dt)
{

   	OPTICK_EVENT();

	//Dibujar mapa
	//app->render->DrawTexture(mapaFondo, 0, 0);



	//L02 DONE 3: Make the camera movement independent of framerate
	float camSpeed = 1; 


	//if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	//	app->render->camera.y -= (int)ceil(camSpeed * dt);

	//if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	//	app->render->camera.y += (int)ceil(camSpeed * dt);

	//if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	//	app->render->camera.x -= (int)ceil(camSpeed * dt);

	//if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	//	app->render->camera.x += (int)ceil(camSpeed * dt);

	








	// L09 DONE 6: Implement a method that repositions the player in the map with a mouse click
	//iPoint origin = iPoint(2, 21);
	//app->map->pathfinding->CreatePath(origin, app->map->WorldToMap(player->position.x,player->position.y));

	//// L13: Get the latest calculated path and draw
	//const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();
	//for (uint i = 0; i < path->Count(); ++i)
	//{
	//	iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
	//	app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
	//}
	
	// L14: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		app->fadeToBlack->FadeToBlack(this, app->scene_testing);
	}
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	return true;
}

// Called each loop iteration
bool Scene_testing::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene_testing::CleanUp()
{
	LOG("Freeing Scene_testing");

	return true;
}



bool Scene_testing::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	return true;
}

Player* Scene_testing::GetPLayer()
{
	return player;
}

