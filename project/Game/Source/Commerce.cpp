
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

	pointerIndex = 0;

	return false;
}

bool Commerce::CloseCommerce()
{
	bool ret = true;

	active = false;

	//Descargar todas las texturas;
	app->tex->UnLoad(backgroundTexture);
	app->tex->UnLoad(sellerTexture);
	app->tex->UnLoad(backgroundTradeTexture);
	app->tex->UnLoad(backgroundTradeHoverTexture);
	app->tex->UnLoad(backgroundSelectAllTexture);
	app->tex->UnLoad(backgroundSelectAllHoverTexture);
	app->tex->UnLoad(backgroundSliderTexture);
	app->tex->UnLoad(knobSliderTexture);
	app->tex->UnLoad(backgroundConfirmTexture);
	app->tex->UnLoad(backgroundConfirmHoverTexture);

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

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
		pointerIndex++;
		if (pointerIndex >= trades.size()) {
			pointerIndex = 0;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		pointerIndex--;
		if (pointerIndex < 0) {
			pointerIndex = trades.size()-1;
		}
	}



	return ret;
}

bool Commerce::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(backgroundTexture, positionGeneral.x, positionGeneral.y, 1, SDL_FLIP_NONE, 0, 0);

	int viewportWidth = 0;
	int viewportHeight = 40000;

	SDL_Rect viewport = { positionGeneral.x + positionList.x, positionGeneral.y + positionList.y, viewportWidth, viewportHeight };


	for (int i = 0; i < trades.size(); i++) {
		int y = positionGeneral.y + positionList.y + (70 + tradeSpacing * i);
		if (y >= viewport.y && y <= viewport.y + viewport.h) {

			app->render->DrawTexture(backgroundTradeTexture, positionGeneral.x + positionList.x, y, 1, SDL_FLIP_NONE, nullptr, 0, 0);

			if (pointerIndex == i) {
				app->render->DrawTexture(backgroundTradeHoverTexture, positionGeneral.x + positionList.x -3, y -2, 1, SDL_FLIP_NONE, nullptr, 0, 0);
			}

		}
	}



	
	return ret;
}






