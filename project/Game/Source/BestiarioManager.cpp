
#include "EntityManager.h"
#include "BestiarioManager.h"
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

	iconoBestiarioTexturePath = ((char*)config.child("nota").attribute("texturePath").as_string());
	listTexturePath = ((char*)config.child("nota").attribute("list_texture").as_string());
	sliderTexturePath = ((char*)config.child("nota").attribute("slider_texture").as_string());
	knobTexturePath = ((char*)config.child("nota").attribute("knob_texture").as_string());
	return ret;

}

bool BestiarioManager::Start() {

	PointerItemText = app->tex->Load("Assets/Textures/Interfaz/Diario/bestiarioselect.png");
	listTexture = app->tex->Load(listTexturePath);
	sliderTexture = app->tex->Load(sliderTexturePath);
	knobTexture = app->tex->Load(knobTexturePath);

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
Bestiario* BestiarioManager::CreateItem(BestiarioType type)
{
	Bestiario* entity = nullptr;


	highestttId = (bestiario.Count() > 0) ? bestiario.end->data->id : -1;
	entity = new Bestiario();
	entity->id = highestttId + 1;
	entity->closeUpBestiarios = CloseUp;
	entity->desc = texto;
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

	/*if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN && PointerPosition.x < 300) {
		PointerPosition.x += 492;
		PointerId += 1;
	}*/
	/*if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN && PointerPosition.x > 176) {
		PointerPosition.x -= 492;
		PointerId -= 1;
	}*/

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
		if (PointerPosition.y + 83 < 500)
		{
			PointerPosition.y += 83;
			int a = 0;
		}
		if (PointerId + 1 != bestiario.Count())
		{
			PointerId += 1;
		}
		else
		{
			PointerId = 0;
			PointerPosition.y = 230;
		}

		
		
	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && PointerPosition.y > -60) {
		if (PointerPosition.y - 83 > 200)
		{
			PointerPosition.y -= 83;
		}
		if (PointerId - 1 != -1)
		{
			PointerId -= 1;
		}
		else
		{
			PointerId = bestiario.Count() - 1;
			PointerPosition.y = 479;
		}
		
		
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
		app->render->DrawRectangle(viewport, 0, 0, 0, 200, true, false);
		ListItem<Bestiario*>* item;
		Bestiario* pEntity = NULL;

		

		
		

		for (item = bestiario.start; item != nullptr; item = item->next)
		{
			

			
			
			
			pEntity = item->data;
			int rowIndex = item->data->id / maxItemsPerRow; // Calcula el índice de la fila
			int columnIndex = item->data->id % maxItemsPerRow; // Calcula el índice de la columna
			int horizontalPosition = 720 + columnIndex * 492; // Calcula la posición horizontal
			int verticalPosition = 210 + rowIndex * 83; // Calcula la posición vertical

			int y2 = PointerPosition.y - scrollY;

			int y = verticalPosition - scrollY;
			
			if (y2 >= viewport.y && y2 <= viewport.y + viewport.h)
			{
				if (y >= viewport.y && y <= viewport.y + viewport.h) {

					app->render->DrawTexture(listTexture, 650, verticalPosition, SDL_FLIP_NONE, 0, 0);

					if (zoomIn == false)
						app->render->DrawTexture(pEntity->icon, horizontalPosition, verticalPosition, 0.8, SDL_FLIP_NONE, 0, 0);

					
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
	int knobY;
	if (bestiario.Count() == 0)
	{
		knobY = 1;
	}
	else
	{
		knobY= 200 + (341 / (bestiario.Count() - 1)) * PointerId;
	}

	if (mostrar == true)
	{
		app->render->DrawTexture(sliderTexture, 930, 200, SDL_FLIP_NONE, 0, 0);
		app->render->DrawTexture(knobTexture, 930, knobY, SDL_FLIP_NONE, 0, 0);

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






