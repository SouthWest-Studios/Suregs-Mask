#ifndef __SCENE_MENU_H__
#define __SCENE_MENU_H__

#include "Module.h"
#include "Player.h"
#include "GuiControl.h"
#include "GuiControlButton.h"

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

	SDL_Texture* placeholderMenu;
	SDL_Texture* placeholderSettings;
	SDL_Texture* placeholderCredits;

	List<GuiControl*> controlsScene;
	List<GuiControl*> controlsSettings;
	GuiControl* gcCloseCredits;

private:

	uint windowW, windowH;
	

	bool showSettings = false;
	bool _showSettings = false;

	bool showCredits = false;
	bool _showCredits = false;
};

#endif // __SCENE_MENU_H__