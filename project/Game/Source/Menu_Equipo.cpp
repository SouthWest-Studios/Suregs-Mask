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
	
	

	return ret;
}

// Called before the first frame
bool Menu_Equipo::Start()
{
	messageTexture = app->tex->Load(messageTexturePath);



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
		std::string AtaqueStr = std::to_string(app->entityManager->GetPlayer()->currentStats.attackDamage);

		app->render->DrawTextBound(AtaqueStr.c_str(), 200, 200, 100, { 0,0,0,0 });

		std::string MaskAtackStr = std::to_string(app->entityManager->GetPlayer()->maskStats[app->entityManager->GetPlayer()->primaryMask][Branches::Rama1][app->entityManager->GetPlayer()->maskLevels[app->entityManager->GetPlayer()->primaryMask][Branches::Rama1]].maskDamage);

		app->render->DrawTextBound(MaskAtackStr.c_str(), 200, 300, 100, { 0,0,0,0 });

		std::string MaskAtackStr2 = std::to_string(app->entityManager->GetPlayer()->maskStats[app->entityManager->GetPlayer()->primaryMask][Branches::Modifiers][app->entityManager->GetPlayer()->maskLevels[app->entityManager->GetPlayer()->primaryMask][Branches::Modifiers]].attackDamageModifier);

		app->render->DrawTextBound(MaskAtackStr2.c_str(), 200, 400, 100, { 0,0,0,0 });
	}

	
	

	return true;
}

// Called before quitting
bool Menu_Equipo::CleanUp()
{
	

	return true;
}




