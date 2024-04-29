
#include "EntityManager.h"
#include "NotesManager.h"
#include "Player.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene_testing.h"
#include "Note.h"
#include "SwordInv.h"
#include "GarraInv.h"
#include "ArmaduraInv.h"
#include "DiamanteInv.h"
#include "ItemInv.h"
#include "OjoInv.h"
#include "Defs.h"
#include "Log.h"
#include "SString.h"

NotesManager::NotesManager(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("notesmanager");
}

// Destructor
NotesManager::~NotesManager()
{}

// Called before render is available
bool NotesManager::Awake(pugi::xml_node config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Note*>* item;
	Note* pEntity = NULL;

	for (item = notes.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool NotesManager::Start() {

	PointerItemText = app->tex->Load("Assets/Textures/Interfaz/noteselect.png");


	bool ret = true;

	//Iterates over the entities and calls Start
	ListItem<Note*>* item;
	Note* pEntity = NULL;

	for (item = notes.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool NotesManager::CleanUp()
{
	bool ret = true;
	ListItem<Note*>* item;
	item = notes.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	notes.Clear();
	app->tex->UnLoad(PointerItemText);
	app->tex->UnLoad(SelectedItemText);
	app->tex->UnLoad(EquipedItemText);
	return ret;
}
int highesttId = -1;
Note* NotesManager::CreateItem(EntityType type, SDL_Texture* CloseUp)
{
	Note* entity = nullptr;


	highesttId = (notes.Count() > 0) ? notes.end->data->id : -1;
	entity = new Note();
	entity->id = highesttId + 1;
	entity->closeUpNotes = CloseUp;
	/*entity->closeUpNotes = app->tex->Load("Assets/Textures/Entidades/Items/textura_NoteCloseUp.png"); */
	switch (type)
	{
	
	case EntityType::ITEM_NOTA:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_nota.png");
		entity->type = NoteType::NOTE;
		break;
	default:
		break;
	}


	AddNote(entity);

	return entity;
}

bool NotesManager::IsFull()
{

	// Encontrar el ID más alto actualmente asignado


	// Verificar si el siguiente ID disponible es 9
	if (notes.Count() == 8) {
		return true;
	}
	else {
		return false;
	}

}

bool NotesManager::LoadState(pugi::xml_node node)
{
	bool ret = true;

	ListItem<Note*>* item;
	item = notes.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	notes.Clear();

	pugi::xml_document configFile;
	pugi::xml_node entitiesDataNode;
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	entitiesDataNode = configFile.child("config").child("entities_data");

	//Note* NoteItem = app->NotesManager->CreateItem(, 0, 0, 0, 0, 0, 0);
	for (pugi::xml_node itemNode = node.child("Note").child("Note"); itemNode; itemNode = itemNode.next_sibling("Note"))
	{

		Note* itemLoaded = nullptr;
		pugi::xml_node itemConfigurationNode;
		switch ((NoteType)itemNode.attribute("type").as_int())
		{
		case NoteType::NOTE:
			/*itemConfigurationNode = entitiesDataNode.child("item_garra");
			itemLoaded = app->notesManager->CreateItem(EntityType::ITEM_NOTA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string());*/
			break;

		case NoteType::UNKNOWN:
			break;
		default:
			break;
		}
		

	}




	return ret;
}

bool NotesManager::SaveState(pugi::xml_node node)
{
	bool ret = true;
	pugi::xml_node NoteNode = node.append_child("Note");

	for (int i = 0; i < notes.Count(); i++) {

		Note* NoteItem = notes.At(i)->data;

		pugi::xml_node NoteItemNode = NoteNode.append_child("Note");
		NoteItemNode.append_attribute("type").set_value((int)NoteItem->type);
		NoteItemNode.append_attribute("quantity").set_value(NoteItem->quantity);
	}

	


	return ret;
}


void NotesManager::UseNoteSelected(int id)
{

	ListItem<Note*>* item;

	/*for (item = notes.start; item != NULL; item = item->next)
	{
		item->data->active = false;
	}*/

	for (item = notes.start; item != NULL; item = item->next)
	{

		if (item->data->id == id) // Comprueba si el ID coincide
		{

			item->data->active = true;
			switch (item->data->type)
			{
				
			case NoteType::NOTE:
			{
				item->data->zoom = true;
				zoomIn = true;

				break;
			}
			}


		}


	}
	
}

void NotesManager::OnMovePointer()
{

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN && PointerPosition.x < 300) {
		PointerPosition.x += 492;
		PointerId += 1;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN && PointerPosition.x > 176) {
		PointerPosition.x -= 492;
		PointerId -= 1;
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && PointerPosition.y < 100) {
		PointerPosition.y += 83;
		PointerId += 2;
	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && PointerPosition.y > -60) {
		PointerPosition.y -= 83;
		PointerId -= 2;
	}
}

void NotesManager::AddNote(Note* entity)
{

	if (entity != nullptr) {

		notes.Add(entity);

	}
	
}

bool NotesManager::Update(float dt)
{

	bool ret = true;



	if (mostrar == true)
	{
		OnMovePointer();

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			/*options = true;
			selected = { PointerPosition.x, PointerPosition.y };
			selectedId = PointerId;*/
			UseNoteSelected(PointerId);

		}

		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && zoomIn == true) {
			ListItem<Note*>* item;
			for (item = notes.start; item != NULL; item = item->next)
			{
				item->data->zoom = false;

			}
			zoomIn = false;

		}


			/*if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				equiped = { PointerPosition.x, PointerPosition.y };
				equipedId = PointerId;
				UseNoteSelected(equipedId);
				options = false;
				selected = { -1000, -1000 };

			}*/
		/*}*/



	}






	return ret;
}

bool NotesManager::PostUpdate()
{
	bool ret = true;

	if (mostrar == true)
	{
		ListItem<Note*>* item;
		Note* pEntity = NULL;

		

		app->render->DrawTexture(PointerItemText, PointerPosition.x, PointerPosition.y, SDL_FLIP_NONE, 0, 0);
		

		for (item = notes.start; item != nullptr; item = item->next)
		{
			pEntity = item->data;
			int rowIndex = item->data->id / maxItemsPerRow; // Calcula el índice de la fila
			int columnIndex = item->data->id % maxItemsPerRow; // Calcula el índice de la columna
			int horizontalPosition = 320 + columnIndex * 492; // Calcula la posición horizontal
			int verticalPosition = 230 + rowIndex * 83; // Calcula la posición vertical

			if(zoomIn == false)
			app->render->DrawTexture(pEntity->icon, horizontalPosition, verticalPosition, 0.8, SDL_FLIP_NONE, 0, 0);

			if (item->data->zoom)
			{
				app->render->DrawTexture(pEntity->closeUpNotes, 400, 100, SDL_FLIP_NONE, 0, 0);
			}

		}


	}
	
	return ret;
}






