
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
#include "Hud.h"
#include "Audio.h"
#include "Menu.h"
#include "Window.h"

CommerceHerreria::CommerceHerreria(uint id) : Commerce(id)
{

}

// Destructor
CommerceHerreria::~CommerceHerreria()
{}


bool CommerceHerreria::Start() {

	bool ret = true;

	return ret;
}

// Called before quitting
bool CommerceHerreria::CleanUp()
{
	bool ret = true;
	
	return ret;
}

bool CommerceHerreria::SelectTrade(uint id, bool add)
{
	bool ret = true;

	Trade* trade = actualTrades.at(id);

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
		if (trade->quantityTraded > 1) trade->quantityTraded = 1;
		
	}
	else {
		if (trade->quantityTraded > 0) {
			trade->quantityTraded--;
		}
	}
	



	return ret;
}

bool CommerceHerreria::ApplyTrades()
{
	bool ret = true;


	for (int i = 0; i < actualTrades.size(); i++) {

		Trade* trade = actualTrades.at(i);
		if (trade->quantityTraded >= 1)  trade->quantityTraded = 1;

		//Agregar estos items al inventario
		if (trade->type == 0 && trade->quantityTraded >= 1){
			app->inventoryManager->swordLevel++;
		}
		if (trade->type == 1 && trade->quantityTraded >= 1) {
			app->inventoryManager->armorLevel++;
			app->entityManager->GetPlayer()->UpdateStats();
			app->entityManager->GetPlayer()->currentStats.currentHealth = app->entityManager->GetPlayer()->currentStats.maxHealth;
		}
		

		//Eliminar estos items
		for (int j = 0; j < trade->itemsRequested.size(); j++) {       

			Inventity inventity = *trade->itemsRequested.at(j);
			if (trade->quantityTraded > 0) {
				if (inventity.type == InventityType::MONEDA) {
					app->inventoryManager->monedasObtenidas -= trade->quantityRequested.at(j) * trade->quantityTraded;
				}
				else {
					inventity.quantity = trade->quantityRequested.at(j) * trade->quantityTraded;
					app->inventoryManager->DestroyItem(inventity.type, inventity.quantity);
				}
			}
			
			
		}
		trade->quantityTraded = 0;
	}



	return ret;
}

bool CommerceHerreria::LoadTextures()
{
	bool ret = true;
	
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



	backgroundTradeHerreriaTexture = app->tex->Load(backgroundTradeHerreriaPathTexture);
	backgroundTradeHerreriaHoverTexture = app->tex->Load(backgroundTradeHerreriaHoverPathTexture);
	backgroundTradeHerreriaItemTexture = app->tex->Load(backgroundTradeHerreriaItemPathTexture);


	pointerIndexF = 0;
	pointerIndexC = 0;
	scrollY = 0;

	button_fx = app->audio->LoadAudioFx("button_fx");
	sell_item_fx = app->audio->LoadAudioFx("sell_item_fx");
	buy_item_fx = app->audio->LoadAudioFx("buy_item_fx");
	change_inventory_fx = app->audio->LoadAudioFx("change_inventory_fx");
	select_fx = app->audio->LoadAudioFx("select_fx");
	

	return ret;
}

bool CommerceHerreria::CloseCommerce()
{
	bool ret = true;

	active = false;
	

	for (int i = 0; i< actualTrades.size(); i++) {
		actualTrades.at(i)->quantityTraded = 0;
	}

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

	app->tex->UnLoad(backgroundTradeHerreriaTexture);
	app->tex->UnLoad(backgroundTradeHerreriaHoverTexture);
	app->tex->UnLoad(backgroundTradeHerreriaItemTexture);

	app->entityManager->active = true;

	return ret;
}

void CommerceHerreria::SetTrades(std::vector<Trade*> trades)
{
	this->trades = trades;
}

int CommerceHerreria::GetInventoryTradesQuantity(InventityType type)
{
	int cantidad = 0;

	for (int i = 0; i < actualTrades.size(); i++) {

		Trade* trade = actualTrades.at(i);

		for (int j = 0; j < trade->itemsRequested.size(); j++) {
			Inventity* inventity = trade->itemsRequested.at(j);
			if (inventity->type == type) {
				cantidad += trade->quantityRequested.at(j) * trade->quantityTraded;
			}
		}
	}
	

	return cantidad;
}

int CommerceHerreria::GetTotalTradesSelected()
{
	int cantidad = 0;

	for (int i = 0; i < actualTrades.size(); i++) {
		if (actualTrades.at(i)->quantityTraded > 0) {
			cantidad += actualTrades.at(i)->quantityTraded;
		}
	}

	return cantidad;
}

//uint CommerceHerreria::GetId()
//{
//	/*return this->GetId();*/
//}

bool CommerceHerreria::Update(float dt)
{

	bool ret = true;



	//actualTrades
	actualTrades.clear();
	if (app->entityManager->GetPlayer() != nullptr) {
		for (int i = 0; i < trades.size(); i++) {
			if (trades.at(i)->type == 0 && trades.at(i)->level == app->inventoryManager->swordLevel + 1) {
				actualTrades.push_back(trades.at(i));
				continue;
			}

			if (trades.at(i)->type == 1 && trades.at(i)->level == app->inventoryManager->armorLevel + 1) {
				actualTrades.push_back(trades.at(i));
				break;
			}

		}
	}
	else {
		actualTrades.push_back(trades.at(0));
		actualTrades.push_back(trades.at(9));
	}

	

	if (app->input->GetButton(DOWN) == KEY_DOWN) {
		pointerIndexF++;
		if (pointerIndexF >= actualTrades.size() + 2) {
			pointerIndexF = 0;
		}
		app->audio->PlayFx(button_fx);
	}

	if (app->input->GetButton(UP) == KEY_DOWN) {
		pointerIndexF--;
		if (pointerIndexF < 0) {
			pointerIndexF = actualTrades.size() + 1;
		}
		app->audio->PlayFx(button_fx);
	}

	if (app->input->GetButton(SELECT) == KEY_DOWN || app->input->GetButton(CONFIRM) == KEY_DOWN) {

		if (pointerIndexF < actualTrades.size()) {
			if (pointerIndexC == 0) SelectTrade(pointerIndexF);
			if (pointerIndexC == 1) SelectAllTrade(pointerIndexF);
			app->audio->PlayFx(select_fx);
		}
		else {
			//Boton confirmar compra
			if (pointerIndexF == actualTrades.size()) {
				ApplyTrades();
				app->audio->PlayFx(sell_item_fx);
			}
			else if (pointerIndexF == actualTrades.size() + 1) {
				CloseCommerce();
				app->audio->PlayFx(change_inventory_fx);
			}

		}
	}
	if (app->input->GetButton(BACK) == KEY_DOWN) {
		if (pointerIndexC == 0) SelectTrade(pointerIndexF, false);
		if (pointerIndexC == 1) SelectAllTrade(pointerIndexF, false);
		app->audio->PlayFx(select_fx);
	}







	return ret;
}

bool CommerceHerreria::PostUpdate()
{
	bool ret = true;

	uint windowWidth, windowHeight;
	app->win->GetWindowSize(windowWidth, windowHeight);
	Uint8 alpha = 188;  // Valor de transparencia (0-255)
	SDL_Texture* transparentTexture = app->menu->CreateTransparentTexture(app->render->renderer, windowWidth, windowHeight, alpha);
	if (transparentTexture != nullptr) {
		SDL_SetTextureBlendMode(transparentTexture, SDL_BLENDMODE_BLEND);
		SDL_RenderCopy(app->render->renderer, transparentTexture, nullptr, nullptr);
		SDL_DestroyTexture(transparentTexture);
	}

	app->render->DrawTexture(backgroundTexture, positionGeneral.x, positionGeneral.y, 1, SDL_FLIP_NONE, 0, 0);

	int viewportWidth = 0;
	int viewportHeight = 400;

	SDL_Rect viewport = { positionGeneral.x + positionList.x, positionGeneral.y + positionList.y, viewportWidth, viewportHeight };

	//Trades
	for (int i = 0; i < actualTrades.size(); i++) {
			int y = positionGeneral.y + positionList.y + (70 + tradeSpacing * i) - scrollY;
		


			//Fondo
			app->render->DrawTexture(backgroundTradeHerreriaTexture, positionGeneral.x + positionList.x, y, 1, SDL_FLIP_NONE, nullptr, 0, 0);

			Trade* trade = actualTrades.at(i);

			//ItemsOfrecidos
			for (int j = 0; j < trade->itemsOffered.size(); j++) {
				if (trade->itemsOffered.at(j) == NULL) { continue; };
				//Items background
				app->render->DrawTexture(backgroundTradeHerreriaItemTexture, positionGeneral.x + positionList.x + positionInList.x + (itemSpacing * j), y + positionInList.y + 7, 1, SDL_FLIP_NONE, nullptr, 0, 0);

				//Primer item de oferta
				app->render->DrawTexture(trade->itemsOffered.at(j)->icon, positionGeneral.x + positionList.x + positionInList.x - 10 + (itemSpacing * j), y + positionInList.y , 1, SDL_FLIP_NONE, nullptr, 0, 0);

			}

			//Si solo ofrece un item, poner el nombre de este al lado
			if (trade->itemsOffered.size() == 1) {
				app->render->DrawTextBound(trade->itemsOffered.at(0)->name.GetString(), positionGeneral.x + positionList.x + positionInList.x + 80, y + positionInList.y + 20, 200);
			}


			//ItemsRequeridos
			for (int j = 0; j < trade->itemsRequested.size(); j++) {

				//Items background
				app->render->DrawTexture(backgroundTradeItemTexture, positionGeneral.x + positionList.x + positionInList.x + (itemSpacing * j) + itemsRequestedSpacing, y + positionInList.y + 15, 1, SDL_FLIP_NONE, nullptr, 0, 0);

				//Primer item de oferta
				app->render->DrawTexture(trade->itemsRequested.at(j)->icon, positionGeneral.x + positionList.x + positionInList.x - 10 + (itemSpacing * j) + itemsRequestedSpacing, y + positionInList.y + 5, 0.65f, SDL_FLIP_NONE, nullptr, 0, 0);
				
				//Cantidad
				app->render->DrawTextBound(std::to_string(trade->quantityRequested.at(j)).c_str(), positionGeneral.x + positionList.x + positionInList.x + (itemSpacing * j)+ itemsRequestedSpacing + 50, y + positionInList.y + 15, 20);

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
				app->render->DrawTextBound(cantidadObtener.c_str(), positionGeneral.x + positionList.x + positionInList.x + (itemSpacing * j) + itemsRequestedSpacing + 45, y + positionInList.y + 35, 20);

			}


			//Punteros
			if (pointerIndexF == i && pointerIndexC == 0) {
				app->render->DrawTexture(backgroundTradeHerreriaHoverTexture, positionGeneral.x + positionList.x + 6, y+5, 1, SDL_FLIP_NONE, nullptr, 0, 0);
			}
	}

	//Boton confirmar compra
	if (GetTotalTradesSelected() > 0) {
		app->render->DrawTexture(backgroundButtonTexture, positionGeneral.x + positionList.x + 285, positionGeneral.y + viewport.h + 80, 1, SDL_FLIP_NONE, nullptr, 0, 0);
	}
	else {
		app->render->DrawTexture(backgroundButtonDisabledTexture, positionGeneral.x + positionList.x + 285, positionGeneral.y + viewport.h + 80, 1, SDL_FLIP_NONE, nullptr, 0, 0);
	}

	app->render->DrawTextBound("Confirmar compra", positionGeneral.x + positionList.x + 320, positionGeneral.y + viewport.h + 100, 300);

	if (pointerIndexF == actualTrades.size()) {
		app->render->DrawTexture(backgroundButtonHoverTexture, positionGeneral.x + positionList.x + 280, positionGeneral.y + viewport.h + 74, 1, SDL_FLIP_NONE, nullptr, 0, 0);
	}

	//Boton cancelar compra
	app->render->DrawTexture(backgroundButtonTexture, positionGeneral.x + positionList.x + 285, positionGeneral.y + viewport.h + 135, 1, SDL_FLIP_NONE, nullptr, 0, 0);
	app->render->DrawTextBound("Cancelar compra", positionGeneral.x + positionList.x + 320, positionGeneral.y + viewport.h + 155, 300);
	if (pointerIndexF == actualTrades.size()+1) {
		app->render->DrawTexture(backgroundButtonHoverTexture, positionGeneral.x + positionList.x + 280, positionGeneral.y + viewport.h + 129, 1, SDL_FLIP_NONE, nullptr, 0, 0);
	}

	//Descripcion item seleccionado
	app->render->DrawTexture(backgroundDescriptionTexture, positionGeneral.x + positionList.x - 200 , positionGeneral.y + viewport.h + 95, 1, SDL_FLIP_NONE, nullptr, 0, 0);
	if (pointerIndexF < actualTrades.size()) {
		app->render->DrawTextBound(actualTrades.at(pointerIndexF)->itemsOffered.at(0)->desc.c_str(), positionGeneral.x + positionList.x + 10 - 150, positionGeneral.y + viewport.h + 100, 400);
	}

	//Fondo Mondes
	app->render->DrawTexture(backgroundMoneyTexture, positionGeneral.x + 970, positionGeneral.y + 30, 1, SDL_FLIP_NONE, 0, 0);
	app->render->DrawTextBound(std::to_string(app->inventoryManager->monedasObtenidas).c_str(), positionGeneral.x + 990, positionGeneral.y + 44, 60);


	//Npc
	app->render->DrawTexture(npcTexture, npcPosition.x-20, npcPosition.y+20, 1, SDL_FLIP_NONE, 0, 0);
	
	return ret;
}






