#ifndef __SCENE_MENU_H__
#define __SCENE_MENU_H__

#include "Module.h"
#include "Player.h"
#include "GuiControl.h"
#include "GuiControlButton.h"
#include "GuiCheckBox.h"
#include "GuiControlSlider.h"
#include "Particle.h"
#include "ParticlePool.h"
#include "Emitter.h"

struct SDL_Texture;

class Scene_Menu : public Module
{
public:

	Scene_Menu(App* app, bool start_enabled = true);

	// Destructor
	virtual ~Scene_Menu();

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

	bool OnGuiMouseClickEvent(GuiControl* control);

	void SettingsInterface();
	void ShowCredits();
	void ShowSavedGames();
	void ShowNewGames();
	void ShowControls();
	void DestroySettingsInterface();

	void Fullscreen();

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

	pugi::xml_document saveOneFile;


	SDL_Texture* menuMain = nullptr;
	SDL_Texture* menuMain2 = nullptr;
	SDL_Texture* settings = nullptr;
	SDL_Texture* credits = nullptr;
	SDL_Texture* logo = nullptr;
	SDL_Texture* savedGames = nullptr;
	SDL_Texture* controls = nullptr;
	SDL_Texture* coin = nullptr;
	SDL_Texture* clock = nullptr;


	const char* menuMain_tp = nullptr;
	const char* menuMain2_tp = nullptr;
	const char* settings_tp = nullptr;
	const char* credits_tp = nullptr;
	const char* logo_tp = nullptr;
	const char* savedGames_tp = nullptr;
	const char* controls_tp = nullptr;
	const char* coin_tp = nullptr;
	const char* clock_tp = nullptr;
	
	int coinQuantityOne;
	int coinQuantityTwo;
	int coinQuantityThree;

	int questLineIdOne;
	int questLineIdTwo;
	int questLineIdThree;

	int actualQuestIndexOne;
	int actualQuestIndexTwo;
	int actualQuestIndexThree;

	std::string titleOne;
	std::string titleTwo;
	std::string titleThree;

	std::string timeOne;
	std::string timeTwo;
	std::string timeThree;

	List<GuiControl*> controlsScene;
	List<GuiControl*> controlsSettings;
	GuiControl* gcCloseCredits = nullptr;
	GuiControl* gcCloseSavedGames = nullptr;
	GuiControl* gcCloseNewGames = nullptr;
	GuiControl* gcCloseControls = nullptr;
	GuiCheckBox* fullscreen = nullptr;
	GuiCheckBox* vsync = nullptr;

	bool showCredits = false;
	bool _showCredits = false;

	Timer closeshowCredits;

private:

	Emitter* eMenu = nullptr;

	GuiControlButton* NuevaPartida = nullptr;
	GuiControlButton* Continuar = nullptr;

	GuiControlButton* gcButtom = nullptr;
	GuiControlButton* exit = nullptr;
	GuiControlButton* atras = nullptr;
	GuiControlSlider* music = nullptr;
	GuiControlSlider* sfx = nullptr;

	GuiControlButton* partida1 = nullptr;
	GuiControlButton* partida2 = nullptr;
	GuiControlButton* partida3 = nullptr;

	GuiControlButton* nuevaPartida1 = nullptr;
	GuiControlButton* nuevaPartida2 = nullptr;
	GuiControlButton* nuevaPartida3 = nullptr;

	

	bool vsyncActive = false;

	uint windowW, windowH;
	
	int menu_fx;
	
	bool fullScreenActive = false;

	bool showSettings = false;
	bool _showSettings = false;

	bool showSavedGames = false;
	bool _showSavedGames = false;

	bool showNewGames = false;
	bool _showNewGames = false;

	bool showControls = false;
	bool _showControls = false;

	bool ajustes = false;

	float animationTime = 0.0f; // Tiempo de animación para la entrada
	float exitAnimationTime = 0.0f; // Tiempo de animación para la salida
	bool animating = false;
	bool animatingExit = false;
	bool logoAnimating = false;
	bool showLogo = true;

	bool eMenuGenerated = false;
	

};

#endif // __SCENE_MENU_H__