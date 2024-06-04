#ifndef __MENU_H__
#define __MENU_H__

#include "Module.h"
#include "Player.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "GuiCheckBox.h"
#include "GuiControlSlider.h"
#include "Render.h"
#include "Textures.h"

struct SDL_Texture;

class Menu : public Module
{
public:

	Menu(App* app, bool start_enabled = true);

	// Destructor
	virtual ~Menu();

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

	bool OnGuiMouseClickEvent(GuiControl* control);

	void SettingsInterface();
	void ShowSettingsInterface();
	void ShowCredits();
	void DestroySettingsInterface();

	void Fullscreen();

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

	SDL_Texture* CreateTransparentTexture(SDL_Renderer* renderer, int width, int height, Uint8 alpha);

	void StartAnimation();

	void StartExitAnimation();

	inline float easeOutBounce(float t);

	inline float easeOutCubic(float t);

	void Menu::ToggleMenu();

	bool vsyncActive = false;
	bool visible = true;
	bool menuu = false;
	bool quitar = false;

	GuiControlSlider* music = nullptr;
	GuiControlSlider* sfx = nullptr;
	int ventana = 1;

	pugi::xml_node config;

	// Declaraciones globales para la animación
	float animationTime = 0.0f; // Tiempo de animación
	bool animating = false; // Indicador de animación
	bool animatingExit = false; // Indicador de animación de salida
	bool animatingExit2 = false;

	bool audio = false;

	int musicNum;
	int sfxNum;

private:
	SDL_Texture* fondoInventario = nullptr;
	SDL_Texture* fondoEquipo = nullptr;
	SDL_Texture* fondoDiario = nullptr;
	SDL_Texture* fondoAjustes = nullptr;
	SDL_Texture* fondoAjustesMando = nullptr;
	
	int a = 0;
	
	int contadormenu = 0;
	bool menuusettings = false;
	bool fullScreenActive = false;
	


	GuiControlButton* gcButtom = nullptr;
	GuiControlButton* exit = nullptr;
	GuiCheckBox* fullScreen = nullptr;
	GuiCheckBox* vsync = nullptr;
	GuiControlButton* title = nullptr;
	GuiControlButton* cruz = nullptr;
	

	int change_inventory_fx;
	int inventory_fx;
};

#endif // __MENU_H__