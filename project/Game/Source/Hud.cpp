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
#include "Hud.h"
#include "Scene_Menu.h"
#include "Scene_Testing.h"
#include "ModuleFadeToBlack.h"

Hud::Hud(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("Menu");
}

// Destructor
Hud::~Hud()
{}

// Called before render is available
bool Hud::Awake(pugi::xml_node config)
{
	LOG("Menu");
	bool ret = true;

	

	return ret;
}

// Called before the first frame
bool Hud::Start()
{
	
	
	

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
	
	return true;
}

// Called each loop iteration
bool Hud::PostUpdate()
{
	

	return true;
}

// Called before quitting
bool Hud::CleanUp()
{
	LOG("Freeing Scene_Intro");

	return true;
}


