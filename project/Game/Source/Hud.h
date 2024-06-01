#ifndef __HUD_H__
#define __HUD_H__

#include "Module.h"
#include "Player.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "GuiCheckBox.h"
#include "GuiControlSlider.h"
#include "inventity.h"

struct SDL_Texture;

struct Acquired_Item {
	SDL_Texture* texture;
	std::string text;
	Timer lifeTimer;
};

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

	void AcquiredItemTrigger(SDL_Texture* texture, std::string text);

	void Potions();
	bool HayPocionesDisponibles();
	bool IsPotion(InventityType type);
	ListItem<Inventity*>* GetSelectedPotionItem();

private:
	char* hudTexturePath;
	char* messageTexturePath;
	char* cdMaskTexturePath;
	SDL_Texture* hudTexture;
	SDL_Texture* messageTexture;
	SDL_Texture* cdPrimaryMaskTexture;
	int rectYPrimaryMask;
	int rectYSecondaryMask;
	SDL_Texture* cdSecondaryMaskTexture;
	int cantidadMonedas = 0;

	SDL_Rect* rectBarraVida;
	SDL_Rect* rectFondoBarraVida;

	SDL_Rect* rectFondoMascara;
	SDL_Rect* rectFondoMascaraSecundaria;

	SDL_Rect* rectCDMascaraPrimaria;
	SDL_Rect* rectCDMascaraSecundaria;

	SDL_Rect* rectFondoPociones;
	SDL_Rect* rectPocionVida1;
	SDL_Rect* rectPocionVida2;
	SDL_Rect* rectPocionVida3;
	SDL_Rect* rectPocionVidaMax;
	SDL_Rect* rectPocionRegeneracion;
	SDL_Rect* rectPocionDano;
	SDL_Rect* rectPocionVelocidad;
	SDL_Rect* rectPocionOrbe;

	//SDL_Rect* rectFondoHabilidad1;
	//SDL_Rect* rectFondoHabilidad2;
	SDL_Rect* rectFondoInventario;
	SDL_Rect* rectFondoMonedas;
	SDL_Rect* rectFondoObjetosConseguidos;

	SDL_Rect* rectMascaraNoMask;
	SDL_Rect* rectMascara0;
	SDL_Rect* rectMascara1;
	SDL_Rect* rectMascara2;
	SDL_Rect* rectMascara3;

	SDL_Rect* rectBotonPlaceholder;
	SDL_Rect* rectBotonTAB;
	SDL_Rect* rectBotonQ;
	


	uint windowWidth, windowHeight;

	std::vector<Acquired_Item*> acquired_Items;
	int acuiredItemLifeTimeMS = 1500;

	int monedasObtenidasHud = 0;


	bool isLowHealth = false;
	int blinkCounter = 0;
	int blinkSpeed = 20;

public:

	std::vector<SDL_Rect*> potionRects;
	std::map<InventityType, SDL_Rect*> potionRectMap;
	int selectedPotionIndex = 0;
	int selectedPotionItemIndex = 0;

	bool estatua = false;

	SDL_Texture* EstatuaTexture = nullptr;
};
#endif // __HUD_H__