
#include "EntityManager.h"
#include "BestiarioManager.h"
#include "NotesManager.h"
#include "Player.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene_testing.h"
#include "Bestiario.h"
#include "SwordInv.h"
#include "GarraInv.h"
#include "ArmaduraInv.h"
#include "DiamanteInv.h"
#include "ItemInv.h"
#include "OjoInv.h"
#include "Defs.h"
#include "Log.h"
#include "SString.h"

BestiarioManager::BestiarioManager(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("bestiariomanager");
}

// Destructor
BestiarioManager::~BestiarioManager()
{}

// Called before render is available
bool BestiarioManager::Awake(pugi::xml_node config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Bestiario*>* item;
	Bestiario* pEntity = NULL;

	for (item = bestiario.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	iconoBestiarioTexturePath = ((char*)config.child("bestiario").attribute("texturePath").as_string());
	listTexturePath = ((char*)config.child("bestiario").attribute("list_texture").as_string());
	sliderTexturePath = ((char*)config.child("bestiario").attribute("slider_texture").as_string());
	knobTexturePath = ((char*)config.child("bestiario").attribute("knob_texture").as_string());
	PointerPath = ((char*)config.child("bestiario").attribute("pointer").as_string());
	textoOsiris = ((char*)config.child("bestiario").attribute("text_osiris").as_string());
	textoMuur = ((char*)config.child("bestiario").attribute("text_muur").as_string());
	CloseUpPath = ((char*)config.child("bestiario").attribute("closeUp").as_string());

	
	return ret;

}

bool BestiarioManager::Start() {


	
	CloseUp = app->tex->Load(CloseUpPath);
	listTexture = app->tex->Load(listTexturePath);
	sliderTexture = app->tex->Load(sliderTexturePath);
	knobTexture = app->tex->Load(knobTexturePath);
	PointerItemText = app->tex->Load(PointerPath);
	PointerId = -2;
	PointerItemText = nullptr;

	bool ret = true;

	//Iterates over the entities and calls Start
	ListItem<Bestiario*>* item;
	Bestiario* pEntity = NULL;

	for (item = bestiario.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	scrollY = 0;

	return ret;
}

// Called before quitting
bool BestiarioManager::CleanUp()
{
	bool ret = true;
	ListItem<Bestiario*>* item;
	item = bestiario.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	bestiario.Clear();
	app->tex->UnLoad(PointerItemText);
	app->tex->UnLoad(SelectedItemText);
	app->tex->UnLoad(EquipedItemText);
	app->tex->UnLoad(listTexture);
	return ret;
}
int highestttId = -1;
Bestiario* BestiarioManager::CreateItem(char* name)
{
	Bestiario* entity = nullptr;


	highestttId = (bestiario.Count() > 0) ? bestiario.end->data->id : -1;
	entity = new Bestiario();
	entity->id = highestttId + 1;
	entity->closeUpBestiarios = CloseUp;
	if (strcmp(name, "osiris") == 0)
	{
		entity->name = name;
		entity->desc = textoOsiris;
	}
	else if (strcmp(name, "muur") == 0)
	{
		entity->name = name;
		entity->desc = textoMuur;
	}
	else
	{
		// Default properties if name doesn't match
	}
	
	/*entity->closeUpBestiario = app->tex->Load("Assets/Textures/Entidades/Items/textura_BestiarioCloseUp.png"); */
	entity->icon = app->tex->Load(iconoBestiarioTexturePath);
		
	entity->type = BestiarioType::BEST;
	
	


	AddBestiario(entity);

	return entity;
}

bool BestiarioManager::IsFull()
{

	// Encontrar el ID más alto actualmente asignado


	// Verificar si el siguiente ID disponible es 9
	if (bestiario.Count() == 8) {
		return true;
	}
	else {
		return false;
	}

}

bool BestiarioManager::LoadState(pugi::xml_node node)
{
	bool ret = true;

	ListItem<Bestiario*>* item;
	item = bestiario.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	bestiario.Clear();

	pugi::xml_document configFile;
	pugi::xml_node entitiesDataNode;
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	entitiesDataNode = configFile.child("config").child("entities_data");

	//Bestiario* BestiarioItem = app->BestiarioManager->CreateItem(, 0, 0, 0, 0, 0, 0);
	for (pugi::xml_node itemNode = node.child("Bestiario").child("Bestiario"); itemNode; itemNode = itemNode.next_sibling("Bestiario"))
	{

		Bestiario* itemLoaded = nullptr;
		pugi::xml_node itemConfigurationNode;
		switch ((BestiarioType)itemNode.attribute("type").as_int())
		{
		case BestiarioType::BEST:
			/*itemConfigurationNode = entitiesDataNode.child("item_garra");
			itemLoaded = app->bestiarioManager->CreateItem(EntityType::ITEM_NOTA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string());*/
			break;

		case BestiarioType::UNKNOWN:
			break;
		default:
			break;
		}
		

	}




	return ret;
}

bool BestiarioManager::SaveState(pugi::xml_node node)
{
	bool ret = true;
	pugi::xml_node BestiarioNode = node.append_child("Bestiario");

	for (int i = 0; i < bestiario.Count(); i++) {

		Bestiario* BestiarioItem = bestiario.At(i)->data;

		pugi::xml_node BestiarioItemNode = BestiarioNode.append_child("Bestiario");
		BestiarioItemNode.append_attribute("type").set_value((int)BestiarioItem->type);
		BestiarioItemNode.append_attribute("quantity").set_value(BestiarioItem->quantity);
	}

	


	return ret;
}


void BestiarioManager::UseBestiarioSelected(int id)
{

	ListItem<Bestiario*>* item;

	/*for (item = bestiario.start; item != NULL; item = item->next)
	{
		item->data->active = false;
	}*/

	for (item = bestiario.start; item != NULL; item = item->next)
	{

		if (item->data->id == id) // Comprueba si el ID coincide
		{

			item->data->active = true;
			switch (item->data->type)
			{
				
			case BestiarioType::BEST:
			{
				item->data->zoom = true;
				zoomIn = true;

				break;
			}
			}


		}


	}
	
}

void BestiarioManager::OnMovePointer()
{
	bool pointer = false;
	if (PointerId != -2)
	{
		if (pointer == false)
		{
			PointerItemText = app->tex->Load(PointerPath);
			pointer = true;
		}
		
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN) {
			if (horitzontalPointerId + 1 > 3)
			{
				horitzontalPointerId = 0;
				PointerPosition.x = 660;
				PointerId -= 3;
			}
			else
			{
				if (PointerId != -1)
				{
					horitzontalPointerId += 1;
					PointerPosition.x += 83;
				}
				PointerId += 1;
			}
			
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN) {
			if (horitzontalPointerId - 1 < 0)
			{
				if (app->notesManager->vacio == false)
				{
					PointerId = -2;
					PointerItemText = nullptr;
					app->notesManager->PointerId = 0;
				}
			
			}
			else
			{
				horitzontalPointerId -= 1;
				PointerPosition.x -= 83;
				PointerId -= 1;
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
			if (verticalPointerId < numberRows)
			{
				PointerId += 4;
				verticalPointerId += 1;
				PointerPosition.y += 83;
			}
			else
			{
				verticalPointerId = 0;
				PointerId -= numberRows * 4;
				PointerPosition.y = 230;
			}



		}
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
			if (verticalPointerId + 1> numberRows)
			{
				PointerPosition.y -= 83;
				PointerId -= 4;
				verticalPointerId -= 1;
			}
			else
			{
				verticalPointerId = numberRows;
				PointerId += numberRows * 4;
				PointerPosition.y = 230 + 83 * (numberRows);
			}


		}
	}
	else
	{
		pointer = false;
	}
}

void BestiarioManager::AddBestiario(Bestiario* entity)
{

	if (entity != nullptr) {

		bestiario.Add(entity);

	}
	
}

bool BestiarioManager::Update(float dt)
{

	bool ret = true;

	

	if (mostrar == true)
	{
		OnMovePointer();

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			/*options = true;
			selected = { PointerPosition.x, PointerPosition.y };
			selectedId = PointerId;*/
			UseBestiarioSelected(PointerId);

		}

		if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && zoomIn == true) {
			ListItem<Bestiario*>* item;
			for (item = bestiario.start; item != NULL; item = item->next)
			{
				item->data->zoom = false;

			}
			zoomIn = false;

		}


			/*if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				equiped = { PointerPosition.x, PointerPosition.y };
				equipedId = PointerId;
				UseBestiarioSelected(equipedId);
				options = false;
				selected = { -1000, -1000 };

			}*/
		/*}*/

		int num;

			num = numberRows + 1;
		

		if (bestiario.Count() > 3 * num)
		{
			numberRows += 1;
		}

	}






	return ret;
}

bool BestiarioManager::PostUpdate()
{
	bool ret = true;

	uint tradeSpacing = 83;

	SDL_Rect viewport = { 620, 200, 400, 350 };
	

	if (mostrar == true)
	{
		/*app->render->DrawRectangle(viewport, 0, 0, 0, 200, true, false);*/
		ListItem<Bestiario*>* item;
		Bestiario* pEntity = NULL;

		

		
		

		for (item = bestiario.start; item != nullptr; item = item->next)
		{
			

			
			
			
			pEntity = item->data;
			int rowIndex = item->data->id / maxItemsPerRow; // Calcula el índice de la fila
			int columnIndex = item->data->id % maxItemsPerRow; // Calcula el índice de la columna
			int horizontalPosition = 660 + columnIndex * 83; // Calcula la posición horizontal
			int verticalPosition = 230 + rowIndex * 83; // Calcula la posición vertical

			int y2 = PointerPosition.y - scrollY;

			int y = verticalPosition - scrollY;
			
			if (y2 >= viewport.y && y2 <= viewport.y + viewport.h)
			{
				if (y >= viewport.y && y <= viewport.y + viewport.h) {
					if (zoomIn == false && app->notesManager->zoomIn == false)
					{
						app->render->DrawTexture(listTexture, horizontalPosition, verticalPosition, SDL_FLIP_NONE, 0, 0);

						app->render->DrawTexture(pEntity->icon, horizontalPosition, verticalPosition, 0.8, SDL_FLIP_NONE, 0, 0);
					}
					
				}
			}
			
			else
			{
				int targetY = PointerId/4 * 83;
				
				
				scrollY = targetY;
				
				scrollY = std::max(0, scrollY);
			}
			app->render->DrawTexture(PointerItemText, PointerPosition.x, PointerPosition.y, SDL_FLIP_NONE, 0, 0);
		}


	}
	ret = true;
	int knobY = 200;
	if (numberRows == 0)
	{
		knobY = 200;
	}
	else
	{
		knobY= 200 + (341 / numberRows ) * verticalPointerId;
	}


	if (mostrar == true)
	{
		app->render->DrawTexture(sliderTexture, 960, 200, SDL_FLIP_NONE, 0, 0);
		app->render->DrawTexture(knobTexture, 960, knobY, SDL_FLIP_NONE, 0, 0);

		ListItem<Bestiario*>* itum;
		for (itum = bestiario.start; itum != nullptr; itum = itum->next)
		{
			if (itum->data->zoom)
			{
				
					
				
				if (PointerId == itum->data->id)
				{
					app->render->DrawTexture(itum->data->closeUpBestiarios, 400, 100, SDL_FLIP_NONE, 0, 0);
					app->render->DrawText(itum->data->desc.c_str(), 450, 200, 270, 400, 0, 0, 0, 0, false);
				}
			}
		}

		
	}
	
	return ret;
}






