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
#include "Scene_menu.h"
#include "Scene_Testing.h"
#include "Optick/include/optick.h"

#include "DialogManager.h"

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

	KILL_ALL = new DebugCommand("kill_all", "Elimina todas las entidades de la pantalla", "kill_all", [this]() {
		std::cout << "Comando KILLALL ejecutado" << std::endl;
		});
	commandList.Add(KILL_ALL);


	HELP = new DebugCommand("help", "Muestra/Oculta la lista de comandos", "help", [this]() {
		showHelp = !showHelp;
		scrollY = 0;
		});
	commandList.Add(HELP);


	SET_GOLD = new DebugCommandArg<int>("set_gold", "Establece la cantidad de oro", "set_gold <cantidad>", [this](int amount) {
		std::cout << "Comando SET_GOLD ejecutado con " << amount << " de oro" << std::endl;
		});
	commandList.Add(SET_GOLD);


	GOTO = new DebugCommandArg<int>("goto", "Te cambia de escena por la elegida", "goto <int nivel>", [this](int nivel) {

		switch (nivel)
		{
		case 0: app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_menu); break;
		case 1: app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_testing); break;
		/*case 2: app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_menu); break;
		case 3: app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_menu); break;
		case 4: app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_menu); break;*/
		default:
			break;
		}

		
		});
	commandList.Add(GOTO);

	SPAWN_DIALOG = new DebugCommand("spawn_dialog", "Spawnea un dialogo de prueba", "spawn_dialog", [this]() {
		Dialog* d1 = new Dialog("Pene");
		d1->name = "NPC 1";
		d1->face_tex = app->tex->Load("Assets/Textures/suscat2.jpg");
		app->dialogManager->AddDialog(d1);
		});
	commandList.Add(SPAWN_DIALOG);
	

	



	if (!active) {
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

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		HandleInput();
		app->input->ResetInputText();
	}


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
		if (showHelp) {
			app->render->DrawRectangle(SDL_Rect{ 0,0, (int)windowW, 40 }, 20, 0, 0, 200, true, false);
		}
		else {
			app->render->DrawRectangle(SDL_Rect{ 0,0, (int)windowW, 40 }, 0, 0, 0, 100, true, false);
		}
		

		// Renderizar texto de la consola
		SDL_Color textColor = { 255, 255, 255, 255 }; // Color del texto blanco
		SDL_Surface* surface = TTF_RenderText_Blended(app->render->consoleFont, ("> " + currentCommand).c_str(), textColor);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(app->render->renderer, surface);
		SDL_Rect textRect = { 10, 7, surface->w, surface->h }; // Posición del texto
		SDL_RenderCopy(app->render->renderer, texture, NULL, &textRect);
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);

		if (showHelp) {
			int y = 40;
			int x = 0;
			app->render->DrawRectangle(SDL_Rect{ 0, y, (int)windowW, (int)windowH }, 0, 0, 0, 220, true, false);
			
			for (int i = 0; i < commandList.Count(); i++)
			{
				DebugCommandBase* command = dynamic_cast<DebugCommandBase*>(commandList[i]);
				std::string label = command->GetCommandFormat() + " - " + command->GetCommandDescription();

				// Calcular posición de la etiqueta
				int labelX = 5;
				int labelY = y + i * 30;

				// Renderizar la etiqueta
				SDL_Surface* surface = TTF_RenderText_Blended(app->render->consoleFont, label.c_str(), textColor);
				SDL_Texture* texture = SDL_CreateTextureFromSurface(app->render->renderer, surface);
				int texW, texH;
				SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);

				// Definir el rectángulo de destino
				SDL_Rect dstRect = { labelX, labelY - scrollY, texW, texH };

				// Renderizar la textura en la pantalla
				SDL_RenderCopy(app->render->renderer, texture, nullptr, &dstRect);

				// Liberar la textura y la superficie
				SDL_DestroyTexture(texture);
				SDL_FreeSurface(surface);

			}
		}



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

void DebugConsole::HandleInput()
{

	std::vector<std::string> properties = SplitString(currentCommand, ' ');


	for (int i = 0; i < commandList.Count(); i++) {
		DebugCommandBase* commandBase = commandList[i];

		//Intenta hacer un casting de DebugCommandBase a DebugCommand
		DebugCommand* command = dynamic_cast<DebugCommand*>(commandBase);

		// Verifica si el casting fue exitoso
		if (command != nullptr) {
			std::string commandId = command->GetCommandId();

			// Verificar si currentCommand contiene el identificador del comando
			if (currentCommand.find(commandId) != std::string::npos) {
				// Invocar el comando si se encuentra
				command->Invoke();
			}
		}
		else {

			DebugCommandArg<int>* commandArg = dynamic_cast<DebugCommandArg<int>*>(commandBase);
			if (commandArg != nullptr) {
				// El puntero se ha convertido con éxito
				std::string commandArgId = commandArg->GetCommandId();
				if (currentCommand.find(commandArgId) != std::string::npos) {
					// Ejecutar el comando con el argumento entero

					if (properties.size() > 1) {

						int argValue = std::stoi(properties[1]); // Convertir la cadena a entero
						commandArg->Invoke(argValue);
					}
				}
			}
		}

	}
}
std::vector<std::string> DebugConsole::SplitString(const std::string& input, char delimiter) {
	std::vector<std::string> result;
	std::string::size_type start = 0;
	std::string::size_type end = input.find(delimiter);

	while (end != std::string::npos) {
		// Extraer la subcadena entre start y end
		result.push_back(input.substr(start, end - start));
		// Actualizar start para el próximo substring
		start = end + 1;
		// Buscar el siguiente delimitador
		end = input.find(delimiter, start);
	}

	// Agregar la última subcadena
	result.push_back(input.substr(start));

	return result;
}


