#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"
#include "PerfTimer.h"
#include "Timer.h"
#include "EntityManager.h"

#include "PugiXml/src/pugixml.hpp"

// L03: DONE 1: Add the EntityManager Module to App

// Modules
class Window;
class Input;
class Render;
class Textures;
class ParticleSystem;
class Audio;
class ModuleFadeToBlack;
class Scene_Logos;
class Scene_Intro;
class Scene_Menu;
class Scene_GameOver;
class Scene_testing;
class Scene_Pueblo;
class Scene_Mazmorra0;
class Scene_Mazmorra1;
class Scene_Mazmorra2;
class Scene_Mazmorra3;
class Scene_Mazmorra4;
class Scene_Mazmorra5;
class Scene_Mazmorra6;
class Scene_Mazmorra7;
class EntityManager;
class Map;
//L07 DONE 2: Add Physics module
class Physics;
class GuiManager;
class DebugConsole;
class DialogManager;
class InventoryManager;
class CommerceManager;
class MissionManager;
class NotesManager;
class BestiarioManager;
class Menu;
class Hud;
class TreeManager;
class Menu_Equipo;


class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	pugi::xml_node App::LoadEmitters(pugi::xml_document& psystem_file) const;

	// L14: TODO 1: Declare the methods LoadRequest() and SaveRequest() to request and call the Load / Save the game state at the end of the frame

	// Request a save data in an XML file 
	bool LoadRequest();

	// Request to load data from XML file 
	bool SaveRequest();

private:

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();
	bool PostLateUpdate();

	// Reads XML file and loads the data
	bool LoadFromFile();

	// Sace XML file with modules data
	bool SaveFromFile();


public:

	// L03: DONE 1: Add the EntityManager Module to App

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	ParticleSystem* psystem;
	Audio* audio;
	ModuleFadeToBlack* fadeToBlack;

	Scene_Intro* scene_intro;
	Scene_Logos* scene_logos;
	Scene_Menu* scene_menu;
	Scene_GameOver* scene_gameover;
	Scene_testing* scene_testing;
	Scene_Pueblo* scene_pueblo;

	Scene_Mazmorra0* scene_mazmorra0;
	Scene_Mazmorra1* scene_mazmorra1;
	Scene_Mazmorra2* scene_mazmorra2;
	Scene_Mazmorra3* scene_mazmorra3;
	Scene_Mazmorra4* scene_mazmorra4;
	Scene_Mazmorra5* scene_mazmorra5;
	Scene_Mazmorra6* scene_mazmorra6;
	Scene_Mazmorra7* scene_mazmorra7;

	EntityManager* entityManager;
	Map* map;
	//L07 DONE 2: Add Physics module
	Physics* physics;
	GuiManager* guiManager;
	DebugConsole* debugConsole;
	DialogManager* dialogManager;
	InventoryManager* inventoryManager;
	CommerceManager* commerceManager;
	MissionManager* missionManager;
	NotesManager* notesManager;
	BestiarioManager* bestiarioManager;
	TreeManager* treeManager;

	Menu* menu;
	Hud* hud;
	Menu_Equipo* menuEquipo;
	

	bool closeApplication = false;
	bool fullscreen = false;

	float dt;

private:

	int argc;
	char** args;
	SString gameTitle;
	SString organization;

	List<Module *> modules;

	// L04: DONE 2 - Create a variable to load and store the XML file in memory
	// xml_document to store the config file
	pugi::xml_document configFile;
	
	uint frames;
	

	// L1: DONE 4: Calculate some timing measures
    // required variables are provided:
	Timer startupTime;
	PerfTimer frameTime;
	PerfTimer lastSecFrameTime;

	uint64 frameCount = 0;
	uint32 framesPerSecond = 0;
	uint32 lastSecFrameCount = 0;

	float averageFps = 0.0f;
	uint32 secondsSinceStartup = 0;

	//L02 DONE 1: Set the maximun frame duration in miliseconds.
	uint32 maxFrameDuration = 16;

	//
	bool loadRequest = false;
	bool saveRequest = false;

};

extern App* app;

#endif	// __APP_H__