#ifndef __SCENE_MENU_H__
#define __SCENE_MENU_H__

#include "Module.h"
#include "Player.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "GuiCheckBox.h"
#include "GuiControlSlider.h"

struct SDL_Texture;

class Scene_Menu : public Module
{
public:

	Scene_Menu(App* app, bool start_enabled = true);

	// Destructor
	virtual ~Scene_Menu();

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

	bool OnGuiMouseClickEvent(GuiControl* control);

	void SettingsInterface();
	void ShowCredits();
	void DestroySettingsInterface();

	void Fullscreen();

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

	SDL_Texture* menuMain;
	SDL_Texture* menuMain2;
	SDL_Texture* settings;
	SDL_Texture* credits;
	SDL_Texture* logo;

	const char* menuMain_tp;
	const char* menuMain2_tp;
	const char* settings_tp;
	const char* credits_tp;
	const char* logo_tp;

	List<GuiControl*> controlsScene;
	List<GuiControl*> controlsSettings;
	GuiControl* gcCloseCredits;
	GuiCheckBox* fullscreen;
	GuiCheckBox* vsync;

private:

	GuiControlButton* NuevaPartida;
	GuiControlButton* Continuar;

	GuiControlButton* gcButtom;
	GuiControlButton* exit;
	GuiControlButton* atras;
	GuiControlSlider* music;
	GuiControlSlider* sfx;

	bool vsyncActive = false;

	uint windowW, windowH;

	int menu_fx;
	
	bool fullScreenActive = false;

	bool showSettings = false;
	bool _showSettings = false;

	bool ajustes = false;

	bool showCredits = false;
	bool _showCredits = false;
};

#endif // __SCENE_MENU_H__