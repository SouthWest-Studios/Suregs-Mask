#include "EntityManager.h"
#include "MissionManager.h"
#include "Player.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene_testing.h"
#include "inventity.h"
#include "InventoryManager.h"
#include "ItemInv.h"
#include "Defs.h"
#include "Log.h"
#include "SString.h"
#include "Utils.cpp"
#include "Hud.h"

MissionManager::MissionManager(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("missionmanager");
}

// Destructor
MissionManager::~MissionManager()
{}

// Called before render is available
bool MissionManager::Awake(pugi::xml_node config)
{
	LOG("Loading Mission Manager");
	bool ret = true;

	return ret;

}

bool MissionManager::Start() {

	bool ret = true;

	return ret;
}

// Called before quitting
bool MissionManager::CleanUp()
{
	bool ret = true;

	return ret;
}

bool MissionManager::Update(float dt)
{

	bool ret = true;
	printf("LOADING MSIONS MANAGER");

	return ret;
}

bool MissionManager::PostUpdate()
{
	bool ret = true;

	return ret;
}









