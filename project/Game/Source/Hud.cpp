﻿#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiCheckBox.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "EntityManager.h"
#include "InventoryManager.h"
#include "Player.h"
#include "Hud.h"
#include "Menu.h"
#include "Scene_Menu.h"
#include "Scene_Testing.h"
#include "ModuleFadeToBlack.h"

Hud::Hud(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name = ("hud");
}

// Destructor
Hud::~Hud()
{}

// Called before render is available
bool Hud::Awake(pugi::xml_node config)
{

	bool ret = true;


	hudTexturePath = (char*)config.child("mainTexture").attribute("texturepath").as_string();
	messageTexturePath = (char*)config.child("messageTexture").attribute("texturepath").as_string();
	
	rectBarraVida = new SDL_Rect{ 269,6,259,16 };
	rectFondoBarraVida = new SDL_Rect{ 0,2,267,23 };

	rectFondoMascara = new SDL_Rect{ 0,37,101,102 };
	rectFondoMascaraSecundaria = new SDL_Rect{ 110,43,90,90 };

	rectFondoPociones = new SDL_Rect{ 110,43,90,90 };
	rectPocionVida1 = new SDL_Rect{ 1,686,60,56 };
	rectPocionVida2 = new SDL_Rect{ 66,686,60,56 };
	rectPocionVida3 = new SDL_Rect{ 131,686,60,56 };
	rectPocionVidaMax = new SDL_Rect{ 196,686,60,56 };
	rectPocionRegeneracion = new SDL_Rect{ 261,686,60,56 };
	rectPocionDano = new SDL_Rect{ 326,686,60,56 };
	rectPocionVelocidad = new SDL_Rect{ 391,686,62,65 };
	rectPocionOrbe = new SDL_Rect{ 456,686,62,65 };	

	potionRectMap[InventityType::POCION_VIDA_1] = rectPocionVida1;
	potionRectMap[InventityType::POCION_VIDA_2] = rectPocionVida2;
	potionRectMap[InventityType::POCION_VIDA_3] = rectPocionVida3;
	potionRectMap[InventityType::POCION_VIDA_MAX] = rectPocionVidaMax;
	potionRectMap[InventityType::POCION_REGENERACION] = rectPocionRegeneracion;
	potionRectMap[InventityType::POCION_DANO] = rectPocionDano;
	potionRectMap[InventityType::POCION_VELOCIDAD] = rectPocionVelocidad;
	potionRectMap[InventityType::ORBE_MAGICO] = rectPocionOrbe;

	//rectFondoHabilidad1 = new SDL_Rect{ 1,149,60,60 };
	//rectFondoHabilidad2 = new SDL_Rect{ 65,157,45,45 };
	rectFondoInventario = new SDL_Rect{ 2,220,109,109 };
	rectFondoMonedas = new SDL_Rect{ 0,341,96,30 };
	rectFondoObjetosConseguidos = new SDL_Rect{ 0,422,357,41 };

	rectMascaraNoMask = new SDL_Rect{ 0,0,1,1 };
	rectMascara0 = new SDL_Rect{ 3,499,100,100 };
	rectMascara1 = new SDL_Rect{ 105,499,100,100 };
	rectMascara2 = new SDL_Rect{ 207,499,100,100 };
	rectMascara3 = new SDL_Rect{ 309,499,0,0 };
	rectMascara4 = new SDL_Rect{ 411,499,0,0 };

	rectBotonPlaceholder = new SDL_Rect{ 0,619,45,45 };
	rectBotonTAB = new SDL_Rect{ 48,619,45,45 };
	rectBotonQ = new SDL_Rect{ 96,619,45,45 };

	return ret;
}

// Called before the first frame
bool Hud::Start()
{
	hudTexture = app->tex->Load(hudTexturePath);
	messageTexture = app->tex->Load(messageTexturePath);

	/*Acquired_Item ai;
	ai.lifeTimer.Start();
	ai.text = "holaaa";

	acquired_Items.push_back(ai);
	acquired_Items.push_back(ai);
	acquired_Items.push_back(ai);*/


	return true;
}

// Called each loop iteration
bool Hud::PreUpdate()
{
	
	
	return true;
}

// Called each loop iteration
bool Hud::Update(float dt)
{


	//Item Acquired
	for (int i = 0; i < acquired_Items.size(); i++) {

		if (acquired_Items.at(i)->lifeTimer.ReadMSec() >= acuiredItemLifeTimeMS) {

			Acquired_Item* acquiredItem = acquired_Items.at(i);
			auto iter = acquired_Items.begin() + i;

			acquired_Items.erase(iter);

			
			SDL_SetTextureAlphaMod(acquiredItem->texture, 255); // Ajusta la opacidad

			delete acquiredItem;

		}
		

	}	

	Potions();

	return true;
}
bool fade = false;
// Called each loop iteration
bool Hud::PostUpdate()
{

	if (app->entityManager->GetPlayer() == nullptr) return true;

	app->win->GetWindowSize(windowWidth, windowHeight);
	


	//Barra de vida
	app->render->DrawTexture(hudTexture, 170, 40, SDL_FLIP_NONE, rectFondoBarraVida, 0);

	int rectW = rectFondoBarraVida->w;
	rectW = (rectFondoBarraVida->w * app->entityManager->GetPlayer()->currentStats.currentHealth) / app->entityManager->GetPlayer()->currentStats.maxHealth;

	SDL_Rect* rectBarraVidaCalculado = new SDL_Rect{ rectBarraVida->x, rectBarraVida->y, rectW, rectBarraVida->h };

	//LOG("Vida player: %d, max vida player: %d, rect: %d", app->entityManager->GetPlayer()->health, app->entityManager->GetPlayer()->maxHealth, rectW);

	app->render->DrawTexture(hudTexture, 175, 44, SDL_FLIP_NONE, rectBarraVidaCalculado, 0);
	
	//Monedas
	std::string quantityStr = std::to_string(app->inventoryManager->monedasObtenidas);
	
	app->render->DrawTexture(hudTexture, windowWidth - rectFondoMonedas->w - 35, 130, SDL_FLIP_NONE, rectFondoMonedas, 0);
	app->render->DrawText(quantityStr.c_str(), windowWidth - rectFondoMonedas->w + 10, 140, 18, 18);
	//Fondos

	//Pociones

	app->render->DrawTexture(hudTexture, 445, 10, SDL_FLIP_NONE, rectFondoPociones, 0);

	if (!potionRects.empty()) {
		app->render->DrawTexture(hudTexture, 458, 24, SDL_FLIP_NONE, potionRects[selectedPotionIndex], 0);
	}

	/*app->render->DrawTexture(hudTexture, 45, 150, SDL_FLIP_NONE, rectFondoHabilidad1, 0);
	app->render->DrawTexture(hudTexture, 51, 220, SDL_FLIP_NONE, rectFondoHabilidad2, 0);
	*/
	app->render->DrawTexture(hudTexture, windowWidth - rectFondoInventario->w - 20, 15, SDL_FLIP_NONE, rectFondoInventario, 0);



	//Objeto conseguido
	//app->render->DrawTexture(hudTexture, 50, 50, SDL_FLIP_NONE, rectFondoObjetosConseguidos, 0);


	//Mascaras
	Mask* primaryMask = app->entityManager->GetPlayer()->GetPrimaryMask();
	Mask* secondaryMask = app->entityManager->GetPlayer()->GetSecondaryMask();

	SDL_Rect rectPrimaryMask;
	SDL_Rect rectSecondaryMask;

	switch (*primaryMask)	
	{
		case Mask::NOMASK: rectPrimaryMask = *rectMascaraNoMask; break;
		case Mask::MASK0: rectPrimaryMask = *rectMascara0; break;
		case Mask::MASK1: rectPrimaryMask = *rectMascara1; break;
		case Mask::MASK2: rectPrimaryMask = *rectMascara2; break;
		case Mask::MASK3: rectPrimaryMask = *rectMascara3; break;
		default: rectPrimaryMask = *rectMascaraNoMask; break;
	}

	switch (*secondaryMask)
	{
		case Mask::NOMASK: rectSecondaryMask = *rectMascaraNoMask; break;
		case Mask::MASK0: rectSecondaryMask = *rectMascara0; break;
		case Mask::MASK1: rectSecondaryMask = *rectMascara1; break;
		case Mask::MASK2: rectSecondaryMask = *rectMascara2; break;
		case Mask::MASK3: rectSecondaryMask = *rectMascara3; break;
		default: rectSecondaryMask = *rectMascaraNoMask; break;
	}


	app->render->DrawTexture(hudTexture, 75, 15, SDL_FLIP_NONE, rectFondoMascaraSecundaria, 0);
	app->render->DrawTexture(hudTexture, 75, 15, 0.79f,SDL_FLIP_NONE, &rectSecondaryMask, 0);

	app->render->DrawTexture(hudTexture, 25, 40, SDL_FLIP_NONE, rectFondoMascara, 0);
	app->render->DrawTexture(hudTexture, 25, 40, SDL_FLIP_NONE, &rectPrimaryMask, 0);

	//Fin mascaras



	//Botones
	app->render->DrawTexture(hudTexture, windowWidth - rectFondoInventario->w + 10, 70, SDL_FLIP_NONE, rectBotonTAB, 0);
	app->render->DrawTexture(hudTexture, 120, 85, SDL_FLIP_NONE, rectBotonQ, 0);


	//Item Acquired
	for (int i = 0; i < acquired_Items.size(); i++) {

		

		if (acquired_Items.at(i)->lifeTimer.ReadMSec() <= 200) {
			//float alpha = (200 - acquired_Items.at(i)->lifeTimer.ReadMSec()) / 200;
			//float alpha = (200 - acquired_Items.at(i)->lifeTimer.ReadMSec()) / 200;
			float alpha = acquired_Items.at(i)->lifeTimer.ReadMSec() / 200;
			

			SDL_SetTextureAlphaMod(hudTexture, static_cast<Uint8>(255 * alpha)); // Ajusta la opacidad
			SDL_SetTextureAlphaMod(acquired_Items.at(i)->texture, static_cast<Uint8>(255 * alpha)); // Ajusta la opacidad

		}
		else if (acquired_Items.at(i)->lifeTimer.ReadMSec() >= 1300) {
			float alpha = 1.0f - (acquired_Items.at(i)->lifeTimer.ReadMSec() - 1300) / (acuiredItemLifeTimeMS - 1300);
			
			SDL_SetTextureAlphaMod(hudTexture, static_cast<Uint8>(255 * alpha)); // Ajusta la opacidad
			SDL_SetTextureAlphaMod(acquired_Items.at(i)->texture, static_cast<Uint8>(255 * alpha)); // Ajusta la opacidad
		
		}
		else {
			SDL_SetTextureAlphaMod(hudTexture, 255);
			SDL_SetTextureAlphaMod(acquired_Items.at(i)->texture, 255);
		}



		app->render->DrawTexture(hudTexture, 950, 600 - (i * 50), SDL_FLIP_NONE, rectFondoObjetosConseguidos, 0);
		app->render->DrawTexture(acquired_Items.at(i)->texture, 1000, 575 - (i * 50), SDL_FLIP_NONE, 0, 0);
		app->render->DrawTextBound(acquired_Items.at(i)->text.c_str(), 1080, 605 - (i * 50), 200);

	}

	SDL_SetTextureAlphaMod(hudTexture, 255);

	//Me da miedo preguntar que es esto de aqui abajo, solo sabe dios que hace.
	

	return true;
}

// Called before quitting
bool Hud::CleanUp()
{
	LOG("Freeing Hud");



	 delete rectBarraVida;
	 delete rectFondoBarraVida;

	 delete rectFondoMascara;
	 delete rectFondoMascaraSecundaria;

	 delete rectFondoPociones;
	 delete rectPocionVida1;
	 delete rectPocionVida2;
	 delete rectPocionVida3;
	 delete rectPocionVidaMax;
	 delete rectPocionRegeneracion;
	 delete rectPocionDano;
	 delete rectPocionVelocidad;
	 delete rectPocionOrbe;
	 potionRects.clear();
	 potionRectMap.clear();

	 //delete rectFondoHabilidad1;
	 //delete rectFondoHabilidad2;
	 delete rectFondoInventario;
	 delete rectFondoMonedas;
	 delete rectFondoObjetosConseguidos;

	 delete rectMascara0;
	 delete rectMascara1;
	 delete rectMascara2;
	 delete rectMascara3;
	 delete rectMascara4;

	 delete rectBotonPlaceholder;
	 delete rectBotonTAB;
	 delete rectBotonQ;

	 app->tex->UnLoad(hudTexture);

	return true;
}

void Hud::PopUpMessage()
{
	 
}

void Hud::AcquiredItemTrigger(SDL_Texture* texture, std::string text)
{
	Acquired_Item* acquiredItem = new Acquired_Item{texture, text};
	acquiredItem->lifeTimer.Start();

	acquired_Items.push_back(acquiredItem);
}

void Hud::Potions()
{

	if (!potionRects.empty()) {
		if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
		{
			/*printf("Potion Used: %d\n", selectedPotionIndex);*/
			ListItem<Inventity*>* item = app->inventoryManager->inventities.At(selectedPotionIndex);
			app->inventoryManager->UsePotionSelected(item);
		}
		else if (app->input->GetButton(CAMBIAR_POCION_RIGHT) == KEY_DOWN && selectedPotionIndex < potionRects.size() - 1) 
		{
			/*printf("selectedPotionIndex: %d\n", selectedPotionIndex);*/
			selectedPotionIndex++;
		}
		else if (app->input->GetButton(CAMBIAR_POCION_LEFT) == KEY_DOWN && selectedPotionIndex > 0) 
		{
			/*printf("selectedPotionIndex: %d\n", selectedPotionIndex);*/
			selectedPotionIndex--;
		}
	}


	potionRects.clear();

	ListItem<Inventity*>* item;
	for (item = app->inventoryManager->inventities.start; item != NULL; item = item->next) {
		Inventity* inventity = item->data;
		if (inventity->type == InventityType::POCION_VIDA_1 || inventity->type == InventityType::POCION_VIDA_2 || inventity->type == InventityType::POCION_VIDA_3 ||
			inventity->type == InventityType::POCION_VIDA_MAX || inventity->type == InventityType::POCION_REGENERACION || inventity->type == InventityType::POCION_DANO ||
			inventity->type == InventityType::POCION_VELOCIDAD || inventity->type == InventityType::ORBE_MAGICO) {
			potionRects.push_back(potionRectMap[inventity->type]);
		}
	}
}

