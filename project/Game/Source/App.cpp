#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "ParticleSystem.h"
#include "Audio.h"
#include "Scene_Testing.h"
#include "Scene_Intro.h"
#include "Scene_Logos.h"
#include "Scene_Menu.h"
#include "Scene_GameOver.h"
#include "Scene_Pueblo.h"
#include "Scene_Pueblo_Tutorial.h"
#include "Scene_Arena_Tutorial.h"
#include "Scene_Mazmorra0.h"
#include "Scene_Mazmorra1.h"
#include "Scene_Mazmorra2.h"
#include "Scene_Mazmorra3.h"
#include "Scene_Mazmorra4.h"
#include "Scene_Mazmorra5.h"
#include "Scene_Mazmorra6.h"
#include "Scene_Mazmorra7.h"
#include "Scene_Mazmorra8.h"
#include "Scene_Cinematic.h"
#include "Map.h"
#include "Physics.h"
#include "GuiManager.h"
#include "ModuleFadeToBlack.h"
#include "DebugConsole.h"
#include "DialogManager.h"
#include "InventoryManager.h"
#include "CommerceManager.h"
#include "MissionManager.h"
#include "NotesManager.h"
#include "BestiarioManager.h"
#include "Menu_Equipo.h"
#include "TreeManager.h"
#include "Menu.h"
#include "Hud.h"
#include "ElevatorMenu.h"
#include "QuestManager.h"
#include "Optick/include/optick.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	// L1: DONE 3: Measure the amount of ms that takes to execute the App constructor and LOG the result
	Timer timer = Timer();
	startupTime = Timer();
	frameTime = PerfTimer();
	lastSecFrameTime = PerfTimer();

	frames = 0;

	// L3: DONE 1: Add the EntityManager Module to App

	win = new Window(this);
	input = new Input(this);
	render = new Render(this);
	tex = new Textures(this);
	psystem = new ParticleSystem(this);
	audio = new Audio(this);
	//L07 DONE 2: Add Physics module
	physics = new Physics(this);
	fadeToBlack = new ModuleFadeToBlack(this);

	scene_logos = new Scene_Logos(this, true);
	scene_intro = new Scene_Intro(this, false);
	scene_menu = new Scene_Menu(this, false);
	scene_gameover = new Scene_GameOver(this, false);
	scene_testing = new Scene_testing(this, false);
	scene_arena_tutorial = new Scene_Arena_Tutorial(this, false);
	scene_pueblo_tutorial = new Scene_Pueblo_Tutorial(this, false);
	scene_pueblo = new Scene_Pueblo(this, false);
	scene_cinematic= new Scene_Cinematic(this, false);

	scene_mazmorra0 = new Scene_Mazmorra0(this, false);
	scene_mazmorra1 = new Scene_Mazmorra1(this, false);
	scene_mazmorra2 = new Scene_Mazmorra2(this, false);
	scene_mazmorra3 = new Scene_Mazmorra3(this, false);
	scene_mazmorra4 = new Scene_Mazmorra4(this, false);
	scene_mazmorra5 = new Scene_Mazmorra5(this, false);
	scene_mazmorra6 = new Scene_Mazmorra6(this, false);
	scene_mazmorra7 = new Scene_Mazmorra7(this, false);
	scene_mazmorra8 = new Scene_Mazmorra8(this, false);

	map = new Map(this, false);
	entityManager = new EntityManager(this);
	inventoryManager = new InventoryManager(this);
	commerceManager = new CommerceManager(this);
	missionManager = new MissionManager(this);
	guiManager = new GuiManager(this);
	debugConsole = new DebugConsole(this);
	dialogManager = new DialogManager(this);
	menu = new Menu(this, false);
	hud = new Hud(this, false);
	notesManager = new NotesManager(this);
	bestiarioManager = new BestiarioManager(this);
	treeManager = new TreeManager(this);
	menuEquipo = new Menu_Equipo(this);
	ascensor = new ElevatorMenu(this, true);
	questManager = new QuestManager(this, true);

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(tex);

	AddModule(audio);
	//L07 DONE 2: Add Physics module
	AddModule(physics);

	AddModule(map);

	AddModule(scene_logos);
	AddModule(scene_intro);
	AddModule(scene_menu);
	AddModule(scene_testing);
	AddModule(scene_pueblo_tutorial);
	AddModule(scene_arena_tutorial);
	AddModule(scene_pueblo);
	AddModule(scene_cinematic);
	AddModule(scene_mazmorra0);
	AddModule(scene_mazmorra1);
	AddModule(scene_mazmorra2);
	AddModule(scene_mazmorra3);
	AddModule(scene_mazmorra4);
	AddModule(scene_mazmorra5);
	AddModule(scene_mazmorra6);
	AddModule(scene_mazmorra7);
	AddModule(scene_mazmorra8);
	AddModule(scene_gameover);
	AddModule(entityManager);
	AddModule(missionManager);
	AddModule(hud);
	AddModule(menu);
	AddModule(inventoryManager);
	AddModule(commerceManager);
	AddModule(questManager);
	AddModule(notesManager);
	AddModule(bestiarioManager);
	AddModule(menuEquipo);
	AddModule(ascensor);
	AddModule(treeManager);
	AddModule(guiManager);
	AddModule(dialogManager);
	AddModule(fadeToBlack);

	// Render last to swap buffer
	AddModule(debugConsole);
	AddModule(render);
	AddModule(psystem);
	
	srand(static_cast<unsigned int>(time(0)));

	LOG("Timer App Constructor: %f", timer.ReadMSec());
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();
}

void App::AddModule(Module* module)
{
	/*module->Init();*/
	modules.Add(module);
}

// Called before render is available
bool App::Awake()
{
	// L1: DONE 3: Measure the amount of ms that takes to execute the Awake and LOG the result
	Timer timer = Timer();

	bool ret = LoadConfig();

	if (ret == true)
	{
		// L04: DONE 3: Read the title from the config file and set the windows title 
		// substitute "Video Game Template" string from the value of the title in the config file
		// also read maxFrameDuration 
		gameTitle.Create(configFile.child("config").child("app").child("title").child_value());
		win->SetTitle(gameTitle.GetString());
		maxFrameDuration = configFile.child("config").child("app").child("maxFrameDuration").attribute("value").as_int();

		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			// L04: DONE 4: Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml

			ret = item->data->Awake(configFile.child("config").child(item->data->name.GetString()));
			item = item->next;
		}
	}

	LOG("Timer App Awake(): %f", timer.ReadMSec());

	return ret;
}

// Called before the first frame
bool App::Start()
{
	// L1: DONE 3: Measure the amount of ms that takes to execute the App Start() and LOG the result
	Timer timer = Timer();

	srand(time(nullptr)); //Establece un numero random al iniciar la aplicaciones segun el tiempo

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		if (item->data->active) {
			ret = item->data->Start();
		}
		item = item->next;
	}

	LOG("Timer App Start(): %f", timer.ReadMSec());

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	//L16 TODO 2: Add the Optick macro to mark the beginning of the main loop
	OPTICK_FRAME("Main Loop");

	bool ret = true;
	PrepareUpdate();

	if (input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if (ret == true)
		ret = PreUpdate();


	if (ret == true)
		ret = DoUpdate();


	if (ret == true)
		ret = PostUpdate();



	if (ret == true)
		ret = PostLateUpdate();

	FinishUpdate();
	if (!ret) return ret;
	return !closeApplication;
	return ret;


}

// Load config from XML file
bool App::LoadConfig()
{
	bool ret = true;

	// L04: DONE 2: Load config.xml file using load_file() method from the xml_document class
	// If the result is ok get the main node of the XML
	// else, log the error
	// check https://pugixml.org/docs/quickstart.html#loading

	pugi::xml_parse_result result = configFile.load_file("config.xml");
	if (result)
	{
		LOG("config.xml parsed without errors");
	}
	else
	{
		LOG("Error loading config.xml: %s", result.description());
	}

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	// L16 TODO 3: Add OPTICK macro to functions to include in the profiling
	OPTICK_EVENT();
	frameTime.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	// L16 TODO 3: Add OPTICK macro to functions to include in the profiling
	OPTICK_EVENT();

	// This is a good place to call Load / Save functions

	// L02: DONE 1: Cap the framerate of the gameloop
	// L02: DONE 2: Measure accurately the amount of time SDL_Delay() actually waits compared to what was expected

	double currentDt = frameTime.ReadMs();
	if (maxFrameDuration > 0 && currentDt < maxFrameDuration) {
		uint32 delay = (uint32)(maxFrameDuration - currentDt);

		PerfTimer delayTimer = PerfTimer();
		SDL_Delay(delay);
		//LOG("We waited for %I32u ms and got back in %f ms",delay,delayTimer.ReadMs());
	}


	// L1: DONE 4: Calculate:
	// Amount of frames since startup
	frameCount++;

	// Amount of time since game start (use a low resolution timer)
	secondsSinceStartup = startupTime.ReadSec();

	// Amount of ms took the last update (dt)
	dt = (float)frameTime.ReadMs();

	// Amount of frames during the last second
	lastSecFrameCount++;

	// Average FPS for the whole game life
	if (lastSecFrameTime.ReadMs() > 1000) {
		lastSecFrameTime.Start();
		averageFps = (averageFps + lastSecFrameCount) / 2;
		framesPerSecond = lastSecFrameCount;
		lastSecFrameCount = 0;
	}

	if (tiempoDeJuego < 359940000)
	{
		tiempoDeJuego += dt;
	}

	convertirTiempo(tiempoDeJuego);

	////printf("tiempo de juego: %f\n", tiempoDeJuego);


	// Shows the time measurements in the window title
	// check s//printf formats here https://cplusplus.com/reference/cstdio///printf/
	static char title[256];
	sprintf_s(title, 256, "%s: Av.FPS: %.2f Last sec frames: %i Last dt: %.3f Time since startup: %I32u Frame Count: %I64u ",
		gameTitle.GetString(), averageFps, framesPerSecond, dt, secondsSinceStartup, frameCount);

	app->win->SetTitle(title);

	if (loadRequest) {
		loadRequest = false;
		LoadFromFile();
	}

	if (saveRequest) {
		saveRequest = false;
		SaveFromFile();
	}
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	// L16 TODO 3: Add OPTICK macro to functions to include in the profiling
	OPTICK_EVENT();

	bool ret = true;

	ListItem<Module*>* item;
	Module* pModule = NULL;
	item = modules.start;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		
		pModule = item->data;

		LOG("PREUPDATE DE: %s", pModule->name.GetString());

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}


	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	// L16 TODO 3: Add OPTICK macro to functions to include in the profiling
	OPTICK_EVENT();

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;


	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		/*LOG("UPDATE DE: %s", pModule->name.GetString());*/

		if (pModule->active == false) {
			continue;
		}
		
		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	// L16 TODO 3: Add OPTICK macro to functions to include in the profiling
	OPTICK_EVENT();

	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;
	item = modules.start;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

bool App::PostLateUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;
	item = modules.start;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (!pModule->active) {
			continue;
		}

		ret = item->data->PostLateUpdate();
	}




	return true;
}

// Called before quitting
bool App::CleanUp()
{
	// L1: DONE 3: Measure the amount of ms that takes to execute the App CleanUp() and LOG the result
	Timer timer = Timer();

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}


	LOG("Timer App CleanUp(): %f", timer.ReadMSec());

	return ret;
}

std::string App::convertirTiempo(long long milisegundos)
{
	long long totalSegundos = milisegundos / 1000; // Convertir milisegundos a segundos
	int horas = totalSegundos / 3600;              // 1 hora = 3600 segundos
	int minutos = (totalSegundos % 3600) / 60;     // 1 minuto = 60 segundos
	int segundos = totalSegundos % 60;             // Segundos restantes


	char buffer[9]; // hh:mm:ss + null terminator
	sprintf_s(buffer, "%02d:%02d:%02d", horas, minutos, segundos);

	return std::string(buffer);
	// Imprimir en formato hh:mm:ss
	////printf("%02d:%02d:%02d\n", horas, minutos, segundos);
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return gameTitle.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

pugi::xml_node App::LoadEmitters(pugi::xml_document& psystem_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = psystem_file.load_file("psystem_config.xml");

	if (result == NULL)
		LOG("Could not load xml file config.xml. Pugi error: %s", result.description());
	else
		ret = psystem_file.child("emitters");
	return ret;
}

// L14: TODO 1: Implement the methods LoadRequest() and SaveRequest() to request and call the Load / Save the game state at the end of the frame
// The real execution of load / save will be implemented in TODO 5 and 7

// Request a save data in an XML file 
bool App::LoadRequest() {

	bool ret = true;
	loadRequest = true;
	return ret;
}

// Request to load data from XML file 
bool App::SaveRequest() {
	bool ret = true;
	saveRequest = true;
	return true;
}

bool App::IsSaving()
{
	return isSaving;
}


// L03: TODO 5: Implement the method LoadFromFile() to actually load a xml file
// then call all the modules to load themselves
bool App::LoadFromFile() {

	bool ret = true;

	pugi::xml_document saveFile;
	pugi::xml_parse_result result;

	if (savedGame == 1) {
		result = saveFile.load_file("save_game.xml");
	}
	if (savedGame == 2) {
		result = saveFile.load_file("save_game2.xml");
	}
	if (savedGame == 3) {
		result = saveFile.load_file("save_game3.xml");
	}


	if (result)
	{
		LOG("save_game.xml parsed without errors");

		// Iterates all modules and call the load of each with the part of the XML node that corresponds to the module
		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			if (item->data->active) {
				ret = item->data->LoadState(saveFile.child("game_state").child(item->data->name.GetString()));
			}
			item = item->next;
		}

	}
	else
	{
		LOG("Error loading save_game.xml: %s", result.description());
		ret = false;
	}



	return ret;

}

// L03: TODO 7: Implement the xml save method SaveToFile() for current state
// check https://pugixml.org/docs/quickstart.html#modify
bool App::SaveFromFile() {

	//COMENTARIO PARA NURIA
	//En las lineas gameState.child("saveX"). Esa parte no funciona, ya que gameState no tiene ningun hijo llamado saveX. Por eso no guarda nada.
	//Al guardar, se vuelcan todos los datos nuevos en el archivo, borrando los anteriores, se complica para tener varios saves en el mismo archivo.
	//Para hacerlo facil deberian de haber: save_game1.xml, save_game2.xml y save_game3.xml. Y hacer solo el switch al final en 
	//"ret = saveFile.save_file("save_game.xml");" en vez de duplicar el mismo codigo para todo.
	//Y en principio no tendria que haber ningun problema.
	//Sigue faltando la seleccion de nueva partida para saber en que slot guardar...

	bool ret = true;
	isSaving = true;


	pugi::xml_document saveFile;
	pugi::xml_node gameState = saveFile.append_child("game_state");

	// Iterates all modules and call the save of each with the part of the XML node that corresponds to the module
	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		if (item->data->active) {
			pugi::xml_node module = gameState.append_child(item->data->name.GetString());
			ret = item->data->SaveState(module);
		}
		item = item->next;
	}

	if (savedGame == 1) {
		ret = saveFile.save_file("save_game.xml");
	}

	if (savedGame == 2) {
		ret = saveFile.save_file("save_game2.xml");
	}

	if (savedGame == 3) {
		ret = saveFile.save_file("save_game3.xml");
	}

	if (ret) LOG("Saved");
	else LOG("Error saving game state");

	isSaving = false;

	return ret;

}
