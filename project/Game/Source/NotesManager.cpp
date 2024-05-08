
#include "EntityManager.h"
#include "NotesManager.h"
#include "BestiarioManager.h"
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
	name = ("notesmanager");
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

	iconoNotaTexturePath = ((char*)config.child("nota").attribute("texturePath").as_string());
	listTexturePath = ((char*)config.child("nota").attribute("list_texture").as_string());
	sliderTexturePath = ((char*)config.child("nota").attribute("slider_texture").as_string());
	knobTexturePath = ((char*)config.child("nota").attribute("knob_texture").as_string());
	PointerPath = ((char*)config.child("nota").attribute("pointer").as_string());
	return ret;

}

bool NotesManager::Start() {

	PointerItemText = app->tex->Load(PointerPath);
	listTexture = app->tex->Load(listTexturePath);
	sliderTexture = app->tex->Load(sliderTexturePath);
	knobTexture = app->tex->Load(knobTexturePath);

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

	scrollY = 0;

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
	app->tex->UnLoad(listTexture);
	return ret;
}
int highesttId = -1;
Note* NotesManager::CreateItem(EntityType type, SDL_Texture* CloseUp, std::string texto, std::string titulo)
{
	Note* entity = nullptr;


	highesttId = (notes.Count() > 0) ? notes.end->data->id : -1;
	entity = new Note();
	entity->id = highesttId + 1;
	entity->closeUpNotes = CloseUp;
	entity->desc = texto;
	entity->title = titulo;
	/*entity->closeUpNotes = app->tex->Load("Assets/Textures/Entidades/Items/textura_NoteCloseUp.png"); */
	switch (type)
	{
		
	case EntityType::ITEM_NOTA:
		entity->icon = app->tex->Load(iconoNotaTexturePath);
		
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
	bool pointer = false;
	if (zoomIn == false)
	{
		if (PointerId != -2)
		{
			if (pointer == false)
			{
				PointerItemText = app->tex->Load(PointerPath);
				pointer = true;
			}
			if (app->input->GetButton(RIGHT) == KEY_DOWN && PointerPosition.x < 300 || vacio) {
				if (PointerId == -1)
				{
					PointerId += 1;
				}
				else
				{
					PointerId = -2;
					PointerItemText = nullptr;
					if (vacio)
						app->bestiarioManager->PointerId = 0;
					else
						app->bestiarioManager->PointerId = -1;
				}


			}

			if (app->input->GetButton(DOWN) == KEY_DOWN) {
				if (PointerId + 1 < notes.Count())
				{
					PointerPosition.y += 83;
					PointerId += 1;

				}
				else
				{
					PointerId = 0;
					PointerPosition.y = 230;
				}



			}
			if (app->input->GetButton(UP) == KEY_DOWN) {
				if (PointerId > 0)
				{
					PointerPosition.y -= 83;
					PointerId -= 1;

				}
				else
				{
					PointerId = notes.Count() - 1;
					PointerPosition.y = 230 + 83 * (notes.Count() - 1);
				}


			}
		}
		else
		{
			pointer = false;
		}
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
	int a = notes.Count();
	int c;
	if (notes.Count() > 0)
	{
		vacio = false;
	}
	else
	{
		vacio = true;
	}
		
	if (mostrar == true)
	{
		OnMovePointer();

		if (app->input->GetButton(SELECT) == KEY_DOWN) {
			/*options = true;
			selected = { PointerPosition.x, PointerPosition.y };
			selectedId = PointerId;*/
			UseNoteSelected(PointerId);

		}

		if (app->input->GetButton(APP_EXIT) == KEY_DOWN && zoomIn == true) {
			ListItem<Note*>* item;
			for (item = notes.start; item != NULL; item = item->next)
			{
				item->data->zoom = false;

			}
			zoomIn = false;

		}




	}






	return ret;
}

bool NotesManager::PostUpdate()
{
	bool ret = true;

	uint tradeSpacing = 83;

	SDL_Rect viewport = { 220, 200, 400, 350 };
	/*app->render->DrawRectangle(viewport, 0, 0, 0, 200, true, false);*/

	if (mostrar == true)
	{
		
		ListItem<Note*>* item;
		Note* pEntity = NULL;

		

		
		

		for (item = notes.start; item != nullptr; item = item->next)
		{
			

			
			
			
			pEntity = item->data;
			int rowIndex = item->data->id / maxItemsPerRow; // Calcula el índice de la fila
			int columnIndex = item->data->id % maxItemsPerRow; // Calcula el índice de la columna
			int horizontalPosition = 320 + columnIndex * 492; // Calcula la posición horizontal
			int verticalPosition = 230 + rowIndex * 83; // Calcula la posición vertical

			int y2 = PointerPosition.y - scrollY;

			int y = verticalPosition - scrollY;
			
			if (y2 >= viewport.y && y2 <= viewport.y + viewport.h)
			{
				if (y >= viewport.y && y <= viewport.y + viewport.h) {

					app->render->DrawTexture(listTexture, 250, verticalPosition - scrollY, SDL_FLIP_NONE, 0, 0);

					if (zoomIn == false)
					{
						app->render->DrawTexture(pEntity->icon, horizontalPosition, verticalPosition - scrollY, 0.8, SDL_FLIP_NONE, 0, 0);
						/*app->render->DrawText(pEntity->title.c_str(), horizontalPosition + 60, verticalPosition - scrollY, 100, 100, 0, 0, 0, 0, false);*/
						app->render->DrawTextBound(pEntity->title.c_str(), horizontalPosition + 60, verticalPosition - scrollY, 100, { 0,0,0 });
					}
					
				}
			}
			
			else
			{
				int targetY = PointerId * 83;
				
				
				scrollY = targetY;
				
				scrollY = std::max(0, scrollY);
			}
			app->render->DrawTexture(PointerItemText, PointerPosition.x, PointerPosition.y - scrollY, SDL_FLIP_NONE, 0, 0);
		}


	}
	ret = true;
	int knobY;
	if (notes.Count() == 0 || (notes.Count() - 1) == 0 || PointerId < 0)
	{
		
		knobY = 200;

	}
	else
	{
		knobY= 200 + (341 / (notes.Count() - 1)) * PointerId;
	}

	if (mostrar == true)
	{
		app->render->DrawTexture(sliderTexture, 530, 200, SDL_FLIP_NONE, 0, 0);
		app->render->DrawTexture(knobTexture, 530, knobY, SDL_FLIP_NONE, 0, 0);

		ListItem<Note*>* itum;
		for (itum = notes.start; itum != nullptr; itum = itum->next)
		{
			if (itum->data->zoom)
			{
				
					
				
				if (PointerId == itum->data->id)
				{
					app->render->DrawTexture(itum->data->closeUpNotes, 400, 100, SDL_FLIP_NONE, 0, 0);
					/*app->render->DrawText(itum->data->desc.c_str(), 450, 200, 270, 400, 0, 0, 0, 0, false);*/
					app->render->DrawTextBound(itum->data->desc.c_str(), 445, 200, 370, { 0,0,0 });
				}
			}
		}

		
	}
	
	return ret;
}






