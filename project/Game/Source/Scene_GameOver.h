#ifndef __SCENE_GAMEOVER_H__
#define __SCENE_GAMEOVER_H__

#include "Module.h"
#include "Player.h"
#include "GuiControl.h"
#include "GuiControlButton.h"

struct SDL_Texture;

class Scene_GameOver : public Module
{
public:

	Scene_GameOver(App* app, bool start_enabled = true);

	// Destructor
	virtual ~Scene_GameOver();

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

	void Fullscreen();

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

	SDL_Texture* gameOverBackground;
	SDL_Texture* logo;

	const char* gameOverBackground_tp;
	const char* logo_tp;

private:

	GuiControlButton* VolverAlMenu;
	GuiControlButton* Continuar;

	uint windowW, windowH;

};

#endif // __SCENE_GAMEOVER_H__