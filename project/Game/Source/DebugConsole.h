#ifndef __DEBUGCONSOLE_H__
#define __DEBUGCONSOLE_H__

#include "Module.h"
#include "Player.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "DebugCommand.h" 
#include "DebugCommandBase.h" 

struct SDL_Texture;

class DebugConsole : public Module
{
public:

	DebugConsole(App* app, bool start_enabled = true);

	// Destructor
	virtual ~DebugConsole();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();
	bool PostLateUpdate();

	// Called before quitting
	bool CleanUp();

	// Handles multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);


private:

	void HandleInput();

	std::vector<std::string> SplitString(const std::string& input, char delimiter);

public:
	DebugCommand* KILL_ALL;
	DebugCommand* HELP;

	DebugCommandArg<int>* SET_GOLD;

	List<DebugCommandBase*> commandList;


	int scrollY = 0;
	int scrollYSpeed = 20;

private:

	bool showConsole = false;
	bool showHelp = false;

	

	//SDL_Texture* img;
	//float textPosX, textPosY = 0;
	//uint texW, texH;
	uint windowW, windowH;

	std::string currentCommand;
	//SDL_Texture* mouseTileTex = nullptr;

	////L03: DONE 3b: Declare a Player attribute
	//

	//// L15: TODO 2: Declare a GUI Control Button 
	//GuiControlButton* gcButtom;

	//SDL_Texture* mapaFondo;
};

#endif // __DEBUGCONSOLE_H__