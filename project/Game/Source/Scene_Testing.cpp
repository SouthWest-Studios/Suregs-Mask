#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene_Testing.h"
#include "Map.h"
#include "Item.h"
#include "BestiarioManager.h"
#include "Bestiario.h"
#include "ModuleFadeToBlack.h"
#include "Optick/include/optick.h"
#include "MiniGameFishing.h"
#include "Menu.h"

#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "Item_Carbon.h"
#include "Item_Viscera.h"
#include "Item_Rubi.h"
#include "Item_Diente.h"
#include "Item_Amatista.h"
#include "Item_Cola.h"
#include "Item_Polvora.h"
#include "Item_Hueso.h"
#include "Item_Nota.h"
#include "Item_Pocion_Dano.h"
#include "Item_Pocion_Regeneracion.h"
#include "Item_Pocion_Velocidad.h"
#include "Item_Pocion_Vida_1.h"
#include "Item_Pocion_Vida_2.h"
#include "Item_Pocion_Vida_3.h"
#include "Item_Pocion_Vida_Max.h"
#include "Item_Saliva.h"
#include "Item_Zafiro.h"

Scene_testing::Scene_testing(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name = ("scene_testing");
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
	/*player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);*/
	////Assigns the XML node to a member in player
	/*player->config = config.child("player");*/

	//Get the map name from the config file and assigns the value in the module
	app->map->name = config.child("map").attribute("name").as_string();
	app->map->path = config.child("map").attribute("path").as_string();
	app->map->pathTextures = config.child("map").attribute("pathTextures").as_string();
	
	// iterate all items in the Scene_testing
	// Checkhttps://pugixml.org/docs/quickstart.html#access


	/*for (pugi::xml_node itemNode = config.child("player"); itemNode; itemNode = itemNode.next_sibling("player"))
	{
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = itemNode;
	}*/
	/*for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}*/

	
	
	/*for (pugi::xml_node itemNode = config.child("minigamefishing"); itemNode; itemNode = itemNode.next_sibling("minigamefishing"))
	{
		fishing = (MiniGameFishing*)app->entityManager->CreateEntity(EntityType::ROD);
		fishing->parameters = itemNode;
	}*/
	/*fishing = (MiniGameFishing*)app->entityManager->CreateEntity(EntityType::ROD);
	fishing->parameters = config.child("minigamefishing");*/

	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	img = app->tex->Load("Assets/Textures/test.png");
	mapaFondo = app->tex->Load("Assets/Textures/Mapas/Testing/background.png");
	//Music is commented so that you can add your own music
	app->audio->LoadAudioMusic("town", 2.0f);

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	// Texture to highligh mouse position 
	mouseTileTex = app->tex->Load("Assets/Mapas/tileSelection.png");

	// L15: DONE 2: Instantiate a new GuiControlButton in the Scene_testing

	/*SDL_Rect btPos = { windowW / 2 - 60,20, 120,20};
	gcButtom = (GuiControlButton*) app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "MyButton", btPos, this);*/

	app->entityManager->Enable();

	pugi::xml_parse_result parseResult2 = configFile.load_file("config.xml");
	if (parseResult2) {
		configNode = configFile.child("config");
	}

	Item_Carbon* carbon = (Item_Carbon*)app->entityManager->CreateEntity(EntityType::ITEM_CARBON);
	carbon->config = configNode.child("entities_data").child("item_carbon");
	carbon->position = iPoint(1903, 2595);
	carbon->Start();

	Item_Viscera* viscera = (Item_Viscera*)app->entityManager->CreateEntity(EntityType::ITEM_VISCERAS);
	viscera->config = configNode.child("entities_data").child("item_viscera");
	viscera->position = iPoint(1923, 2595);
	viscera->Start();

	Item_Rubi* rubi = (Item_Rubi*)app->entityManager->CreateEntity(EntityType::ITEM_RUBI);
	rubi->config = configNode.child("entities_data").child("item_rubi");
	rubi->position = iPoint(1943, 2595);
	rubi->Start();

	Item_Diente* diente = (Item_Diente*)app->entityManager->CreateEntity(EntityType::ITEM_DIENTE);
	diente->config = configNode.child("entities_data").child("item_diente");
	diente->position = iPoint(1973, 2595);
	diente->Start();

	Item_Amatista* amatista = (Item_Amatista*)app->entityManager->CreateEntity(EntityType::ITEM_AMATISTA);
	amatista->config = configNode.child("entities_data").child("item_amatista");
	amatista->position = iPoint(1873, 2595);
	amatista->Start();

	Item_Cola* cola = (Item_Cola*)app->entityManager->CreateEntity(EntityType::ITEM_COLA);
	cola->config = configNode.child("entities_data").child("item_cola");
	cola->position = iPoint(1873, 2555);
	cola->Start();

	Item_Polvora* polvora = (Item_Polvora*)app->entityManager->CreateEntity(EntityType::ITEM_POLVORA);
	polvora->config = configNode.child("entities_data").child("item_polvora");
	polvora->position = iPoint(1853, 2555);
	polvora->Start();

	Item_Hueso* hueso = (Item_Hueso*)app->entityManager->CreateEntity(EntityType::ITEM_HUESO);
	hueso->config = configNode.child("entities_data").child("item_hueso");
	hueso->position = iPoint(1823, 2555);
	hueso->Start();

	Item_Nota* nota = (Item_Nota*)app->entityManager->CreateEntity(EntityType::ITEM_NOTA, 0);
	nota->config = configNode.child("entities_data").child("item_nota");
	nota->position = iPoint(1803, 2555);
	nota->Start();

	Item_Nota* nota2 = (Item_Nota*)app->entityManager->CreateEntity(EntityType::ITEM_NOTA, 1);
	nota2->config = configNode.child("entities_data").child("item_nota");
	nota2->position = iPoint(1783, 2535);
	nota2->Start();

	Item_Nota* nota3 = (Item_Nota*)app->entityManager->CreateEntity(EntityType::ITEM_NOTA, 2);
	nota3->config = configNode.child("entities_data").child("item_nota");
	nota3->position = iPoint(1763, 2535);
	nota3->Start();

	Item_Nota* nota4 = (Item_Nota*)app->entityManager->CreateEntity(EntityType::ITEM_NOTA, 3);
	nota4->config = configNode.child("entities_data").child("item_nota");
	nota4->position = iPoint(1803, 2555);
	nota4->Start();

	Item_Nota* nota5 = (Item_Nota*)app->entityManager->CreateEntity(EntityType::ITEM_NOTA, 4);
	nota5->config = configNode.child("entities_data").child("item_nota");
	nota5->position = iPoint(1803, 2555);
	nota5->Start();

	Item_Nota* nota6 = (Item_Nota*)app->entityManager->CreateEntity(EntityType::ITEM_NOTA, 5);
	nota6->config = configNode.child("entities_data").child("item_nota");
	nota6->position = iPoint(1803, 2555);
	nota6->Start();

	Item_Nota* nota7 = (Item_Nota*)app->entityManager->CreateEntity(EntityType::ITEM_NOTA, 6);
	nota7->config = configNode.child("entities_data").child("item_nota");
	nota7->position = iPoint(1803, 2555);
	nota7->Start();

	Item_Nota* nota8 = (Item_Nota*)app->entityManager->CreateEntity(EntityType::ITEM_NOTA, 7);
	nota8->config = configNode.child("entities_data").child("item_nota");
	nota8->position = iPoint(1803, 2555);
	nota8->Start();

	Item_Nota* nota9 = (Item_Nota*)app->entityManager->CreateEntity(EntityType::ITEM_NOTA, 8);
	nota9->config = configNode.child("entities_data").child("item_nota");
	nota9->position = iPoint(1803, 2555);
	nota9->Start();

	Item_Nota* nota10 = (Item_Nota*)app->entityManager->CreateEntity(EntityType::ITEM_NOTA, 9);
	nota10->config = configNode.child("entities_data").child("item_nota");
	nota10->position = iPoint(1803, 2555);
	nota10->Start();

	app->bestiarioManager->CreateItem("osiris");
	app->bestiarioManager->CreateItem("muur");
	app->bestiarioManager->CreateItem("osiris");
	app->bestiarioManager->CreateItem("muur");
	app->bestiarioManager->CreateItem("osiris");
	app->bestiarioManager->CreateItem("muur");

	Item_Pocion_Dano* pocion_dano = (Item_Pocion_Dano*)app->entityManager->CreateEntity(EntityType::ITEM_POCION_DANO);
	pocion_dano->config = configNode.child("entities_data").child("item_pocion_dano");
	pocion_dano->position = iPoint(1703, 2255);
	pocion_dano->Start();
	
	Item_Pocion_Regeneracion* pocion_regeneracion = (Item_Pocion_Regeneracion*)app->entityManager->CreateEntity(EntityType::ITEM_POCION_REGENERACION);
	pocion_regeneracion->config = configNode.child("entities_data").child("item_pocion_regeneracion");
	pocion_regeneracion->position = iPoint(1733, 2255);
	pocion_regeneracion->Start();

	Item_Pocion_Velocidad* pocion_velocidad = (Item_Pocion_Velocidad*)app->entityManager->CreateEntity(EntityType::ITEM_POCION_VELOCIDAD);
	pocion_velocidad->config = configNode.child("entities_data").child("item_pocion_velocidad");
	pocion_velocidad->position = iPoint(1753, 2255);
	pocion_velocidad->Start();

	Item_Pocion_Vida_1* pocion_vida_1 = (Item_Pocion_Vida_1*)app->entityManager->CreateEntity(EntityType::ITEM_POCION_VIDA_1);
	pocion_vida_1->config = configNode.child("entities_data").child("item_pocion_vida_1");
	pocion_vida_1->position = iPoint(1783, 2255);
	pocion_vida_1->Start();

	Item_Pocion_Vida_2* pocion_vida_2 = (Item_Pocion_Vida_2*)app->entityManager->CreateEntity(EntityType::ITEM_POCION_VIDA_2);
	pocion_vida_2->config = configNode.child("entities_data").child("item_pocion_vida_2");
	pocion_vida_2->position = iPoint(1813, 2255);
	pocion_vida_2->Start();

	Item_Pocion_Vida_3* pocion_vida_3 = (Item_Pocion_Vida_3*)app->entityManager->CreateEntity(EntityType::ITEM_POCION_VIDA_3);
	pocion_vida_3->config = configNode.child("entities_data").child("item_pocion_vida_3");
	pocion_vida_3->position = iPoint(1843, 2255);
	pocion_vida_3->Start();

	Item_Pocion_Vida_Max* pocion_vida_max = (Item_Pocion_Vida_Max*)app->entityManager->CreateEntity(EntityType::ITEM_POCION_VIDA_MAX);
	pocion_vida_max->config = configNode.child("entities_data").child("item_pocion_vida_max");
	pocion_vida_max->position = iPoint(1873, 2255);
	pocion_vida_max->Start();

	Item_Saliva* saliva = (Item_Saliva*)app->entityManager->CreateEntity(EntityType::ITEM_SALIVA);
	saliva->config = configNode.child("entities_data").child("item_saliva");
	saliva->position = iPoint(1823, 2455);
	saliva->Start();

	Item_Zafiro* zafiro = (Item_Zafiro*)app->entityManager->CreateEntity(EntityType::ITEM_ZAFIRO);
	zafiro->config = configNode.child("entities_data").child("item_zafiro");
	zafiro->position = iPoint(1523, 2455);
	zafiro->Start();

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
	/*if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		app->fadeToBlack->FadeToBlack(this, app->scene_testing);
		app->menu->active = true;
	}*/
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
		app->SaveRequest();
	}
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();

	return true;
}

// Called each loop iteration
bool Scene_testing::PostUpdate()
{
	bool ret = true;

	/*if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;*/

	return ret;
}

// Called before quitting
bool Scene_testing::CleanUp()
{
	LOG("Freeing Scene_testing");
	//delete fishing;
	return true;
}



//MiniGameFishing* Scene_testing::GetRod()
//{
//	return fishing;
//}

bool Scene_testing::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	return true;
}

