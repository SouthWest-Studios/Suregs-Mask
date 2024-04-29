#ifndef __NOTESMANAGER_H__
#define __NOTESMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "Note.h"
#include "List.h"
#include "Textures.h"

class NotesManager : public Module
{
public:

	NotesManager(App* app, bool start_enabled = true);

	// Destructor
	virtual ~NotesManager();

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
	Note* CreateItem(EntityType type, SDL_Texture* CloseUp);

	void AddNote(Note* Inventity);

	/*void DestroyItem(Note* Inventity);*/

	/*void DestroyItemById(int entityId);*/

	void UseNoteSelected(int id);

	void OnMovePointer();

	bool IsFull();

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

	

public:

	List<Note*> notes;
	
	iPoint PointerPosition = { +165, -127 };
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

	bool zoomIn = false;

private:
	pugi::xml_document configFile;
	pugi::xml_node configNode;
	int maxItemsPerRow = 2; // Define el número máximo de elementos por fila

	char* iconoNotaTexture;
};

#endif // __INVENTORYMANAGER_H__
