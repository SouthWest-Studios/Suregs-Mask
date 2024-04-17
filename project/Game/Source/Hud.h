#ifndef __HUD_H__
#define __HUD_H__

#include "Module.h"
#include "Player.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "GuiCheckBox.h"
#include "GuiControlSlider.h"


struct SDL_Texture;

class Hud : public Module
{
public:

	Hud(App* app, bool start_enabled = true);

	// Destructor
	virtual ~Hud();

	// Called before render is available
	bool Awake(pugi::xml_node conf);

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
	SDL_Texture* texturaMoneda;
	int cantidadMonedas = 0;

};
#endif // __HUD_H__