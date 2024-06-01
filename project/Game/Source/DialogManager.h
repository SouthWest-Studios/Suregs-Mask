#ifndef __DIALOGMANAGER_H__
#define __DIALOGMANAGER_H__

#include "Module.h"
#include "Dialog.h"
#include "Timer.h"
#include "List.h"
#include <vector>
#include "SDL_ttf/include/SDL_ttf.h"

class DialogManager : public Module
{
public:

	DialogManager(App* app, bool start_enabled = true);

	// Destructor
	virtual ~DialogManager();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//Funcion para crear dialogos
	Dialog* CreateDialog(pugi::xml_node itemNode, std::string name = "", const char* faceTexturePath = "", const char* font = "primary");
	
	//Funcion para añadir a la lista de dialogos
	bool AddDialog(Dialog* dialog);
	

	//Fishing
	void CreateDialogSinEntity(std::string Texto, std::string nombre, const char* texture);
	void AutoNextDiagolo(int autoNextTime);
private:
	//Funcion que se encarga de mostrar los dialogos
	bool ShowDialog(Dialog* dialog);

	SDL_Texture* CreateTextTexture(TTF_Font* font, const char* text, SDL_Color color, int textBoundWidth);

	TTF_Font* FontSelector(const char* font);


public:

	List<Dialog*> dialogues;

	bool isPlaying;

	uint32 autoNextTime_show;
	Timer autoNextTime_TimerDown;


private:

	//Colores
	SDL_Color textColor = { 52,25,0,255 };
	SDL_Color optionSelectedColor = { 255, 255, 255, 255 };
	SDL_Color optionColor = { 52, 25, 0, 255 };
	

	//Posiciones
	std::vector<int> dialogMargin = { 120, 200, 0, 30 };
	iPoint dialogPosition = { 59 , 318 }; //{ 0 , 375 }
	iPoint namePosition = { 70 , -60 };
	iPoint optionsPosition = { 0 , 300 };
	int optionsDistanceBetween = 60;

	Timer dialogueAnimation;

	int actualDialogYPosition = 0;

	//Dimensiones
	int textBoundWidth= 500;
	int textNameBoundWidth = 200;
	int optionsBoundWidth = 350;
	iPoint faceTextureSize = { 256, 210 };
	

	//Config
	int charTimeMS = 10;


	//Para el funcionamiento

	SDL_Texture* background_tex;
	std::string background_tex_path;
	int indexText;
	Timer charTimer;
	int optionSelected = 0;

	int voice;
	int dialog_fx;
	int dialogAlt_fx;
	int dialogAlt2_fx;
};

#endif // __DIALOGMANAGER_H__
