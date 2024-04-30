
#include "EntityManager.h"
#include "Commerce.h"
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

Commerce::Commerce(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("commerce");
}

// Destructor
Commerce::~Commerce()
{}

// Called before render is available
bool Commerce::Awake(pugi::xml_node config)
{
	LOG("Loading Commerce");
	bool ret = true;

	return ret;

}

bool Commerce::Start() {

	bool ret = true;


	return ret;
}

// Called before quitting
bool Commerce::CleanUp()
{
	bool ret = true;
	
	return ret;
}

bool Commerce::Update(float dt)
{

	bool ret = true;



	return ret;
}

bool Commerce::PostUpdate()
{
	bool ret = true;
	
	return ret;
}






