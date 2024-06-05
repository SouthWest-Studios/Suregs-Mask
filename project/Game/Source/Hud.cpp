#include "App.h"
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
#include "ElevatorMenu.h"
#include "QuestManager.h"
#include "Scene_Pueblo.h"
#include "Scene_Pueblo_Tutorial.h"
#include "Utils.cpp"
#include "Scene_Mazmorra0.h"

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
	cdMaskTexturePath = (char*)config.child("cdMaskTexture").attribute("texturepath").as_string();

	levelUpTexturePath = (char*)config.child("levelUpTexture").attribute("texturepath").as_string();
	maskZeroTexturePath = (char*)config.child("maskZeroTexture").attribute("texturepath").as_string();
	maskOneTexturePath = (char*)config.child("maskOneTexture").attribute("texturepath").as_string();
	maskTwoTexturePath = (char*)config.child("maskTwoTexture").attribute("texturepath").as_string();
	maskThreeTexturePath = (char*)config.child("maskThreeTexture").attribute("texturepath").as_string();

	rectBarraVida = new SDL_Rect{ 269,6,259,16 };
	rectFondoBarraVida = new SDL_Rect{ 0,2,267,23 };

	rectFondoMascara = new SDL_Rect{ 0,37,101,102 };
	rectFondoMascaraSecundaria = new SDL_Rect{ 110,43,90,90 };

	rectCDMascaraPrimaria = new SDL_Rect{ 0,0,100,100 };
	rectCDMascaraSecundaria = new SDL_Rect{ 0,0,100,100 };

	rectFondoPociones = new SDL_Rect{ 110,43,90,90 };
	rectPocionVida1 = new SDL_Rect{ 0,686,60,56 };
	rectPocionVida2 = new SDL_Rect{ 63,686,60,56 };
	rectPocionVida3 = new SDL_Rect{ 126,686,60,56 };
	rectPocionVidaMax = new SDL_Rect{ 190,684,60,58 };
	rectPocionRegeneracion = new SDL_Rect{ 255,686,60,56 };
	rectPocionDano = new SDL_Rect{ 324,686,60,100 };
	rectPocionVelocidad = new SDL_Rect{ 387,686,62,65 };
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

	rectFondoQuest = new SDL_Rect{ 0,822,357,41 };

	rectMascaraNoMask = new SDL_Rect{ 0,0,1,1 };
	rectMascara0 = new SDL_Rect{ 3,499,100,100 };
	rectMascara1 = new SDL_Rect{ 105,499,100,100 };
	rectMascara2 = new SDL_Rect{ 207,499,100,100 };
	rectMascara3 = new SDL_Rect{ 309,499,100,100 };

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

	cdPrimaryMaskTexture = app->tex->Load(cdMaskTexturePath);
	cdSecondaryMaskTexture = app->tex->Load(cdMaskTexturePath);

	levelUpTexture = app->tex->Load(levelUpTexturePath);
	maskZeroTexture = app->tex->Load(maskZeroTexturePath);
	maskOneTexture = app->tex->Load(maskOneTexturePath);
	maskTwoTexture = app->tex->Load(maskTwoTexturePath);
	maskThreeTexture = app->tex->Load(maskThreeTexturePath);

	low_health_fx = app->audio->LoadAudioFx("low_health_fx");
	level_up_fx = app->audio->LoadAudioFx("level_up_fx");
	use_potion_fx = app->audio->LoadAudioFx("use_potion_fx");

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
	if(app->entityManager->GetPlayer() != nullptr) 
	{

		float currentHealth = app->entityManager->GetPlayer()->currentStats.currentHealth;
		float maxHealth = app->entityManager->GetPlayer()->currentStats.maxHealth;
		isLowHealth = (currentHealth / maxHealth) < 0.2f;

		// Primary mask
		float maskCoolDownPrimary = app->entityManager->GetPlayer()->maskStats[app->entityManager->GetPlayer()->primaryMask][Branches::Rama2][ app->entityManager->GetPlayer()->maskLevels[app->entityManager->GetPlayer()->primaryMask][Branches::Rama2]].maskCoolDown;
		float elapsedTimePrimary = app->entityManager->GetPlayer()->maskStats[app->entityManager->GetPlayer()->primaryMask][Branches::Rama2][app->entityManager->GetPlayer()->maskLevels[app->entityManager->GetPlayer()->primaryMask][Branches::Rama2]].maskCoolDownTimer.ReadMSec();
		int alphaModPrimary = 0;

		if (elapsedTimePrimary < maskCoolDownPrimary) {
			float percentageRemainingPrimary = (maskCoolDownPrimary - elapsedTimePrimary) / maskCoolDownPrimary;

			//alphaModPrimary = 100 + (int)(100 * percentageRemainingPrimary);
			// if (alphaModPrimary > 200) {
			// 	alphaModPrimary = 200;
			// }

			alphaModPrimary = 200;
			int rectHeightPrimary = static_cast<int>(percentageRemainingPrimary * 100);

			rectYPrimaryMask = 100 - rectHeightPrimary;

			rectCDMascaraPrimaria->h = rectHeightPrimary;
			rectCDMascaraPrimaria->y = rectYPrimaryMask;
		}

		SDL_SetTextureAlphaMod(cdPrimaryMaskTexture, alphaModPrimary);

		// Secondary mask
		float maskCoolDownSecondary = app->entityManager->GetPlayer()->maskStats[app->entityManager->GetPlayer()->secondaryMask][Branches::Rama2][ app->entityManager->GetPlayer()->maskLevels[app->entityManager->GetPlayer()->secondaryMask][Branches::Rama2]].maskCoolDown;
		float elapsedTimeSecondary = app->entityManager->GetPlayer()->maskStats[app->entityManager->GetPlayer()->secondaryMask][Branches::Rama2][app->entityManager->GetPlayer()->maskLevels[app->entityManager->GetPlayer()->secondaryMask][Branches::Rama2]].maskCoolDownTimer.ReadMSec();
		int alphaModSecondary = 0;

		if (elapsedTimeSecondary < maskCoolDownSecondary) {
			float percentageRemainingSecondary = (maskCoolDownSecondary - elapsedTimeSecondary) / maskCoolDownSecondary;
			
			// alphaModSecondary = 100 + (int)(100 * percentageRemainingSecondary);
			// if (alphaModSecondary > 200) {
			// 	alphaModSecondary = 200;
			// }

			alphaModSecondary = 200;
			int rectHeightSecondary = static_cast<int>(percentageRemainingSecondary * 100);

			rectYSecondaryMask = 100 - rectHeightSecondary;

			rectCDMascaraSecundaria->h = rectHeightSecondary;
			rectCDMascaraSecundaria->y = rectYSecondaryMask;
		}

		SDL_SetTextureAlphaMod(cdSecondaryMaskTexture, alphaModSecondary);
	}

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

	if (monedasObtenidasHud < app->inventoryManager->monedasObtenidas) {
		monedasObtenidasHud += 1;
	}
	else if (monedasObtenidasHud < app->inventoryManager->monedasObtenidas) {
		monedasObtenidasHud -= 1;
	}

	return true;
}
bool fade = false;
int couunt = 0;
// Called each loop iteration
bool Hud::PostUpdate()
{

	if (app->entityManager->GetPlayer() == nullptr) return true;

	app->win->GetWindowSize(windowWidth, windowHeight);


	// Barra de vida
	int rectW = rectFondoBarraVida->w;
	rectW = (rectFondoBarraVida->w * app->entityManager->GetPlayer()->currentStats.currentHealth) / app->entityManager->GetPlayer()->currentStats.maxHealth;

	SDL_Rect* rectBarraVidaCalculado = new SDL_Rect{ rectBarraVida->x, rectBarraVida->y, rectW, rectBarraVida->h };

	if (isLowHealth && !shrinking && !growing && app->entityManager->GetPlayer()->currentStats.currentHealth > 0) {
		app->audio->PlayTimedFx(low_health_fx, 696);
		shrinking = true;
		animationTimer.Start();
	}

	if (shrinking) {

		float progress = animationTimer.ReadMSec() / (animationDuration * 1000);
		float easedProgress = easeInOutCubic(progress);
		float scale = 1 - easedProgress * 0.06;

		// Calcular la nueva posición para que la textura se reduzca hacia el centro
		int newX = rectFondoBarraVida->x + rectFondoBarraVida->w / 2 - (rectFondoBarraVida->w * scale) / 2;
		int newY = rectFondoBarraVida->y + rectFondoBarraVida->h / 2 - (rectFondoBarraVida->h * scale) / 2;

		app->render->DrawTexture(hudTexture, newX + 175, newY + 40, scale, SDL_FLIP_NONE, rectFondoBarraVida, 0);
		app->render->DrawTexture(hudTexture, newX + 179, newY + 44, scale, SDL_FLIP_NONE, rectBarraVidaCalculado, 0);

		if (animationTimer.ReadMSec() >= animationDuration * 1000) {
			shrinking = false;
			growing = true;
			animationTimer.Start();
		}
	}
	else if (growing) {

		float progress = animationTimer.ReadMSec() / (animationDuration * 10000);
		float easedProgress = easeInOutCubic(progress);
		float scale = 0.94 + easedProgress * 0.06;

		// Calcular la nueva posición para que la textura crezca hacia el centro
		int newX = rectFondoBarraVida->x + rectFondoBarraVida->w / 2 - (rectFondoBarraVida->w * scale) / 2;
		int newY = rectFondoBarraVida->y + rectFondoBarraVida->h / 2 - (rectFondoBarraVida->h * scale) / 2;

		app->render->DrawTexture(hudTexture, newX + 175, newY + 40, scale, SDL_FLIP_NONE, rectFondoBarraVida, 0);
		app->render->DrawTexture(hudTexture, newX + 179, newY + 44, scale, SDL_FLIP_NONE, rectBarraVidaCalculado, 0);

		if (animationTimer.ReadMSec() >= animationDuration * 10000) {
			growing = false;
		}
	}
	else {
		app->render->DrawTexture(hudTexture, rectFondoBarraVida->x + 175, rectFondoBarraVida->y + 40, 1.0, SDL_FLIP_NONE, rectFondoBarraVida, 0);
		app->render->DrawTexture(hudTexture, rectFondoBarraVida->x + 179, rectFondoBarraVida->y + 44, 1.0, SDL_FLIP_NONE, rectBarraVidaCalculado, 0);
	}


	//Monedas
	std::string quantityStr = std::to_string(monedasObtenidasHud);
	//std::string quantityStr = std::to_string(app->inventoryManager->monedasObtenidas);

	app->render->DrawTexture(hudTexture, windowWidth - rectFondoMonedas->w - 35, 130, SDL_FLIP_NONE, rectFondoMonedas, 0);
	//app->render->DrawText(quantityStr.c_str(), windowWidth - rectFondoMonedas->w + 10, 140, 18, 18);
	app->render->DrawTextBound(quantityStr.c_str(), windowWidth - rectFondoMonedas->w - (quantityStr.size() * 10) + 30, 133, 100, SDL_Color{52, 25, 0}, app->render->goldFont);
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
	Mask primaryMask = app->entityManager->GetPlayer()->GetPrimaryMask();
	Mask secondaryMask = app->entityManager->GetPlayer()->GetSecondaryMask();
	//Mask primaryMask2 = *app->entityManager->GetPlayer()->GetPrimaryMask();


	SDL_Rect rectPrimaryMask;
	SDL_Rect rectSecondaryMask;


	// Mensaje de depuración
	switch (primaryMask)
	{
		case Mask::NOMASK: rectPrimaryMask = *rectMascaraNoMask; break;
		case Mask::MASK0: rectPrimaryMask = *rectMascara0; break;
		case Mask::MASK1: rectPrimaryMask = *rectMascara1; break;
		case Mask::MASK2: rectPrimaryMask = *rectMascara2; break;
		case Mask::MASK3: rectPrimaryMask = *rectMascara3; break;
		default: rectPrimaryMask = *rectMascaraNoMask; break;
	}


	
	switch (secondaryMask)
	{
		case Mask::NOMASK: rectSecondaryMask = *rectMascaraNoMask; break;
		case Mask::MASK0: rectSecondaryMask = *rectMascara0; break;
		case Mask::MASK1: rectSecondaryMask = *rectMascara1; break;
		case Mask::MASK2: rectSecondaryMask = *rectMascara2; break;
		case Mask::MASK3: rectSecondaryMask = *rectMascara3; break;
		default: rectSecondaryMask = *rectMascaraNoMask; break;
	}
	

	app->render->DrawTexture(hudTexture, 75, 15, SDL_FLIP_NONE, rectFondoMascaraSecundaria, 0);
	app->render->DrawTexture(hudTexture, 83, 18, 0.79f,SDL_FLIP_NONE, &rectSecondaryMask, 0);
	app->render->DrawTexture(cdSecondaryMaskTexture, 80, 18 + (rectYSecondaryMask * 0.79f), 0.79f, SDL_FLIP_NONE, rectCDMascaraSecundaria, 0);
	app->render->DrawTexture(hudTexture, 25, 40, SDL_FLIP_NONE, rectFondoMascara, 0);
	app->render->DrawTexture(hudTexture, 25, 40, SDL_FLIP_NONE, &rectPrimaryMask, 0);
	app->render->DrawTexture(cdPrimaryMaskTexture, 25, 40 + rectYPrimaryMask, SDL_FLIP_NONE, rectCDMascaraPrimaria, 0);
	//Fin mascaras
 



	//Botones
	/*app->render->DrawTexture(hudTexture, windowWidth - rectFondoInventario->w + 10, 70, SDL_FLIP_NONE, rectBotonTAB, 0);
	app->render->DrawTexture(hudTexture, 120, 85, SDL_FLIP_NONE, rectBotonQ, 0);*/


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
		app->render->DrawTextBound(acquired_Items.at(i)->text.c_str(), 1080, 605 - (i * 50), 200, { 52, 25, 0 },app->render->titleFont);

	}

	SDL_SetTextureAlphaMod(hudTexture, 255);

	//Me da miedo preguntar que es esto de aqui abajo, solo sabe dios que hace.
	if (estatua)
	{
		if (app->input->GetButton(APP_EXIT) == KEY_DOWN || app->input->GetButton(BACK) == KEY_DOWN || app->input->GetButton(CONFIRM) == KEY_DOWN || app->input->GetButton(SELECT) == KEY_DOWN) {
			if (couunt > 0 )
			{
				estatua = false;
			}
			couunt++;

		}
		app->physics->active = false;
		{
			uint windowWidth, windowHeight;
			app->win->GetWindowSize(windowWidth, windowHeight);
			Uint8 alpha = 188;  // Valor de transparencia (0-255)
			SDL_Texture* transparentTexture = app->menu->CreateTransparentTexture(app->render->renderer, windowWidth, windowHeight, alpha);
			if (transparentTexture != nullptr) {
				SDL_SetTextureBlendMode(transparentTexture, SDL_BLENDMODE_BLEND);
				SDL_RenderCopy(app->render->renderer, transparentTexture, nullptr, nullptr);
				SDL_DestroyTexture(transparentTexture);
			}
			app->render->DrawTexture(EstatuaTexture, 0, 0, SDL_FLIP_NONE, 0, 0);
		}
	}
	else
	{
		app->physics->active = true;
		couunt = 0;
	}



	//Misiones

	if (app->scene_pueblo->active || app->scene_pueblo_tutorial->active || app->scene_mazmorra0->active) {

		std::vector<Quest*> quests = app->questManager->GetActiveQuest();
		for (int i = 0; i < quests.size(); i++) {
			app->render->DrawTexture(hudTexture, 925, 200 + (i * 50), SDL_FLIP_NONE, rectFondoQuest, 0);
			app->render->DrawTextBound(quests.at(i)->questTitle.c_str(), 992, 207 + (i * 50), 300, { 52, 25, 0 }, app->render->questFont);
		}

	}


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

	delete rectCDMascaraPrimaria;
	delete rectCDMascaraSecundaria;

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
	delete rectMascaraNoMask;

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
	Acquired_Item* acquiredItem = new Acquired_Item{ texture, text };
	acquiredItem->lifeTimer.Start();

	acquired_Items.push_back(acquiredItem);
}

void Hud::Potions()
{
	if (!potionRects.empty()) {
		if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) {
			if (selectedPotionIndex >= 0 && selectedPotionIndex < potionRects.size()) {
				ListItem<Inventity*>* item = GetSelectedPotionItem();
				if (item) {
					app->inventoryManager->UsePotionSelected(item);
					app->audio->PlayTimedFx(use_potion_fx, 175);
				}
			}
		}
		else if (app->input->GetButton(CAMBIAR_POCION_RIGHT) == KEY_DOWN && selectedPotionIndex < potionRects.size() - 1) {
			selectedPotionIndex++;
		}
		else if (app->input->GetButton(CAMBIAR_POCION_LEFT) == KEY_DOWN && selectedPotionIndex > 0) {
			selectedPotionIndex--;
		}
	}

	potionRects.clear();

	ListItem<Inventity*>* item;
	int potionIndex = 0;
	for (item = app->inventoryManager->inventities.start; item != NULL; item = item->next) {
		Inventity* inventity = item->data;
		if (IsPotion(inventity->type)) {
			potionRects.push_back(potionRectMap[inventity->type]);
			potionIndex++;
		}
	}
	if (selectedPotionIndex >= potionIndex) {
		selectedPotionIndex = std::max(0, potionIndex - 1);
	}
}

bool Hud::IsPotion(InventityType type) {
	return (type == InventityType::POCION_VIDA_1 || type == InventityType::POCION_VIDA_2 || type == InventityType::POCION_VIDA_3 ||
		type == InventityType::POCION_VIDA_MAX || type == InventityType::POCION_REGENERACION || type == InventityType::POCION_DANO ||
		type == InventityType::POCION_VELOCIDAD || type == InventityType::ORBE_MAGICO);
}

ListItem<Inventity*>* Hud::GetSelectedPotionItem() {
	ListItem<Inventity*>* item = app->inventoryManager->inventities.start;
	int potionIndex = 0;
	while (item != NULL) {
		Inventity* inventity = item->data;
		if (IsPotion(inventity->type)) {
			if (potionIndex == selectedPotionIndex) {
				return item;
			}
			potionIndex++;
		}
		item = item->next;
	}
	return nullptr;
}



bool Hud::HayPocionesDisponibles() {
	return !potionRects.empty();
}
