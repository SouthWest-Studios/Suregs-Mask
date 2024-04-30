
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

Commerce::Commerce()
{

}

// Destructor
Commerce::~Commerce()
{}


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

bool Commerce::SelectTrade(uint id, bool add)
{
	return false;
}

bool Commerce::SelectAllTrade(uint id, bool add)
{
	return false;
}

bool Commerce::ApplyTrades()
{
	return false;
}

bool Commerce::LoadTextures()
{



	return false;
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






