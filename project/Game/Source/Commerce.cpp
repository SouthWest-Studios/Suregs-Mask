
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

	backgroundTradeItemTexture = app->tex->Load(backgroundTradeItemPathTexture);

	pointerIndex = 0;
	scrollY = 0;

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
	app->tex->UnLoad(backgroundTradeItemTexture);

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
	int viewportHeight = 400;

	SDL_Rect viewport = { positionGeneral.x + positionList.x, positionGeneral.y + positionList.y, viewportWidth, viewportHeight };

	//Trades
	for (int i = 0; i < trades.size(); i++) {
		int y = positionGeneral.y + positionList.y + (70 + tradeSpacing * i) - scrollY;
		if (y >= viewport.y && y <= viewport.y + viewport.h) {


			//Fondo
			app->render->DrawTexture(backgroundTradeTexture, positionGeneral.x + positionList.x, y, 1, SDL_FLIP_NONE, nullptr, 0, 0);

			Trade* trade = trades.at(i);

			//ItemsOfrecidos
			for (int j = 0; j < trade->itemsOffered.size(); j++) {
				if (trade->itemsOffered.at(j) == NULL) { continue; };
				//Items background
				app->render->DrawTexture(backgroundTradeItemTexture, positionGeneral.x + positionList.x + positionInList.x + (itemSpacing * j), y + positionInList.y, 1, SDL_FLIP_NONE, nullptr, 0, 0);

				//Primer item de oferta
				app->render->DrawTexture(trade->itemsOffered.at(j)->icon, positionGeneral.x + positionList.x + positionInList.x - 25 + (itemSpacing * j), y + positionInList.y - 25, 1, SDL_FLIP_NONE, nullptr, 0, 0);

				//Cantidad
				app->render->DrawTextBound(std::to_string(trade->quantityOffered.at(j)).c_str(), positionGeneral.x + positionList.x + positionInList.x + (itemSpacing * j) + 50, y + positionInList.y, 20);
			}


			//ItemsRequeridos
			for (int j = 0; j < trade->itemsRequested.size(); j++) {

				//Items background
				app->render->DrawTexture(backgroundTradeItemTexture, positionGeneral.x + positionList.x + positionInList.x + (itemSpacing * j) + itemsRequestedSpacing, y + positionInList.y, 1, SDL_FLIP_NONE, nullptr, 0, 0);

				//Primer item de oferta
				app->render->DrawTexture(trade->itemsRequested.at(j)->icon, positionGeneral.x + positionList.x + positionInList.x - 25 + (itemSpacing * j) + itemsRequestedSpacing, y + positionInList.y - 25, 1, SDL_FLIP_NONE, nullptr, 0, 0);
				
				//Cantidad
				app->render->DrawTextBound(std::to_string(trade->quantityRequested.at(j)).c_str(), positionGeneral.x + positionList.x + positionInList.x + (itemSpacing * j)+ itemsRequestedSpacing + 50, y + positionInList.y, 20);

			}




			if (pointerIndex == i) {
				app->render->DrawTexture(backgroundTradeHoverTexture, positionGeneral.x + positionList.x -3, y -2, 1, SDL_FLIP_NONE, nullptr, 0, 0);
			}

		}
		else {
			//Se sale del viewport, mover el scroll
			
			if (pointerIndex == i) {

				int targetY = positionGeneral.y + positionList.y + (57 + (tradeSpacing * (pointerIndex - 4)));
				if (scrollY - targetY == 400) {
					scrollY -= 100;
				}
				else {
					scrollY = targetY;
				}
				scrollY = std::max(0, scrollY);
			}

		}
	}



	
	return ret;
}






