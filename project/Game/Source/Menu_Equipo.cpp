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
#include "InventoryManager.h"
#include "Menu_Equipo.h"
#include "Player.h"
#include "Menu.h"
#include "Scene_Menu.h"
#include "Scene_Testing.h"
#include "ModuleFadeToBlack.h"

Menu_Equipo::Menu_Equipo(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name = ("equipo");
}

// Destructor
Menu_Equipo::~Menu_Equipo()
{}

// Called before render is available
bool Menu_Equipo::Awake(pugi::xml_node config)
{

	bool ret = true;


	swordTexturePath = (char*)config.child("props").attribute("espada").as_string();
	armorTexturePath = (char*)config.child("props").attribute("armadura").as_string();
	messageTexturePath = (char*)config.child("messageTexture").attribute("texturepath").as_string();
	mask0TexturePath = (char*)config.child("props").attribute("mask0").as_string();
	mask1TexturePath = (char*)config.child("props").attribute("mask1").as_string();
	mask2TexturePath = (char*)config.child("props").attribute("mask2").as_string();
	mask3TexturePath = (char*)config.child("props").attribute("mask3").as_string();

	

	return ret;
}

// Called before the first frame
bool Menu_Equipo::Start()
{
	messageTexture = app->tex->Load(messageTexturePath);
	mask0Texture = app->tex->Load(mask0TexturePath);
	mask1Texture = app->tex->Load(mask1TexturePath);
	mask2Texture = app->tex->Load(mask2TexturePath);
	mask3Texture = app->tex->Load(mask3TexturePath);
	swordTexture = app->tex->Load(swordTexturePath);
	armorTexture = app->tex->Load(armorTexturePath);

	swordRect = new SDL_Rect { 0,0,138,140 };
	armorRect = new SDL_Rect{ 0,0,93,110 };

	return true;
}

// Called each loop iteration
bool Menu_Equipo::PreUpdate()
{
	swordRect->x = 138 * app->inventoryManager->swordLevel;

	armorRect->x = 93 * app->inventoryManager->armorLevel;
	
	return true;
}

// Called each loop iteration
bool Menu_Equipo::Update(float dt)
{
	
	
	//Item Acquired
	

	
	return true;
}

// Called each loop iteration
bool Menu_Equipo::PostUpdate()
{
	app->win->GetWindowSize(windowWidth, windowHeight);
	
	if (mostrar)
	{
		if (app->menu->animating == false && app->menu->animatingExit2 == false && app->menu->menuu == true)
		{
			int levelSword = static_cast<int>(app->inventoryManager->swordLevel + 1);
			std::string levelSwordStr = "Nivel: " + std::to_string(levelSword);
			app->render->DrawTextBound(levelSwordStr.c_str(), 280, 200, 200, { 52,25,0 }, app->render->titleFont);

			int attackDamage = static_cast<int>(app->entityManager->GetPlayer()->currentStats.attackDamage);
			std::string AtaqueStr = "Ataque: " + std::to_string(attackDamage);
			app->render->DrawTextBound(AtaqueStr.c_str(), 280, 250, 200, { 52,25,0 }, app->render->titleFont);

			int levelArmour = static_cast<int>(app->inventoryManager->armorLevel + 1);
			std::string levelArmorStr = "Nivel: " + std::to_string(levelArmour);
			app->render->DrawTextBound(levelArmorStr.c_str(), 670, 200, 200, { 52,25,0 }, app->render->titleFont);

			int vida = static_cast<int>(app->entityManager->GetPlayer()->currentStats.currentHealth);
			std::string VidaStr = "Vida: " + std::to_string(vida);

			app->render->DrawTextBound(VidaStr.c_str(), 670, 250, 200, { 52,25,0 }, app->render->titleFont);

			/*	std::string MaskAtackStr = std::to_string(app->entityManager->GetPlayer()->maskStats[app->entityManager->GetPlayer()->primaryMask][Branches::Rama1][app->entityManager->GetPlayer()->maskLevels[app->entityManager->GetPlayer()->primaryMask][Branches::Rama1]].maskDamage);

				app->render->DrawTextBound(MaskAtackStr.c_str(), 320, 430, 100, { 0,0,0,0 });

				std::string MaskAtackStr2 = std::to_string(app->entityManager->GetPlayer()->maskStats[app->entityManager->GetPlayer()->primaryMask][Branches::Modifiers][app->entityManager->GetPlayer()->maskLevels[app->entityManager->GetPlayer()->primaryMask][Branches::Modifiers]].attackDamageModifier);

				app->render->DrawTextBound(MaskAtackStr2.c_str(), 720, 430, 100, { 0,0,0,0 });*/

			app->render->DrawTexture(swordTexture, 413, 185, SDL_FLIP_NONE, swordRect, 0, 0);
			app->render->DrawTexture(armorTexture, 845, 190, SDL_FLIP_NONE, armorRect, 0, 0);

			if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK0)
			{
				app->render->DrawTextBound("Dinka", 280, 395, 200, { 52,25,0 }, app->render->titleFont);
				app->render->DrawTextBound("Rayo Magico", 270, 450, 200, { 52,25,0 }, app->render->titleFont);
				app->render->DrawTexture(mask0Texture, 470, 420, SDL_FLIP_NONE, 0, 0);
			}
			else if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK1)
			{
				app->render->DrawTextBound("Inuit", 280, 395, 200, { 52,25,0 }, app->render->titleFont);
				app->render->DrawTextBound("Esfera Expansiva", 270, 450, 200, { 52,25,0 }, app->render->titleFont);
				app->render->DrawTexture(mask1Texture, 470, 420, SDL_FLIP_NONE, 0, 0);
			}
			else if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK2)
			{
				app->render->DrawTextBound("Musri", 280, 395, 200, { 52,25,0 }, app->render->titleFont);
				app->render->DrawTextBound("Invulnerabilidad", 270, 450, 200, { 52,25,0 }, app->render->titleFont);
				app->render->DrawTexture(mask2Texture, 470, 420, SDL_FLIP_NONE, 0, 0);
			}
			else if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK3)
			{
				app->render->DrawTextBound("Surma", 280, 395, 200, { 52,25,0 }, app->render->titleFont);
				app->render->DrawTextBound("Mejora Interna", 270, 450, 200, { 52,25,0 }, app->render->titleFont);
				app->render->DrawTexture(mask3Texture, 470, 420, SDL_FLIP_NONE, 0, 0);
			}

			if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK0)
			{
				app->render->DrawTextBound("Dinka", 678, 395, 200, { 52,25,0 }, app->render->titleFont);
				app->render->DrawTextBound("Ataque Basico", 660, 450, 200, { 52,25,0 }, app->render->titleFont);
				app->render->DrawTexture(mask0Texture, 860, 420, SDL_FLIP_NONE, 0, 0);
			}
			else if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK1)
			{
				app->render->DrawTextBound("Inuit", 678, 395, 200, { 52,25,0 }, app->render->titleFont);
				app->render->DrawTextBound("Onda Expansiva", 660, 450, 200, { 52,25,0 }, app->render->titleFont);
				app->render->DrawTexture(mask1Texture, 860, 420, SDL_FLIP_NONE, 0, 0);
			}
			else if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK2)
			{
				app->render->DrawTextBound("Musri", 678, 395, 200, { 52,25,0 }, app->render->titleFont);
				app->render->DrawTextBound("Dash Invisible", 660, 450, 200, { 52,25,0 }, app->render->titleFont);
				app->render->DrawTexture(mask2Texture, 860, 420, SDL_FLIP_NONE, 0, 0);
			}
			else if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK3)
			{
				app->render->DrawTextBound("Surma", 678, 395, 200, { 52,25,0 }, app->render->titleFont);
				app->render->DrawTextBound("Ultima Vida", 660, 450, 200, { 52,25,0 }, app->render->titleFont);
				app->render->DrawTexture(mask3Texture, 860, 420, SDL_FLIP_NONE, 0, 0);
			}
		}

	}
	

	return true;
}

// Called before quitting
bool Menu_Equipo::CleanUp()
{
	

	return true;
}




