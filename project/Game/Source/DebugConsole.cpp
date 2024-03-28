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
	if(!active){
		return ret;
	}

	return ret;
}

// Called before the first frame
bool DebugConsole::Start()
{

	app->win->GetWindowSize(windowW, windowH);


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
		app->render->DrawRectangle(SDL_Rect{0,0, (int)windowW, 40 }, 0,0,0, 100, true, false);
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


