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


	return true;
}

// Called each loop iteration
bool Menu_Equipo::PreUpdate()
{
	
	
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
		int levelSword = static_cast<int>(app->inventoryManager->swordLevel);
		std::string levelSwordStr = "Nivel: " + std::to_string(levelSword);
		app->render->DrawTextBound(levelSwordStr.c_str(), 280, 200, 200, { 0,0,0,0 });

		int attackDamage = static_cast<int>(app->entityManager->GetPlayer()->currentStats.attackDamage);
		std::string AtaqueStr = "Ataque: " + std::to_string(attackDamage);
		app->render->DrawTextBound(AtaqueStr.c_str(), 280, 250, 200, { 0,0,0,0 });

		int levelArmour = static_cast<int>(app->inventoryManager->armorLevel);
		std::string levelArmorStr = "Nivel: " + std::to_string(levelArmour);
		app->render->DrawTextBound(levelArmorStr.c_str(), 670, 200, 200, { 0,0,0,0 });

		int vida = static_cast<int>(app->entityManager->GetPlayer()->currentStats.currentHealth);
		std::string VidaStr = "Vida: " + std::to_string(vida);

		app->render->DrawTextBound(VidaStr.c_str(), 670, 250, 200, { 0,0,0,0 });

	/*	std::string MaskAtackStr = std::to_string(app->entityManager->GetPlayer()->maskStats[app->entityManager->GetPlayer()->primaryMask][Branches::Rama1][app->entityManager->GetPlayer()->maskLevels[app->entityManager->GetPlayer()->primaryMask][Branches::Rama1]].maskDamage);

		app->render->DrawTextBound(MaskAtackStr.c_str(), 320, 430, 100, { 0,0,0,0 });

		std::string MaskAtackStr2 = std::to_string(app->entityManager->GetPlayer()->maskStats[app->entityManager->GetPlayer()->primaryMask][Branches::Modifiers][app->entityManager->GetPlayer()->maskLevels[app->entityManager->GetPlayer()->primaryMask][Branches::Modifiers]].attackDamageModifier);

		app->render->DrawTextBound(MaskAtackStr2.c_str(), 720, 430, 100, { 0,0,0,0 });*/

		if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK0)
		{
			app->render->DrawTextBound("Dinka", 280, 395, 200, { 0,0,0,0 });
			app->render->DrawTextBound("Rayo Magico", 270, 450, 200, { 0,0,0,0 });
			app->render->DrawTexture(mask0Texture, 470, 420, SDL_FLIP_NONE, 0, 0);
		}
		else if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK1)
		{
			app->render->DrawTextBound("Inuit", 280, 395, 200, { 0,0,0,0 });
			app->render->DrawTextBound("Esfera Expansiva", 270, 450, 200, { 0,0,0,0 });
			app->render->DrawTexture(mask1Texture, 470, 420, SDL_FLIP_NONE, 0, 0);
		}
		else if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK2)
		{
			app->render->DrawTextBound("Musri", 280, 395, 200, { 0,0,0,0 });
			app->render->DrawTextBound("Dash Invisible", 270, 450, 200, { 0,0,0,0 });
			app->render->DrawTexture(mask2Texture, 470, 420, SDL_FLIP_NONE, 0, 0);
		}
		else if (app->entityManager->GetPlayer()->primaryMask == Mask::MASK3)
		{
			app->render->DrawTextBound("Surma", 280, 395, 200, { 0,0,0,0 });
			app->render->DrawTextBound("Mejora Interna", 270, 450, 200, { 0,0,0,0 });
			app->render->DrawTexture(mask3Texture, 470, 420, SDL_FLIP_NONE, 0, 0);
		}

		if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK0)
		{
			app->render->DrawTextBound("Dinka", 678, 395, 200, { 0,0,0,0 });
			app->render->DrawTextBound("Rayo Magico", 660, 450, 200, { 0,0,0,0 });
			app->render->DrawTexture(mask0Texture, 860, 420, SDL_FLIP_NONE, 0, 0);
		}
		else if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK1)
		{
			app->render->DrawTextBound("Inuit", 678, 395, 200, { 0,0,0,0 });
			app->render->DrawTextBound("Esfera Expansiva", 660, 450, 200, { 0,0,0,0 });
			app->render->DrawTexture(mask1Texture, 860, 420, SDL_FLIP_NONE, 0, 0);
		}
		else if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK2)
		{
			app->render->DrawTextBound("Musri", 678, 395, 200, { 0,0,0,0 });
			app->render->DrawTextBound("Dash Invisible", 660, 450, 200, { 0,0,0,0 });
			app->render->DrawTexture(mask2Texture, 860, 420, SDL_FLIP_NONE, 0, 0);
		}
		else if (app->entityManager->GetPlayer()->secondaryMask == Mask::MASK3)
		{
			app->render->DrawTextBound("Surma", 678, 395, 200, { 0,0,0,0 });
			app->render->DrawTextBound("Mejora Interna", 660, 450, 200, { 0,0,0,0 });
			app->render->DrawTexture(mask3Texture, 860, 420, SDL_FLIP_NONE, 0, 0);
		}
	}

	
	

	return true;
}

// Called before quitting
bool Menu_Equipo::CleanUp()
{
	

	return true;
}




