#ifndef __ELVATORMENU_H__
#define __ELVATORMENU_H__

#include "Module.h"
#include "Player.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "GuiCheckBox.h"
#include "GuiControlSlider.h"


struct SDL_Texture;

class ElevatorMenu : public Module
{
public:

	ElevatorMenu(App* app, bool start_enabled = true);

	// Destructor
	virtual ~ElevatorMenu();

	// Called before render is available
	bool Awake(pugi::xml_node conf);

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

	void ElevatorMenu::OnMovePointer();



	pugi::xml_node config;

	bool abierto = false;

	iPoint PointerPosition = { 400, -90 };
	int PointerId = 0;
	SDL_Texture* texture = NULL;
	SDL_Texture* Menutexture = NULL;
	SDL_Texture* listTexture = NULL;
	SDL_Texture* PointerTexture = NULL;

private:
	
};

#endif // __MENU_H__