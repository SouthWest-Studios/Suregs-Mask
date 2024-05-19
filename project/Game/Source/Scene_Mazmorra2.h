#ifndef __Scene_Mazmorra2_H__
#define __Scene_Mazmorra2_H__

#include "Module.h"
#include "Player.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
struct SDL_Texture;

class Scene_Mazmorra2 : public Module
{
public:

	Scene_Mazmorra2(App* app, bool start_enabled = false);

	// Destructor
	virtual ~Scene_Mazmorra2();

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
	int sceneLevel = 2;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	SDL_Texture* mouseTileTex = nullptr;
	GuiControlButton* gcButtom;

public:



};

#endif // __Scene_Mazmorra2_H__