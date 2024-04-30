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

	void PopUpMessage();



private:
	char* hudTexturePath;
	char* messageTexturePath;
	SDL_Texture* hudTexture;
	SDL_Texture* messageTexture;
	int cantidadMonedas = 0;

	SDL_Rect* rectBarraVida;
	SDL_Rect* rectFondoBarraVida;

	SDL_Rect* rectFondoMascara;
	SDL_Rect* rectFondoMascaraSecundaria;
	SDL_Rect* rectFondoPociones;
	SDL_Rect* rectFondoHabilidad1;
	SDL_Rect* rectFondoHabilidad2;
	SDL_Rect* rectFondoInventario;
	SDL_Rect* rectFondoMonedas;
	SDL_Rect* rectFondoObjetosConseguidos;

	SDL_Rect* rectMascara0;
	SDL_Rect* rectMascara1;
	SDL_Rect* rectMascara2;
	SDL_Rect* rectMascara3;
	SDL_Rect* rectMascara4;

	SDL_Rect* rectBotonPlaceholder;
	SDL_Rect* rectBotonTAB;
	SDL_Rect* rectBotonQ;
	


	uint windowWidth, windowHeight;

};
#endif // __HUD_H__