#ifndef __TREEMANAGER_H__
#define __TREEMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "Tree.h"
#include "List.h"
#include "Textures.h"

class TreeManager : public Module
{
public:

	TreeManager(App* app, bool start_enabled = true);

	// Destructor
	virtual ~TreeManager();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Tree* CreateItem(TreeType type, int nivelArbol, int nivelMejora, bool usable);

	void AddTree(Tree* Inventity);

	/*void DestroyItem(Bestiario* Inventity);*/

	/*void DestroyItemById(int entityId);*/

	void UseTreeSelected(int id);

	void ReembolsarTreeSelected(int id);

	void OnMovePointer();

	bool IsFull();

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

	Tree* searchTreeByArbolMejora(int nivelArbol, int nivelMejora);

	

public:
	int mask = 0;
	List<Tree*> arboles; 
	unsigned int numMejoras0 = 0;
	unsigned int numMejoras1 = 0;
	unsigned int numMejoras2 = 0;
	unsigned int numMejoras3 = 0;
	unsigned int puntsRembolso = 0;

	iPoint PointerPosition = { 156, 148 };
	iPoint PointerPosition2 = { 174, 138 };
	int PointerId = 0;
	iPoint selected = { -1000,-1000 };
	iPoint equiped = { -1000,-1000 };
	int selectedId = -1;
	int equipedId = -1;
	SDL_Texture* TreeBackground;
	char* PointerPath;
	SDL_Texture* PointerItemText;
	SDL_Texture* SelectedItemText;
	SDL_Texture* EquipedItemText;

	char* PointerPath2;
	SDL_Texture* PointerItemText2;

	char* PointerPath3;
	SDL_Texture* PointerItemText3;

	uint texW, texH;
	bool mostrar = false;
	bool options = false;

	int highestIde = -1;

	bool zoomIn = false;

	int verticalPointerId = 0;
	int horitzontalPointerId = 0;

	bool selectPrimaryMask = false;
	bool selectSecondaryMask = false;

private:
	pugi::xml_document configFile;
	pugi::xml_node configNode;
	int maxItemsPerRow = 5; // Define el número máximo de elementos por fila
	int numberRows = 0;

	char* iconoTreeTexturePath;
	char* listTexturePath;
	char* listTexturePathNot;
	char* sliderTexturePath;
	char* knobTexturePath;
	char* BackGroundPath;

	char* alcancePath;
	char* aumentoPath;
	char* cooldownPath;
	char* danoPath;
	char* distanciaPath;
	char* durationPath;
	char* invisiblePath;
	char* poisonPath;
	char* rayoPath;

	char* alcanceLockedPath ;
	char* aumentoLockedPath ;
	char* cooldownLockedPath ;
	char* danoLockedPath ;
	char* distanciaLockedPath ;
	char* durationLockedPath ;
	char* invisibleLockedPath ;
	char* poisonLockedPath ;
	char* rayoLockedPath ;

	char* alcanceSelectedPath ;
	char* aumentoSelectedPath ;
	char* cooldownSelectedPath ;
	char* danoSelectedPath ;
	char* distanciaSelectedPath ;
	char* durationSelectedPath ;
	char* invisibleSelectedPath ;
	char* poisonSelectedPath ;
	char* rayoSelectedPath ;
	SDL_Texture* BackGroundTexture;
	SDL_Texture* iconoTreeTexture;
	SDL_Texture* listTexture;
	SDL_Texture* listTextureNot;


	int scrollY;

	char* CloseUpPath;
	SDL_Texture* CloseUp;

	char* mascara0Path = nullptr;
	char* mascara1Path = nullptr;
	char* mascara2Path = nullptr;
	char* mascara3Path = nullptr;

	SDL_Texture* mascara0Texture;
	SDL_Texture* mascara1Texture;
	SDL_Texture* mascara2Texture;
	SDL_Texture* mascara3Texture;

	char* mascara0SelectedPath = nullptr;
	char* mascara1SelectedPath = nullptr;
	char* mascara2SelectedPath = nullptr;
	char* mascara3SelectedPath = nullptr;

	SDL_Texture* mascara0SelectedTexture = nullptr;
	SDL_Texture* mascara1SelectedTexture = nullptr;
	SDL_Texture* mascara2SelectedTexture = nullptr;
	SDL_Texture* mascara3SelectedTexture = nullptr;

	char* description;

	bool setMask = false;

	int button_fx;
	int select_fx;
};

#endif 
