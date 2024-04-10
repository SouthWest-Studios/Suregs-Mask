#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "GuiControl.h"

#include "List.h"

class GuiManager : public Module
{
public:

	// Constructor
	GuiManager(App* app, bool start_enabled = true);

	// Destructor
	virtual ~GuiManager();

	// Called before the first frame
	 bool Start();

	 // Called each loop iteration
	 bool Update(float dt);

	 bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds = {0,0,0,0}, SDL_Rect bounds2 = { 0,0,0,0 }, int minValue = 0, int maxValue = 0 );

	void DestroyGuiControl(GuiControl* controlToDestroy);
	void NavigateUp();
	void NavigateDown();
	GuiControl* GetControlById(int id);

public:
	int maxId;
	int minId;

	int pointerId = 1;

	List<GuiControl*> guiControlsList;
	SDL_Texture* texture;

	int button_audio;

};

#endif // __GUIMANAGER_H__
