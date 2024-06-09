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
#include "Scene_Menu.h"
#include "Scene_Testing.h"
#include "Scene_gameOver.h"
#include "Scene_Mazmorra0.h"
#include "Scene_Mazmorra1.h"
#include "Scene_Mazmorra2.h"
#include "Scene_Mazmorra3.h"
#include "Scene_Mazmorra4.h"
#include "Scene_Mazmorra5.h"
#include "Scene_Mazmorra6.h"
#include "Scene_Mazmorra7.h"
#include "Scene_Mazmorra8.h"
#include "Scene_Logos.h"
#include "Scene_Pueblo.h"
#include "InventoryManager.h"
#include "Optick/include/optick.h"
#include "Menu.h"

#include "DialogManager.h"
#include "CommerceManager.h"
#include "InventoryManager.h"
#include "ElevatorMenu.h"

#include "Defs.h"
#include "Log.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include <iostream>

DebugConsole::DebugConsole(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name = ("debugConsole");
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
			app->inventoryManager->monedasObtenidas = amount;
		});
	commandList.Add(SET_GOLD);
	
	SET_MASK_POINTS = new DebugCommandArg<int>("set_mask_points", "Establece la cantidad de puntos de mascara(max 8) para todas las mascaras", "set_mask_points <cantidad>", [this](int amount) {
		app->inventoryManager->maskZeroPoints = amount;
		app->inventoryManager->maskOnePoints = amount;
		app->inventoryManager->maskTwoPoints = amount;
		app->inventoryManager->maskThreePoints = amount;
			});
	commandList.Add(SET_MASK_POINTS);

	SET_PRIMARY_MASK = new DebugCommandArg<int>("set_primary_mask", "Establece la mascara primaria del jugador", "set_primary_mask <int mask>", [this](int mask) {
			if (mask >= -1 && mask < 4) {
				app->entityManager->GetPlayer()->primaryMask = static_cast<Mask>(mask + 1);
				std::cout << "Comando SET_PRIMARY_MASK ejecutado con " << mask << " como mascara primaria" << std::endl;
			} else {
				std::cout << "Valor de mascara no valido (NOMASK = -1 | MASK0 = 0 | MASK1 = 1 | MASK2 = 2 | MASK3 = 3)" << std::endl;
			}
		});
	commandList.Add(SET_PRIMARY_MASK);

	SET_SECONDARY_MASK = new DebugCommandArg<int>("set_secondary_mask", "Establece la mascara secundaria del jugador", "set_secondary_mask <int mask>", [this](int mask) {
			if (mask >= -1 && mask < 4) {
				app->entityManager->GetPlayer()->secondaryMask = static_cast<Mask>(mask + 1);
				std::cout << "Comando SET_SECONDARY_MASK ejecutado con " << mask << " como mascara secundaria" << std::endl;
			} else {
				std::cout << "Valor de mascara no valido (NOMASK = -1 | MASK0 = 0 | MASK1 = 1 | MASK2 = 2 | MASK3 = 3)" << std::endl;
			}
		});
	commandList.Add(SET_SECONDARY_MASK);

	PLAY_COMMERCE = new DebugCommandArg<int>("play_commerce", "Abre una tienda", "play_commerce <int commerceID>", [this](int id) {
			app->commerceManager->PlayCommerce(id);
		});
	commandList.Add(PLAY_COMMERCE);


	GOTO = new DebugCommandArg<int>("goto", "Te cambia de escena por la elegida", "goto <int nivel>", [this](int nivel) {

		switch (nivel)
		{

		case -4:				app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_gameover);		app->menu->active = false;			break;
		case -3:				app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_menu);			app->menu->active = false;			break;
		case -2:	case 1000:	app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_testing);		app->menu->active = true;			break;
		case -1:	case 1001:	app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_pueblo);		app->menu->active = true;			break;
		case 0:					app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra0);		app->menu->active = true; app->ascensor->mazmorraActual = 0; if (!app->inventoryManager->dungeon0Entered) app->inventoryManager->dungeon0Entered = true;			break;
		case 1:					app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra1);		app->menu->active = true;	app->ascensor->mazmorraActual = 1;		break;
		case 2:					app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra2);		app->menu->active = true;	app->ascensor->mazmorraActual = 2;		break;
		case 3:					app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra3);		app->menu->active = true;	app->ascensor->mazmorraActual = 3;		break;
		case 4:					app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra4);		app->menu->active = true;	app->ascensor->mazmorraActual = 4;		break;
		case 5:					app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra5);		app->menu->active = true;	app->ascensor->mazmorraActual = 5;		break;
		case 6:					app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra6);		app->menu->active = true;	app->ascensor->mazmorraActual = 6;		break;
		case 7:					app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra7);		app->menu->active = true;	app->ascensor->mazmorraActual = 7;		break;
		case 8:					app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_mazmorra8);		app->menu->active = true;	app->ascensor->mazmorraActual = 8;		break;
		case 9:					app->fadeToBlack->FadeToBlack(app->fadeToBlack->activeScene, app->scene_logos);			app->menu->active = true;												break;

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
		SDL_Rect textRect = { 10, 7, surface->w, surface->h }; // Posici�n del texto
		SDL_RenderCopy(app->render->renderer, texture, NULL, &textRect);
		SDL_FreeSurface(surface);
		//app->tex->UnLoad(texture);
		SDL_DestroyTexture(texture);

		if (showHelp) {
			int y = 40;
			int x = 0;
			app->render->DrawRectangle(SDL_Rect{ 0, y, (int)windowW, (int)windowH }, 0, 0, 0, 220, true, false);

			for (int i = 0; i < commandList.Count(); i++)
			{
				DebugCommandBase* command = dynamic_cast<DebugCommandBase*>(commandList[i]);
				std::string label = command->GetCommandFormat() + " - " + command->GetCommandDescription();

				// Calcular posici�n de la etiqueta
				int labelX = 5;
				int labelY = y + i * 30;

				// Renderizar la etiqueta
				SDL_Surface* surface = TTF_RenderText_Blended(app->render->consoleFont, label.c_str(), textColor);
				SDL_Texture* texture = SDL_CreateTextureFromSurface(app->render->renderer, surface);
				int texW, texH;
				SDL_QueryTexture(texture, nullptr, nullptr, &texW, &texH);
			

				// Definir el rect�ngulo de destino
				SDL_Rect dstRect = { labelX, labelY - scrollY, texW, texH };

				// Renderizar la textura en la pantalla
				SDL_RenderCopy(app->render->renderer, texture, nullptr, &dstRect);

				// Liberar la textura y la superficie
				//app->tex->UnLoad(texture);
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
	std::transform(currentCommand.begin(), currentCommand.end(), currentCommand.begin(), ::tolower);
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
				// El puntero se ha convertido con �xito
				std::string commandArgId = commandArg->GetCommandId();
				if (currentCommand.find(commandArgId) != std::string::npos) {
					// Ejecutar el comando con el argumento entero

					if (properties.size() > 1) {
						try {

							int argValue = std::stoi(properties[1]); // Convertir la cadena a entero
							commandArg->Invoke(argValue);
						}
						catch (const std::invalid_argument& e) {
							std::cerr << "Error: Argumento no v�lido. " << e.what() << std::endl;
						}
						catch (const std::out_of_range& e) {
							std::cerr << "Error: Valor fuera de rango. " << e.what() << std::endl;
						}
						catch (const std::exception& e) {
							std::cerr << "Error: Ocurri� una excepci�n. " << e.what() << std::endl;
						}
						catch (...) {
							std::cerr << "Error: Ocurri� una excepci�n desconocida." << std::endl;
						}
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
		// Actualizar start para el pr�ximo substring
		start = end + 1;
		// Buscar el siguiente delimitador
		end = input.find(delimiter, start);
	}

	// Agregar la �ltima subcadena
	result.push_back(input.substr(start));

	return result;
}


