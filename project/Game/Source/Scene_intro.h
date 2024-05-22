#ifndef __SCENE_INTRO_H__
#define __SCENE_INTRO_H__

#include "Module.h"
#include "Player.h"
#include "GuiControl.h"
#include "GuiControlButton.h"

struct SDL_Texture;

class Scene_Intro : public Module
{
public:

	Scene_Intro(App* app, bool start_enabled = true);

	// Destructor
	virtual ~Scene_Intro();

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
	const char* texturaIntroPath;
	SDL_Texture* texturaIntro;
	Timer timerIntro;

	Emitter* potionFlare = nullptr;
};

#endif // __SCENE_INTRO_H__