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
	Note* CreateItem(EntityType type, SDL_Texture* CloseUp, std::string texto, std::string titulo, int id2);

	void SortNotesById2();

	void AddNote(Note* Inventity);

	void NotesManager::BubbleSortByID();

	/*void DestroyItem(Note* Inventity);*/

	/*void DestroyItemById(int entityId);*/

	void UseNoteSelected(int id);

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

public:

	List<Note*> notes;
	
	iPoint PointerPosition = { 250, 200 };
	int PointerId = 0;
	iPoint selected = { -1000,-1000 };
	iPoint equiped = { -1000,-1000 };
	int selectedId = -1;
	int equipedId = -1;
	char* PointerPath;
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

	bool vacio = true;

	bool acabar = true;

private:
	pugi::xml_document configFile;
	pugi::xml_node configNode;
	int maxItemsPerRow = 1; // Define el número máximo de elementos por fila

	char* iconoNotaTexturePath;
	char* listTexturePath;
	char* listTexturePath2;
	char* listTexturePath3;
	char* listTexturePath4;
	char* sliderTexturePath;
	char* knobTexturePath;
	SDL_Texture* listTexture;
	SDL_Texture* listTexture2;
	SDL_Texture* listTexture3;
	SDL_Texture* listTexture4;
	SDL_Texture* sliderTexture;
	SDL_Texture* knobTexture;

	int scrollY;
};

#endif // __INVENTORYMANAGER_H__
