#ifndef __MENU_H__
#define __MENU_H__

#include "Module.h"
#include "Player.h"
#include "GuiControl.h"
#include "GuiControlButton.h"


struct SDL_Texture;

class Menu : public Module
{
public:

	Menu(App* app, bool start_enabled = true);

	// Destructor
	virtual ~Menu();

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

	// Called before quitting
	bool CleanUp();

private:
	SDL_Texture* fondo;
	
	bool visible = true;

	bool menuu = false;
	int contadormenu = 0;
	bool menuusettings = false;

	GuiControlButton* gcButtom;
	GuiControlButton* exit;
	GuiControlButton* settings;
	GuiControlButton* title;
	GuiControlButton* cruz;
};

#endif // __MENU_H__