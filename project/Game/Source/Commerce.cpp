
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
#include "CommerceManager.h"

Commerce::Commerce(uint id) : id(id)
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

	backgroundTexture = app->tex->Load(backgroundPathTexture);
	sellerTexture = app->tex->Load(sellerPathTexture);
	backgroundTradeTexture = app->tex->Load(backgroundTradePathTexture);
	backgroundTradeHoverTexture = app->tex->Load(backgroundTradeHoverPathTexture);
	backgroundSelectAllTexture = app->tex->Load(backgroundSelectAllPathTexture);
	backgroundSelectAllHoverTexture = app->tex->Load(backgroundSelectAllHoverPathTexture);
	backgroundSliderTexture = app->tex->Load(backgroundSliderPathTexture);
	knobSliderTexture = app->tex->Load(knobSliderPathTexture);
	backgroundConfirmTexture = app->tex->Load(backgroundConfirmPathTexture);
	backgroundConfirmHoverTexture = app->tex->Load(backgroundConfirmHoverPathTexture);

	return false;
}

bool Commerce::CloseCommerce()
{
	bool ret = true;

	active = false;

	//Descargar todas las texturas;

	return ret;
}

void Commerce::SetTrades(std::vector<Trade*> trades)
{
	this->trades = trades;
}

uint Commerce::GetId()
{
	return id;
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






