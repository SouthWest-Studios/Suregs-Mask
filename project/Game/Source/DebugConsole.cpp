#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "DebugConsole.h"
#include "Map.h"
#include "Item.h"
#include "ModuleFadeToBlack.h"
#include "Optick/include/optick.h"

#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include <iostream>

DebugConsole::DebugConsole(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("debugConsole");
}

// Destructor
DebugConsole::~DebugConsole()
{}

// Called before render is available
bool DebugConsole::Awake(pugi::xml_node config)
{
	LOG("Loading DebugConsole");
	bool ret = true;

	KILL_ALL = DebugCommand("kill_all", "Elimina todas las entidades de la pantalla", "kill_all", []() {
		std::cout << "Comando KILLALL ejecutado" << std::endl;
		});







	commandList.Add(KILL_ALL);



	if(!active){
		return ret;
	}

	

	return ret;
}

// Called before the first frame
bool DebugConsole::Start()
{

	app->win->GetWindowSize(windowW, windowH);
	currentCommand = "hola";
	

	return true;
}

// Called each loop iteration
bool DebugConsole::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool DebugConsole::Update(float dt)
{

   	OPTICK_EVENT();

	if (app->input->GetKey(SDL_SCANCODE_GRAVE) == KEY_DOWN) { //SE MUESTRA CON LA TECLA: º
		showConsole = !showConsole;
		app->input->ResetInputText();
	}

	currentCommand = app->input->GetInputText();

	return true;
}

// Called each loop iteration
bool DebugConsole::PostUpdate()
{
	bool ret = true;
	

	return ret;
}

bool DebugConsole::PostLateUpdate()
{
	bool ret = true;

	if (showConsole) {
		

		//Fondo
		app->render->DrawRectangle(SDL_Rect{0,0, (int)windowW, 40 }, 0,0,0, 100, true, false);

		// Renderizar texto de la consola
		SDL_Color textColor = { 255, 255, 255, 255 }; // Color del texto blanco
		SDL_Surface* surface = TTF_RenderText_Blended(app->render->consoleFont,("> " + currentCommand).c_str(), textColor);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(app->render->renderer, surface);
		SDL_Rect textRect = { 10, 7, surface->w, surface->h }; // Posición del texto
		SDL_RenderCopy(app->render->renderer, texture, NULL, &textRect);
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);
		

		
	}

	return ret;
}

// Called before quitting
bool DebugConsole::CleanUp()
{
	LOG("Freeing DebugConsole");

	return true;
}



bool DebugConsole::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	return true;
}


