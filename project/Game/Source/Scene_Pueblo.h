#ifndef __Scene_Pueblo_H__
#define __Scene_Pueblo_H__

#include "Module.h"
#include "Player.h"
#include "GuiControl.h"
#include "GuiControlButton.h"

struct SDL_Texture;

class Scene_Pueblo : public Module
{
public:

	Scene_Pueblo(App* app, bool start_enabled = false);

	// Destructor
	virtual ~Scene_Pueblo();

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

	
	MiniGameFishing* GetRod();

private:

	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	SDL_Texture* mouseTileTex = nullptr;
	

	//L03: DONE 3b: Declare a Player attribute
	

	// L15: TODO 2: Declare a GUI Control Button 
	GuiControlButton* gcButtom;

public:
	MiniGameFishing* fishing = new MiniGameFishing;

	PhysBody* pbody;
};

#endif // __Scene_Pueblo_H__