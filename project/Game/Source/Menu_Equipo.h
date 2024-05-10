#ifndef __EQUIPO_H__
#define __EQUIPO_H__

#include "Module.h"
#include "Player.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "GuiCheckBox.h"
#include "GuiControlSlider.h"



class Menu_Equipo : public Module
{
public:

	Menu_Equipo(App* app, bool start_enabled = true);

	// Destructor
	virtual ~Menu_Equipo();

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

	bool mostrar = false;

private:
	char* TexturePath;
	char* messageTexturePath;

	char* life;
	char* Maxlife;
	char* damage;
	char* speed;
	char* atackSpeed;
	SDL_Texture* Texture;

	SDL_Texture* messageTexture;
	

	uint windowWidth, windowHeight;

	int acuiredItemLifeTimeMS = 1500;

	

};
#endif // __HUD_H__