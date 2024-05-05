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

	

public:

	List<Bestiario*> bestiario;
	
	iPoint PointerPosition = { 660, 230 };
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
	SDL_Texture* sliderTexture;
	SDL_Texture* knobTexture;

	int scrollY;

	char* CloseUpPath;
	SDL_Texture* CloseUp;
	char* textoOsiris;
	char* textoMuur;
};

#endif // __INVENTORYMANAGER_H__
