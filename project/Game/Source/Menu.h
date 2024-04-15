#ifndef __MENU_H__
#define __MENU_H__

#include "Module.h"
#include "Player.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "GuiCheckBox.h"
#include "GuiControlSlider.h"


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

	bool vsyncActive = false;
	bool visible = true;
	bool menuu = false;

private:
	SDL_Texture* fondoInventario;
	SDL_Texture* fondoEquipo;
	SDL_Texture* fondoDiario;
	SDL_Texture* fondoAjustes;
	
	int a = 0;
	
	int contadormenu = 0;
	bool menuusettings = false;
	bool fullScreenActive = false;
	

	int ventana = 1;

	GuiControlButton* gcButtom;
	GuiControlButton* exit;
	GuiCheckBox* fullScreen;
	GuiCheckBox* vsync;
	GuiControlButton* title;
	GuiControlButton* cruz;
	GuiControlSlider* music;
	GuiControlSlider* sfx;

	int inventory_audio;
	int change_inventory_audio;
};

#endif // __MENU_H__