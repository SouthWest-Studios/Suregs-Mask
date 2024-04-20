#ifndef __Scene_testing_H__
#define __Scene_testing_H__

#include "Module.h"
#include "Player.h"
#include "GuiControl.h"
#include "GuiControlButton.h"

#include "MiniGameFishing.h"

struct SDL_Texture;

class Scene_testing : public Module
{
public:

	Scene_testing(App* app, bool start_enabled = false);

	// Destructor
	virtual ~Scene_testing();

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

	// Return the player position
	iPoint GetPLayerPosition();

	// Handles multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	MiniGameFishing* GetRod();

public:
	MiniGameFishing* fishing = new MiniGameFishing;
	SDL_Texture* mouseTileTex = nullptr;
private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;

	//L03: DONE 3b: Declare a Player attribute
	

	// L15: TODO 2: Declare a GUI Control Button 
	GuiControlButton* gcButtom;

	SDL_Texture* mapaFondo;
};

#endif // __Scene_testing_H__