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
	Inventity* CreateItem(EntityType type, std::string descripcioon, std::string tipo, std::string nombre, bool addInventory = true);

	Inventity* CreateItem(InventityType type, bool addInventory = true); //Necesario para las tiendas

	void AddItem(Inventity* Inventity);

	void AddQuantity(Entity* entity = nullptr);

	void DestroyItem(InventityType type, int cantidad);

	void DestroyItemById(int entityId, bool useStack = true);

	void UseItemSelected(int id);

	void UsePotionSelected(ListItem<Inventity*>* it);

	void OnMovePointer();

	bool IsFull();

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

	int GetInventityQuantity(InventityType type);

	

public:

	uint monedasObtenidas = 0;

	List<Inventity*> inventities;
	iPoint PointerPosition = { 258, 130 };
	int PointerId = 0;
	iPoint selected = { -1000,-1000 };
	bool selectedd = false;
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
	bool inList = false;
	int highestIde = -1;
	int idSaved;

	int swordLevel = 0;
	int armorLevel = 0;

	int storyLevel = 1;
	bool dungeon0Entered = false;

	int currentHealthInventoryManager = NULL;
	int primaryMaskInventoryManager = -1;
	int secondaryMaskInventoryManager = -1;

	int numMasks = 0;
	bool mask = false;
private:
	pugi::xml_document configFile;
	pugi::xml_node configNode;
	int maxItemsPerRow = 3; // Define el n�mero m�ximo de elementos por fila


	SDL_Rect* pointerSpritePosition;
	Animation SPosition;

	Animation* currentPointerAnimation = nullptr;
	Animation pointerAnim;

};

#endif // __INVENTORYMANAGER_H__
