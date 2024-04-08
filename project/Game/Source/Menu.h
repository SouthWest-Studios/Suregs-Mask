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

	bool OnGuiMouseClickEvent(GuiControl* control);

	void SettingsInterface();
	void ShowSettingsInterface();
	void ShowCredits();
	void DestroySettingsInterface();

	void Fullscreen();

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

private:
	SDL_Texture* fondoInventario;
	SDL_Texture* fondoEquipo;
	SDL_Texture* fondoDiario;
	SDL_Texture* fondoAjustes;
	
	bool visible = true;

	bool menuu = false;
	int contadormenu = 0;
	bool menuusettings = false;

	int ventana = 1;

	GuiControlButton* gcButtom;
	GuiControlButton* exit;
	GuiControlButton* settings;
	GuiControlButton* title;
	GuiControlButton* cruz;
};

#endif // __MENU_H__