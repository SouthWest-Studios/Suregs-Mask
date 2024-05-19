#ifndef __Scene_Mazmorra1_H__
#define __Scene_Mazmorra1_H__

#include "Module.h"
#include "Player.h"
#include "GuiControl.h"
#include "GuiControlButton.h"

struct SDL_Texture;

class Scene_Mazmorra1 : public Module
{
public:

	Scene_Mazmorra1(App* app, bool start_enabled = false);

	// Destructor
	virtual ~Scene_Mazmorra1();

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

	int GetSceneNumber();

	
	
private:
int sceneLevel = 1;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	SDL_Texture* mouseTileTex = nullptr;
	

	//L03: DONE 3b: Declare a Player attribute
	

	// L15: TODO 2: Declare a GUI Control Button 
	GuiControlButton* gcButtom;


};

#endif // __Scene_Mazmorra1_H__