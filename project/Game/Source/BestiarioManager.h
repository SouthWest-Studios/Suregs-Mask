#ifndef __BESTIARIOMANAGER_H__
#define __BESTARIOMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "Bestiario.h"
#include "List.h"
#include "Textures.h"

class BestiarioManager : public Module
{
public:

	BestiarioManager(App* app, bool start_enabled = true);

	// Destructor
	virtual ~BestiarioManager();

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
	Bestiario* CreateItem(char* name);

	void AddBestiario(Bestiario* Inventity);

	/*void DestroyItem(Bestiario* Inventity);*/

	/*void DestroyItemById(int entityId);*/

	void UseBestiarioSelected(int id);

	void OnMovePointer();

	bool IsFull();

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

	void StartAnimation();

	void StartExitAnimation();

	inline float easeOutBounce(float t);

	inline float easeOutCubic(float t);

	// Declaraciones globales para la animación
	float animationTime = 0.0f; // Tiempo de animación
	bool animating = false; // Indicador de animación
	bool animatingExit = false; // Indicador de animación de salida
	bool animatingExit2 = false;
	bool acabar = true;

public:

	List<Bestiario*> bestiario;
	
	iPoint PointerPosition = { 630, 230 };
	int PointerId = 0;
	iPoint selected = { -1000,-1000 };
	iPoint equiped = { -1000,-1000 };
	int selectedId = -1;
	int equipedId = -1;
	iPoint InventorySize;
	SDL_Texture* InventoryBackground;
	char* PointerPath;
	SDL_Texture* PointerItemText;
	SDL_Texture* SelectedItemText;
	SDL_Texture* EquipedItemText;

	uint texW, texH;
	bool mostrar = false;
	bool options = false;

	int highestIde = -1;

	bool zoomIn = false;

	int verticalPointerId = 0;
	int horitzontalPointerId = 0;

private:
	pugi::xml_document configFile;
	pugi::xml_node configNode;
	int maxItemsPerRow = 4; // Define el número máximo de elementos por fila
	int numberRows = 0;

	char* iconoBestiarioTexturePath;
	char* listTexturePath;
	char* sliderTexturePath;
	char* knobTexturePath;
	SDL_Texture* iconoBestiarioTexture;
	SDL_Texture* listTexture;
	char* listTexturePath2;
	char* listTexturePath3;
	char* listTexturePath4;
	SDL_Texture* listTexture2;
	SDL_Texture* listTexture3;
	SDL_Texture* listTexture4;
	SDL_Texture* sliderTexture;
	SDL_Texture* knobTexture;

	int scrollY;

	char* CloseUpPath;
	SDL_Texture* CloseUp;
	char* textoOsiris;
	char* pathOsiris;
	SDL_Texture* textureOsiris;
	char* textoOsirisV;
	char* pathOsirisV;
	SDL_Texture* textureOsirisV;
	char* textoMuur;
	char* textoMuurV;
	char* pathMuur;
	SDL_Texture* textureMuur;
	char* textoKhurt;
	char* textoKhurtV;
	char* pathKhurt;
	SDL_Texture* textureKhurt;
	char* textoBoorok;
	char* textoBoorokV;
	char* pathBoorok;
	SDL_Texture* textureBoorok;
	char* textoOls;
	char* textoOlsV;
	char* pathOls;
	SDL_Texture* textureOls;
	char* textoInuit;
	char* textoInuitV;
	char* pathInuit;
	SDL_Texture* textureInuit;
	char* textoSurma;
	char* textoSurmaV;
	char* pathSurma;
	SDL_Texture* textureSurma;
	char* textoMusri;
	char* textoMusriV;
	char* pathMusri;
	SDL_Texture* textureMusri;
	char* iconPathOsiris;
	SDL_Texture* iconTextureOsiris;
	char* iconPathMuur;
	SDL_Texture* iconTextureMuur;
	char* iconPathKhurt;
	SDL_Texture* iconTextureKhurt;
	char* iconPathBoorok;
	SDL_Texture* iconTextureBoorok;
	char* iconPathOls;
	SDL_Texture* iconTextureOls;
	char* iconPathInuit;
	SDL_Texture* iconTextureInuit;
	char* iconPathMusri;
	SDL_Texture* iconTextureMusri;
	char* iconPathSurma;
	SDL_Texture* iconTextureSurma;

	int osiris = 0;
	int osirisV = 0;
	int muur = 0;
	int muurV = 0;
	int khurt = 0;
	int khurtV = 0;
	int boorok = 0;
	int boorokV = 0;
	int ols = 0;
	int olsV = 0;
	int inuit = 0;
	int inuitV = 0;
	int musri = 0;
	int musriV = 0;
	int surma = 0;
	int surmaV = 0;
};

#endif // __INVENTORYMANAGER_H__
