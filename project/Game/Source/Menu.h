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

	GuiControlSlider* music = nullptr;
	GuiControlSlider* sfx = nullptr;
	int ventana = 1;

	pugi::xml_node config;

private:
	SDL_Texture* fondoInventario = nullptr;
	SDL_Texture* fondoEquipo = nullptr;
	SDL_Texture* fondoDiario = nullptr;
	SDL_Texture* fondoAjustes = nullptr;
	
	int a = 0;
	
	int contadormenu = 0;
	bool menuusettings = false;
	bool fullScreenActive = false;
	


	GuiControlButton* gcButtom = nullptr;
	GuiControlButton* exit = nullptr;
	GuiCheckBox* fullScreen = nullptr;
	GuiCheckBox* vsync = nullptr;
	GuiControlButton* title = nullptr;
	GuiControlButton* cruz = nullptr;
	

	int inventory_audio;
	int change_inventory_audio;

	int newVolumeAudio;
	int newVolumeFx;
};

#endif // __MENU_H__