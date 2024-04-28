
#include "EntityManager.h"
#include "CommerceManager.h"
#include "Player.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene_testing.h"
#include "inventity.h"
#include "SwordInv.h"
#include "GarraInv.h"
#include "ArmaduraInv.h"
#include "DiamanteInv.h"
#include "ItemInv.h"
#include "OjoInv.h"
#include "Defs.h"
#include "Log.h"
#include "SString.h"

CommerceManager::CommerceManager(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("commercemanager");
}

// Destructor
CommerceManager::~CommerceManager()
{}

// Called before render is available
bool CommerceManager::Awake(pugi::xml_node config)
{
	LOG("Loading Commerce Manager");
	bool ret = true;

	return ret;

}

bool CommerceManager::Start() {

	bool ret = true;


	return ret;
}

// Called before quitting
bool CommerceManager::CleanUp()
{
	bool ret = true;
	
	return ret;
}

bool CommerceManager::Update(float dt)
{

	bool ret = true;



	return ret;
}

bool CommerceManager::PostUpdate()
{
	bool ret = true;
	
	return ret;
}






