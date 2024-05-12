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

	void OnMovePointer();

	bool IsFull();

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

	

public:
	int mask = 1;
	List<Tree*> arboles;
	
	iPoint PointerPosition = { 350, 130 };
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
	int maxItemsPerRow = 5; // Define el número máximo de elementos por fila
	int numberRows = 0;

	char* iconoTreeTexturePath;
	char* listTexturePath;
	char* listTexturePathNot;
	char* sliderTexturePath;
	char* knobTexturePath;
	char* BackGroundPath;
	SDL_Texture* BackGroundTexture;
	SDL_Texture* iconoTreeTexture;
	SDL_Texture* listTexture;
	SDL_Texture* listTextureNot;


	int scrollY;

	char* CloseUpPath;
	SDL_Texture* CloseUp;

	char* name = nullptr;
	char* description;

};

#endif // __INVENTORYMANAGER_H__
