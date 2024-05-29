#ifndef __SCENE_PUEBLO_TUTORIAL_H__
#define __SCENE_PUEBLO_TUTORIAL_H__

#include "Module.h"
#include "Player.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "Particle.h"
#include "ParticlePool.h"
#include "Emitter.h"


struct SDL_Texture;

class Scene_Pueblo_Tutorial : public Module
{
public:

	Scene_Pueblo_Tutorial(App* app, bool start_enabled = false);

	// Destructor
	virtual ~Scene_Pueblo_Tutorial();

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

	// Handles multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

private:

	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	SDL_Texture* mouseTileTex = nullptr;

	int town_fx;
	

	//L03: DONE 3b: Declare a Player attribute
	

	// L15: TODO 2: Declare a GUI Control Button 
	GuiControlButton* gcButtom;

public:

	PhysBody* pbody;
};

#endif // __SCENE_PUEBLO_TUTORIAL_H__