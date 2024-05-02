
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
#include "InventoryManager.h"

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
	bool ret = true;

	Trade* trade = trades.at(id);

	if (add) {
		//Comprovar que se pueda
		bool canBuy = true;

		for (int i = 0; i < trade->itemsRequested.size(); i++) {
			int cantidadEnInventario;
			if (trade->itemsRequested.at(i)->type == InventityType::MONEDA) {
				cantidadEnInventario = app->inventoryManager->monedasObtenidas;
			}
			else {
				cantidadEnInventario = app->inventoryManager->GetInventityQuantity(trade->itemsRequested.at(i)->type);
			}
		   
			int cantidadEnActivoTodosTrades = GetInventoryTradesQuantity(trade->itemsRequested.at(i)->type);

			int calculoCantidad = (cantidadEnInventario - cantidadEnActivoTodosTrades);
			calculoCantidad -= trade->quantityRequested.at(i);

			if (calculoCantidad < 0) {
				canBuy = false;
			}

		}

		if(canBuy) trade->quantityTraded++;
		
	}
	else {
		if (trade->quantityTraded > 0) {
			trade->quantityTraded--;
		}
	}
	



	return ret;
}

bool Commerce::SelectAllTrade(uint id, bool add)
{
	bool ret = true;

	Trade* trade = trades.at(id);

	if (add) {

		bool canBuy = true;

		while (canBuy) {
			for (int i = 0; i < trade->itemsRequested.size(); i++) {

				int cantidadEnInventario;

				if (trade->itemsRequested.at(i)->type == InventityType::MONEDA) {
					cantidadEnInventario = app->inventoryManager->monedasObtenidas;
				}
				else {
					cantidadEnInventario = app->inventoryManager->GetInventityQuantity(trade->itemsRequested.at(i)->type);
				}

				
				int cantidadEnActivoTodosTrades = GetInventoryTradesQuantity(trade->itemsRequested.at(i)->type);

				int calculoCantidad = (cantidadEnInventario - cantidadEnActivoTodosTrades);
				calculoCantidad -= trade->quantityRequested.at(i);

				if (calculoCantidad < 0) {
					canBuy = false;
				}

			}

			if (canBuy) trade->quantityTraded++;
		}
	}
	else {
		trade->quantityTraded = 0;
	}

	return ret;
}

bool Commerce::ApplyTrades()
{
	bool ret = true;


	for (int i = 0; i < trades.size(); i++) {

		Trade* trade = trades.at(i);

		//Agregar estos items al inventario
		for (int j = 0; j < trade->itemsOffered.size(); j++) {
			Inventity inventity = *trade->itemsOffered.at(j);

			if (inventity.type == InventityType::MONEDA) {
				app->inventoryManager->monedasObtenidas += trade->quantityOffered.at(j) * trade->quantityTraded;
			}
			else {
				inventity.quantity = trade->quantityOffered.at(j);
				app->inventoryManager->AddItem(&inventity);
			}

			
		}

		//Eliminar estos items
		for (int j = 0; j < trade->itemsRequested.size(); j++) {       

			Inventity inventity = *trade->itemsRequested.at(j);
			if (inventity.type == InventityType::MONEDA) {
				app->inventoryManager->monedasObtenidas -= trade->quantityRequested.at(j) * trade->quantityTraded;
			}
			else {
				inventity.quantity = trade->quantityRequested.at(j) * trade->quantityTraded;
				app->inventoryManager->DestroyItem(inventity.type, inventity.quantity);
			}
			trade->quantityTraded = 0;
			
		}

		


	}



	return ret;
}

bool Commerce::LoadTextures()
{

	backgroundTexture = app->tex->Load(backgroundPathTexture);
	backgroundTradeTexture = app->tex->Load(backgroundTradePathTexture);
	backgroundTradeHoverTexture = app->tex->Load(backgroundTradeHoverPathTexture);
	backgroundSelectAllTexture = app->tex->Load(backgroundSelectAllPathTexture);
	backgroundSelectAllHoverTexture = app->tex->Load(backgroundSelectAllHoverPathTexture);
	backgroundSliderTexture = app->tex->Load(backgroundSliderPathTexture);
	knobSliderTexture = app->tex->Load(knobSliderPathTexture);
	backgroundConfirmTexture = app->tex->Load(backgroundConfirmPathTexture);
	backgroundConfirmHoverTexture = app->tex->Load(backgroundConfirmHoverPathTexture);

	backgroundTradeItemTexture = app->tex->Load(backgroundTradeItemPathTexture);

	backgroundButtonTexture = app->tex->Load(backgroundButtonPathTexture);
	backgroundButtonHoverTexture = app->tex->Load(backgroundButtonHoverPathTexture);
	backgroundButtonDisabledTexture = app->tex->Load(backgroundButtonDisabledPathTexture);
	backgroundMoneyTexture = app->tex->Load(backgroundMoneyPathTexture);

	backgroundDescriptionTexture = app->tex->Load(backgroundDescriptionPathTexture);

	npcTexture = app->tex->Load(npcPathTexture);

	pointerIndexF = 0;
	scrollY = 0;

	return false;
}

bool Commerce::CloseCommerce()
{
	bool ret = true;

	active = false;

	//Descargar todas las texturas;
	app->tex->UnLoad(backgroundTexture);
	app->tex->UnLoad(backgroundTradeTexture);
	app->tex->UnLoad(backgroundTradeHoverTexture);
	app->tex->UnLoad(backgroundSelectAllTexture);
	app->tex->UnLoad(backgroundSelectAllHoverTexture);
	app->tex->UnLoad(backgroundSliderTexture);
	app->tex->UnLoad(knobSliderTexture);
	app->tex->UnLoad(backgroundConfirmTexture);
	app->tex->UnLoad(backgroundConfirmHoverTexture);
	app->tex->UnLoad(backgroundTradeItemTexture);
	app->tex->UnLoad(backgroundButtonTexture);
	app->tex->UnLoad(backgroundButtonHoverTexture);
	app->tex->UnLoad(backgroundButtonDisabledTexture);
	app->tex->UnLoad(backgroundMoneyTexture);
	app->tex->UnLoad(backgroundDescriptionTexture);
	app->tex->UnLoad(npcTexture);


	return ret;
}

void Commerce::SetTrades(std::vector<Trade*> trades)
{
	this->trades = trades;
}

int Commerce::GetInventoryTradesQuantity(InventityType type)
{
	int cantidad = 0;

	for (int i = 0; i < trades.size(); i++) {

		Trade* trade = trades.at(i);

		for (int j = 0; j < trade->itemsRequested.size(); j++) {
			Inventity* inventity = trade->itemsRequested.at(j);
			if (inventity->type == type) {
				cantidad += trade->quantityRequested.at(j) * trade->quantityTraded;
			}
		}
	}
	

	return cantidad;
}

int Commerce::GetTotalTradesSelected()
{
	int cantidad = 0;

	for (int i = 0; i < trades.size(); i++) {
		if (trades.at(i)->quantityTraded > 0) {
			cantidad += trades.at(i)->quantityTraded;
		}
	}

	return cantidad;
}

uint Commerce::GetId()
{
	return id;
}

bool Commerce::Update(float dt)
{

	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
		pointerIndexF++;
		if (pointerIndexF >= trades.size() + 2) {
			pointerIndexF = 0;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		pointerIndexF--;
		if (pointerIndexF < 0) {
			pointerIndexF = trades.size() + 1;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) {
		pointerIndexC++;
		if(pointerIndexC > 1) {
			pointerIndexC = 0;
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) {
		pointerIndexC--;
		if (pointerIndexC < 0) {
			pointerIndexC = 1;
		}
	}


	if (app->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN) {

		if (pointerIndexF < trades.size()) {
			if (pointerIndexC == 0) SelectTrade(pointerIndexF);
			if (pointerIndexC == 1) SelectAllTrade(pointerIndexF);
		}
		else {
			//Boton confirmar compra
			if (pointerIndexF == trades.size()) {
				ApplyTrades();
			}
			else if (pointerIndexF == trades.size() + 1) {
				CloseCommerce();
			}

		}
	}
	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
		if (pointerIndexC == 0) SelectTrade(pointerIndexF, false);
		if (pointerIndexC == 1) SelectAllTrade(pointerIndexF, false);
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
				app->render->DrawTexture(trade->itemsOffered.at(j)->icon, positionGeneral.x + positionList.x + positionInList.x - 10 + (itemSpacing * j), y + positionInList.y - 10, 0.65f, SDL_FLIP_NONE, nullptr, 0, 0);

				//Cantidad
				app->render->DrawTextBound(std::to_string(trade->quantityOffered.at(j)).c_str(), positionGeneral.x + positionList.x + positionInList.x + (itemSpacing * j) + 60, y + positionInList.y, 20);
			
				//Cantidad a obtener:
				std::string cantidadObtener = "(" + std::to_string((trade->quantityOffered.at(j) * trade->quantityTraded)) + ")";
				app->render->DrawTextBound(cantidadObtener.c_str(), positionGeneral.x + positionList.x + positionInList.x + (itemSpacing * j) + 55, y + positionInList.y + 20, 20);

			
			}

			//Si solo ofrece un item, poner el nombre de este al lado
			if (trade->itemsOffered.size() == 1) {
				
				app->render->DrawTextBound(trade->itemsOffered.at(0)->name.GetString(), positionGeneral.x + positionList.x + positionInList.x + 100, y + positionInList.y + 0, 200);

			}


			//ItemsRequeridos
			for (int j = 0; j < trade->itemsRequested.size(); j++) {

				//Items background
				app->render->DrawTexture(backgroundTradeItemTexture, positionGeneral.x + positionList.x + positionInList.x + (itemSpacing * j) + itemsRequestedSpacing, y + positionInList.y, 1, SDL_FLIP_NONE, nullptr, 0, 0);

				//Primer item de oferta
				app->render->DrawTexture(trade->itemsRequested.at(j)->icon, positionGeneral.x + positionList.x + positionInList.x - 10 + (itemSpacing * j) + itemsRequestedSpacing, y + positionInList.y - 10, 0.65f, SDL_FLIP_NONE, nullptr, 0, 0);
				
				//Cantidad
				app->render->DrawTextBound(std::to_string(trade->quantityRequested.at(j)).c_str(), positionGeneral.x + positionList.x + positionInList.x + (itemSpacing * j)+ itemsRequestedSpacing + 50, y + positionInList.y, 20);

				//Cantidad inventario:
				int calculoCantidad = 0;
				int cantidadEnActivoTodosTrades = GetInventoryTradesQuantity(trade->itemsRequested.at(j)->type);
				int cantidadEnInventario = 0;
				if (trade->itemsRequested.at(j)->type == InventityType::MONEDA){
					cantidadEnInventario = app->inventoryManager->monedasObtenidas;
				}
				else {
					cantidadEnInventario = app->inventoryManager->GetInventityQuantity(trade->itemsRequested.at(j)->type);

				}
				calculoCantidad = cantidadEnInventario - cantidadEnActivoTodosTrades;
				
				std::string cantidadObtener = "(" + std::to_string(calculoCantidad) + ")";
				app->render->DrawTextBound(cantidadObtener.c_str(), positionGeneral.x + positionList.x + positionInList.x + (itemSpacing * j) + itemsRequestedSpacing + 55, y + positionInList.y + 20, 20);

			}


			//Select all
			app->render->DrawTexture(backgroundSelectAllTexture, positionGeneral.x + positionList.x + 675, y, 1, SDL_FLIP_NONE, nullptr, 0, 0);
			

			//Punteros
			if (pointerIndexF == i && pointerIndexC == 0) {
				app->render->DrawTexture(backgroundTradeHoverTexture, positionGeneral.x + positionList.x -3, y -2, 1, SDL_FLIP_NONE, nullptr, 0, 0);
			}
			else if (pointerIndexF == i && pointerIndexC == 1) {
				app->render->DrawTexture(backgroundSelectAllHoverTexture, positionGeneral.x + positionList.x - 3 + 677, y - 1, 1, SDL_FLIP_NONE, nullptr, 0, 0);
			}

		}
		else {
			//Se sale del viewport, mover el scroll
			
			if (pointerIndexF == i) {

				int targetY = positionGeneral.y + positionList.y + (57 + (tradeSpacing * (pointerIndexF - 4)));
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

	//Boton confirmar compra
	if (GetTotalTradesSelected() > 0) {
		app->render->DrawTexture(backgroundButtonTexture, positionGeneral.x + positionList.x + 485, positionGeneral.y + viewport.h + 80, 1, SDL_FLIP_NONE, nullptr, 0, 0);
	}
	else {
		app->render->DrawTexture(backgroundButtonDisabledTexture, positionGeneral.x + positionList.x + 485, positionGeneral.y + viewport.h + 80, 1, SDL_FLIP_NONE, nullptr, 0, 0);
	}

	app->render->DrawTextBound("Confirmar compra", positionGeneral.x + positionList.x + 520, positionGeneral.y + viewport.h + 100, 300);

	if (pointerIndexF == trades.size()) {
		app->render->DrawTexture(backgroundButtonHoverTexture, positionGeneral.x + positionList.x + 480, positionGeneral.y + viewport.h + 74, 1, SDL_FLIP_NONE, nullptr, 0, 0);
	}

	//Boton cancelar compra
	app->render->DrawTexture(backgroundButtonTexture, positionGeneral.x + positionList.x + 485, positionGeneral.y + viewport.h + 135, 1, SDL_FLIP_NONE, nullptr, 0, 0);
	app->render->DrawTextBound("Cancelar compra", positionGeneral.x + positionList.x + 520, positionGeneral.y + viewport.h + 155, 300);
	if (pointerIndexF == trades.size()+1) {
		app->render->DrawTexture(backgroundButtonHoverTexture, positionGeneral.x + positionList.x + 480, positionGeneral.y + viewport.h + 129, 1, SDL_FLIP_NONE, nullptr, 0, 0);
	}

	//Descripcion item seleccionado
	app->render->DrawTexture(backgroundDescriptionTexture, positionGeneral.x + positionList.x , positionGeneral.y + viewport.h + 95, 1, SDL_FLIP_NONE, nullptr, 0, 0);
	if (pointerIndexF < trades.size()) {
		app->render->DrawTextBound(trades.at(pointerIndexF)->itemsOffered.at(0)->desc.c_str(), positionGeneral.x + positionList.x + 10, positionGeneral.y + viewport.h + 100, 500);
	}


	//Slider scroll texture
	app->render->DrawTexture(backgroundSliderTexture, positionGeneral.x + positionList.x + 760, positionGeneral.y + positionList.y + 80, 1, SDL_FLIP_NONE, nullptr, 0, 0);
	int calculoAltura;
	if (pointerIndexF < trades.size()) {
		calculoAltura = ((320 * pointerIndexF) / (trades.size()) + 80);
	}
	else {
		calculoAltura = 400;
	}
	app->render->DrawTexture(knobSliderTexture, positionGeneral.x + positionList.x + 760, positionGeneral.y + positionList.y + calculoAltura, 1, SDL_FLIP_NONE, nullptr, 0, 0);


	//Fondo Mondes
	app->render->DrawTexture(backgroundMoneyTexture, positionGeneral.x + 970, positionGeneral.y + 30, 1, SDL_FLIP_NONE, 0, 0);
	app->render->DrawTextBound(std::to_string(app->inventoryManager->monedasObtenidas).c_str(), positionGeneral.x + 990, positionGeneral.y + 44, 60);


	//Npc
	app->render->DrawTexture(npcTexture, npcPosition.x-20, npcPosition.y+20, 1, SDL_FLIP_NONE, 0, 0);
	
	return ret;
}






