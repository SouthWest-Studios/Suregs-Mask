#ifndef __INVENTORYMANAGER_H__
#define __INVENTORYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "inventity.h"
#include "List.h"
#include "Textures.h"

class InventoryManager : public Module
{
public:

	InventoryManager(App* app, bool start_enabled = true);

	// Destructor
	virtual ~InventoryManager();

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
	Inventity* CreateItem(EntityType type, std::string descripcioon, std::string tipo);

	void AddItem(Inventity* Inventity);

	void DestroyItem(Inventity* Inventity);

	void DestroyItemById(int entityId);

	void UseItemSelected(int id);

	void OnMovePointer();

	bool IsFull();

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

	

public:



	List<Inventity*> inventities;
	iPoint PointerPosition = { 176,-60 };
	int PointerId = 0;
	iPoint selected = { -1000,-1000 };
	iPoint equiped = { -1000,-1000 };
	int selectedId = -1;
	int equipedId = -1;
	iPoint InventorySize;
	SDL_Texture* InventoryBackground;
	SDL_Texture* PointerItemText;
	SDL_Texture* SelectedItemText;
	SDL_Texture* EquipedItemText;

	uint texW, texH;
	bool mostrar = false;
	bool options = false;

	int highestIde = -1;

private:
	int maxItemsPerRow = 3; // Define el número máximo de elementos por fila
};

#endif // __INVENTORYMANAGER_H__
