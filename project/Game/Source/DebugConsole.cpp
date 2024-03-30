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

	KILL_ALL = new DebugCommand("kill_all", "Elimina todas las entidades de la pantalla", "kill_all", []() {
		std::cout << "Comando KILLALL ejecutado" << std::endl;
		});

	SET_GOLD = new DebugCommandArg<int>("set_gold", "Establece la cantidad de oro", "set_gold <amount>", [](int amount) {
		std::cout << "Comando SET_GOLD ejecutado con " << amount << " de oro" << std::endl;
		});


	commandList.Add(KILL_ALL);
	commandList.Add(SET_GOLD);



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

	if (app->input->GetKey(SDL_SCANCODE_GRAVE) == KEY_DOWN) { //SE MUESTRA CON LA TECLA: �
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
		app->render->DrawRectangle(SDL_Rect{ 0,0, (int)windowW, 40 }, 0, 0, 0, 100, true, false);

		// Renderizar texto de la consola
		SDL_Color textColor = { 255, 255, 255, 255 }; // Color del texto blanco
		SDL_Surface* surface = TTF_RenderText_Blended(app->render->consoleFont, ("> " + currentCommand).c_str(), textColor);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(app->render->renderer, surface);
		SDL_Rect textRect = { 10, 7, surface->w, surface->h }; // Posici�n del texto
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

		DebugCommandArg<int>* commandArg = dynamic_cast<DebugCommandArg<int>*>(commandBase);
		if (commandArg != nullptr) {
			// El puntero se ha convertido con �xito
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
std::vector<std::string> DebugConsole::SplitString(const std::string& input, char delimiter) {
	std::vector<std::string> result;
	std::string::size_type start = 0;
	std::string::size_type end = input.find(delimiter);

	while (end != std::string::npos) {
		// Extraer la subcadena entre start y end
		result.push_back(input.substr(start, end - start));
		// Actualizar start para el pr�ximo substring
		start = end + 1;
		// Buscar el siguiente delimitador
		end = input.find(delimiter, start);
	}

	// Agregar la �ltima subcadena
	result.push_back(input.substr(start));

	return result;
}


