#ifndef __SCENE_LOGOS_H__
#define __SCENE_LOGOS_H__

#include "Module.h"
#include "Player.h"
#include "GuiControl.h"
#include "GuiControlButton.h"

struct SDL_Texture;

class Scene_Logos : public Module
{
public:

	Scene_Logos(App* app, bool start_enabled = true);

	// Destructor
	virtual ~Scene_Logos();

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
	const char* logoGamePath;
	const char* logoUpcPath;
	const char* logoStudioPath;
	const char* backgroundPath;

	SDL_Texture* logoGame;
	SDL_Texture* logoUpc;
	SDL_Texture* logoStudio;
	SDL_Texture* background;

	Timer timerIntro;

	int intro_fx;
};

#endif // __SCENE_LOGOS_H__