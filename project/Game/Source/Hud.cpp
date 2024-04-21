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
#include "Menu.h"
#include "Scene_Menu.h"
#include "Scene_Testing.h"
#include "ModuleFadeToBlack.h"

Hud::Hud(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("hud");
}

// Destructor
Hud::~Hud()
{}

// Called before render is available
bool Hud::Awake(pugi::xml_node config)
{

	bool ret = true;

	

	return ret;
}

// Called before the first frame
bool Hud::Start()
{
	hudTexture = app->tex->Load("Assets/Textures/Interfaz/HUD/texture_general_hud.png");
	rectBarraVida					= new SDL_Rect{ 220,0,220,28 };
	rectFondoBarraVida				= new SDL_Rect{ 0,0,220,28 };
	rectFondoMascara				= new SDL_Rect{ 0,38,101,101 };
	rectFondoMascaraSecundaria		= new SDL_Rect{ 115,48,79,79 };
	rectFondoPociones				= new SDL_Rect{ 213,67,44,44 };
	rectFondoHabilidad1				= new SDL_Rect{ 1,149,60,60 };
	rectFondoHabilidad2				= new SDL_Rect{ 65,157,45,45 };
	rectFondoInventario				= new SDL_Rect{ 0,218,113,113 };
	rectFondoMonedas				= new SDL_Rect{0,338,97,32};
	rectFondoObjetosConseguidos		= new SDL_Rect{ 0,338,97,32 };
	rectMascara0					= new SDL_Rect{ 3,499,100,100 };
	rectMascara1					= new SDL_Rect{ 0,0,0,0 };
	rectMascara2					= new SDL_Rect{ 0,0,0,0 };
	rectMascara3					= new SDL_Rect{ 0,0,0,0 };
	rectMascara4					= new SDL_Rect{ 0,0,0,0 };
	
	

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
	app->win->GetWindowSize(windowWidth, windowHeight);
	


	//Barra de vida
	app->render->DrawTexture(hudTexture, 165, 40, SDL_FLIP_NONE, rectFondoBarraVida, 0);
	app->render->DrawTexture(hudTexture, 165, 40, SDL_FLIP_NONE, rectBarraVida, 0);
	
	//Monedas
	std::string quantityStr = std::to_string(cantidadMonedas);
	
	app->render->DrawTexture(hudTexture, windowWidth - rectFondoMonedas->w - 20, windowHeight / 8 - 50, SDL_FLIP_NONE, rectFondoMonedas, 0);
	app->render->DrawText(quantityStr.c_str(), windowWidth - rectFondoMonedas->w + 10, windowHeight / 8 - 40, 18, 18);


	//Fondos
	app->render->DrawTexture(hudTexture, 75, 15, SDL_FLIP_NONE, rectFondoMascaraSecundaria, 0);
	app->render->DrawTexture(hudTexture, 25, 40, SDL_FLIP_NONE, rectFondoMascara, 0);
	//app->render->DrawTexture(hudTexture, 50, 50, SDL_FLIP_NONE, rectFondoPociones, 0);
	//app->render->DrawTexture(hudTexture, 50, 50, SDL_FLIP_NONE, rectFondoHabilidad1, 0);
	//app->render->DrawTexture(hudTexture, 50, 50, SDL_FLIP_NONE, rectFondoHabilidad2, 0);
	//app->render->DrawTexture(hudTexture, 50, 50, SDL_FLIP_NONE, rectFondoInventario, 0);



	//Objeto conseguido
	//app->render->DrawTexture(hudTexture, 50, 50, SDL_FLIP_NONE, rectFondoObjetosConseguidos, 0);
	
	

	return true;
}

// Called before quitting
bool Hud::CleanUp()
{
	LOG("Freeing Scene_Intro");

	return true;
}


