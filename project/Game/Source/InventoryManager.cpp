
#include "EntityManager.h"
#include "InventoryManager.h"
#include "Player.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene_testing.h"
#include "inventity.h"
#include "SwordInv.h"
#include "GarraInv.h"
#include "ArmaduraInv.h"
#include "DiamanteInv.h"
#include "ItemInv.h"
#include "OjoInv.h"
#include "Defs.h"
#include "Log.h"
#include "SString.h"

InventoryManager::InventoryManager(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("iventorymanager");
}

// Destructor
InventoryManager::~InventoryManager()
{}

// Called before render is available
bool InventoryManager::Awake(pugi::xml_node config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Inventity*>* item;
	Inventity* pEntity = NULL;

	for (item = inventities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool InventoryManager::Start() {

	PointerItemText = app->tex->Load("Assets/Textures/Interfaz/select.png");
	SelectedItemText = app->tex->Load("Assets/Textures/Interfaz/selected.png");
	EquipedItemText = app->tex->Load("Assets/Textures/Interfaz/equiped.png");

	bool ret = true;

	//Iterates over the entities and calls Start
	ListItem<Inventity*>* item;
	Inventity* pEntity = NULL;

	for (item = inventities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool InventoryManager::CleanUp()
{
	bool ret = true;
	ListItem<Inventity*>* item;
	item = inventities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	inventities.Clear();
	app->tex->UnLoad(PointerItemText);
	app->tex->UnLoad(SelectedItemText);
	app->tex->UnLoad(EquipedItemText);
	return ret;
}
int highestId = -1;
Inventity* InventoryManager::CreateItem(EntityType type, int id, int ataque, int durabilidad, int magia, float peso, int defensa)
{
	Inventity* entity = nullptr;


	//L03: DONE 3a: Instantiate entity according to the type and add the new entity to the list of Entities
	switch (type)
	{
	case EntityType::ITEM:
	{
		int newId = 0;
		for (ListItem<Inventity*>* item = inventities.start; item != nullptr; item = item->next)
		{

			item->data->id = newId;
			newId++;
		}
		if (inventities.Count() > 0)
		{
			highestId = inventities.end->data->id;
		}
		else
		{
			highestId = -1;
		}

		Iteminv* itemm = new Iteminv();
		itemm->id = highestId + 1;
		itemm->type = InventityType::ITEM;
		itemm->icon = app->tex->Load("Assets/Textures/CoinIcon.png");
		itemm->stackable = true;
		entity = itemm;

		break;
	}

	case EntityType::ITEM_GARRA:
	{
		int newId = 0;
		for (ListItem<Inventity*>* item = inventities.start; item != nullptr; item = item->next)
		{

			item->data->id = newId;
			newId++;
		}
		/*for (ListItem<Inventity*>* item = inventities.start; item != NULL; item = item->next)
		{
			if (item->data->id > highestId)
			{
				highestId = item->data->id;
			}
		}*/
		if (inventities.Count() > 0)
		{
			highestId = inventities.end->data->id;
		}
		else
		{
			highestId = -1;
		}


		GarraInv* garra = new GarraInv();
		garra->id = highestId + 1;
		garra->type = InventityType::GARRA;
		garra->stackable = true;
		/*sword->damage = ataque;
		sword->durability = durabilidad;
		sword->magic = magia;
		sword->weight = peso;*/
		garra->icon = app->tex->Load("Assets/Textures/Interfaz/garraInv.png");
		entity = garra;
		break;
	}
	case EntityType::ITEM_DIAMANTE:
	{
		int newId = 0;
		for (ListItem<Inventity*>* item = inventities.start; item != nullptr; item = item->next)
		{

			item->data->id = newId;
			newId++;
		}
		/*for (ListItem<Inventity*>* item = inventities.start; item != NULL; item = item->next)
		{
			if (item->data->id > highestId)
			{
				highestId = item->data->id;
			}
		}*/
		if (inventities.Count() > 0)
		{
			highestId = inventities.end->data->id;
		}
		else
		{
			highestId = -1;
		}


		DiamanteInv* diamante = new DiamanteInv();
		diamante->id = highestId + 1;
		diamante->type = InventityType::DIAMANTE;
		diamante->stackable = true;
		/*sword->damage = ataque;
		sword->durability = durabilidad;
		sword->magic = magia;
		sword->weight = peso;*/
		diamante->icon = app->tex->Load("Assets/Textures/Interfaz/diamante_inv.png");
		entity = diamante;
		break;
	}
	case EntityType::ITEM_OJO:
	{
		int newId = 0;
		for (ListItem<Inventity*>* item = inventities.start; item != nullptr; item = item->next)
		{

			item->data->id = newId;
			newId++;
		}
		/*for (ListItem<Inventity*>* item = inventities.start; item != NULL; item = item->next)
		{
			if (item->data->id > highestId)
			{
				highestId = item->data->id;
			}
		}*/
		if (inventities.Count() > 0)
		{
			highestId = inventities.end->data->id;
		}
		else
		{
			highestId = -1;
		}


		OjoInv* ojo = new OjoInv();
		ojo->id = highestId + 1;
		ojo->type = InventityType::OJO;
		ojo->stackable = true;
		/*sword->damage = ataque;
		sword->durability = durabilidad;
		sword->magic = magia;
		sword->weight = peso;*/
		ojo->icon = app->tex->Load("Assets/Textures/Entidades/Items/texture_eye.png");
		entity = ojo;
		break;
	}
	case EntityType::ITEM_ARMADURA:
	{
		for (ListItem<Inventity*>* item = inventities.start; item != NULL; item = item->next)
		{
			if (item->data->id > highestId)
			{
				highestId = item->data->id;
			}
		}

		ArmaduraInv* armadura = new ArmaduraInv();
		armadura->id = highestId + 1;
		armadura->type = InventityType::ARMADURA;
		armadura->defense = defensa;
		armadura->durability = durabilidad;
		armadura->weight = peso;
		armadura->icon = app->tex->Load("Assets/Textures/armaduraicon.png");
		entity = armadura;
	}

	default:
		break;
	}



	AddItem(entity);

	return entity;
}

bool InventoryManager::IsFull()
{

	// Encontrar el ID más alto actualmente asignado


	// Verificar si el siguiente ID disponible es 9
	if (inventities.Count() == 10) {
		return true;
	}
	else {
		return false;
	}

}

bool InventoryManager::LoadState(pugi::xml_node node)
{
	bool ret = true;

	ListItem<Inventity*>* item;
	item = inventities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	inventities.Clear();


	//Inventity* inventoryItem = app->inventoryManager->CreateItem(, 0, 0, 0, 0, 0, 0);
	for (pugi::xml_node itemNode = node.child("inventory").child("inventity"); itemNode; itemNode = itemNode.next_sibling("inventity"))
	{

		Inventity* itemLoaded = nullptr;

		switch ((InventityType)itemNode.attribute("type").as_int())
		{

		case InventityType::ARMADURA:
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_ARMADURA, 0, 0, 0, 0, 0, 0);
			break;
		case InventityType::GARRA:
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_GARRA, 0, 0, 0, 0, 0, 0);
			break;
		case InventityType::DIAMANTE:
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_DIAMANTE, 0, 0, 0, 0, 0, 0);
			break;
		case InventityType::OJO:
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_OJO, 0, 0, 0, 0, 0, 0);
			break;
		case InventityType::ESPADA2:
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_ARMADURA, 0, 0, 0, 0, 0, 0);
			break;
		case InventityType::ITEM:
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_ARMADURA, 0, 0, 0, 0, 0, 0);
			break;
		case InventityType::UNKNOWN:
			break;
		default:
			break;
		}
		itemLoaded->quantity = itemNode.attribute("quantity").as_int();

	}




	return ret;
}

bool InventoryManager::SaveState(pugi::xml_node node)
{
	bool ret = true;
	pugi::xml_node inventoryNode = node.append_child("inventory");

	for (int i = 0; i < inventities.Count(); i++) {

		Inventity* inventoryItem = inventities.At(i)->data;

		pugi::xml_node inventoryItemNode = inventoryNode.append_child("inventity");
		inventoryItemNode.append_attribute("type").set_value((int)inventoryItem->type);
		inventoryItemNode.append_attribute("quantity").set_value(inventoryItem->quantity);
	}

	/*playerNode.append_attribute("x").set_value(player->position.x);
	playerNode.append_attribute("y").set_value(player->position.y);
	playerNode.append_attribute("sceneLevel").set_value(app->sceneLevel);

	playerNode.append_attribute("coins").set_value(app->scene->getPlayer()->coins);

	pugi::xml_node cameraNode = node.append_child("camera");
	cameraNode.append_attribute("x").set_value(app->render->camera.x);
	cameraNode.append_attribute("y").set_value(app->render->camera.y);



	pugi::xml_node coinsListNode = node.append_child("coinsPicked");

	for (int i = 0; i < app->entityManager->coins.Count(); i++) {
		pugi::xml_node cointNode = coinsListNode.append_child("coin");
		cointNode.append_attribute("x").set_value(app->entityManager->coins.At(i)->data->position.x);
		cointNode.append_attribute("y").set_value(app->entityManager->coins.At(i)->data->position.y);
		cointNode.append_attribute("active").set_value(app->entityManager->coins.At(i)->data->active);
	}*/


	return ret;
}

void InventoryManager::DestroyItem(Inventity* entity)
{
	ListItem<Inventity*>* item;

	for (item = inventities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) inventities.Del(item);
	}
}

void InventoryManager::DestroyItem2(int entityId)
{
	ListItem<Inventity*>* item;

	for (item = inventities.start; item != NULL; item = item->next)
	{

		if (item->data->id == entityId) // Comprueba si el ID coincide
		{
			if (item->data->stackable && item->data->quantity > 1)
			{
				item->data->quantity--;
			}
			else
			{
				inventities.Del(item);
				delete item->data; // Libera la memoria de la espada eliminada
			}


			break; // Termina el bucle después de eliminar la espada
		}


	}
	// Reasignar los IDs después de la eliminación
	int newId = 0;
	for (item = inventities.start; item != nullptr; item = item->next)
	{

		item->data->id = newId;
		newId++;
	}
}

void InventoryManager::UseItemSelected(int id)
{

	ListItem<Inventity*>* item;

	/*for (item = inventities.start; item != NULL; item = item->next)
	{
		item->data->active = false;
	}*/
	bool foundMatchingID = false;

	for (item = inventities.start; item != NULL; item = item->next)
	{

		if (item->data->id == id) // Comprueba si el ID coincide
		{
			foundMatchingID = true;
			item->data->active = true;
			switch (item->data->type)
			{
				//case InventityType::CU:
				//	{
				//		//app->scene->GetPlayer()->espadaHierro = false; //ponemos la textura de la correspondiente
				//		//app->scene->GetPlayer()->espadaMadera = true;
				//		//app->scene->GetPlayer()->armaduraPoner = false;
				//		//Swordinv* espada = dynamic_cast<Swordinv*>(item->data); // Convierte a Espada si es posible

				//		//	app->scene->GetPlayer()->espadaMadera = true;
				//		//	app->scene->GetPlayer()->espadaHierro = false;
				//		//	app->scene->GetPlayer()->ataque = espada->damage;
				//		//	app->scene->GetPlayer()->durabilidadArma = espada->durability;
				//		//	app->scene->GetPlayer()->magia = espada->magic;
				//		//	app->scene->GetPlayer()->peso = espada->weight;
				//		
				//		

				//		
				//		break;
				//	}
			case InventityType::ESPADA2:
			{
				//app->scene->GetPlayer()->espadaHierro = true; //ponemos la textura de la correspondiente
				//app->scene->GetPlayer()->espadaMadera = false;
				//app->scene->GetPlayer()->armaduraPoner = false;
				//Swordinv* espada = dynamic_cast<Swordinv*>(item->data); // Convierte a Espada si es posible

				//app->scene->GetPlayer()->ataque = espada->damage;
				//app->scene->GetPlayer()->durabilidadArma = espada->durability;
				//app->scene->GetPlayer()->magia = espada->magic;
				//app->scene->GetPlayer()->peso = espada->weight;


				break;
			}
			case InventityType::ARMADURA:
			{
				//app->scene->GetPlayer()->armaduraPoner = true; //ponemos la textura de la correspondiente
				//app->scene->GetPlayer()->espadaMadera = false;
				//app->scene->GetPlayer()->espadaHierro = false;
				//ArmaduraInv* armadura = dynamic_cast<ArmaduraInv*>(item->data); // Convierte a Espada si es posible

				//app->scene->GetPlayer()->defensa = armadura->defense;
				//app->scene->GetPlayer()->durabilidadArma = armadura->durability;
				//app->scene->GetPlayer()->magia = armadura->magic;
				//app->scene->GetPlayer()->peso = armadura->weight;


				break;
			}
			}


		}


	}
	if (!foundMatchingID)
	{
		/*app->scene->GetPlayer()->espadaMadera = false;
		app->scene->GetPlayer()->espadaHierro = false;
		app->scene->GetPlayer()->armaduraPoner = false;*/
	}
}

void InventoryManager::OnMovePointer()
{

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN && PointerPosition.x < 143) {
		PointerPosition.x += 100;
		PointerId += 1;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN && PointerPosition.x > -43) {
		PointerPosition.x -= 100;
		PointerId -= 1;
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && PointerPosition.y < 1) {
		PointerPosition.y += 76;
		PointerId += 5;
	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && PointerPosition.y > -160) {
		PointerPosition.y -= 76;
		PointerId -= 5;
	}
}

void InventoryManager::AddItem(Inventity* entity)
{

	bool CuernoEncontrado = false;

	bool DiamanteEncontrado = false;

	bool OjoEncontrado = false;
	if (entity != nullptr) {

		if (entity->stackable)
		{
			if (entity->type == InventityType::GARRA)
			{


				for (int i = 0; i < inventities.Count(); i++) {
					if (inventities.At(i)->data->type == InventityType::GARRA) {
						inventities.At(i)->data->quantity += entity->quantity;
						CuernoEncontrado = true;
						break;
					}

				}
				if (!CuernoEncontrado)
				{
					inventities.Add(entity);
				}
			}
			else if (entity->type == InventityType::DIAMANTE)
			{
				for (int i = 0; i < inventities.Count(); i++) {

					if (inventities.At(i)->data->type == InventityType::DIAMANTE) {
						inventities.At(i)->data->quantity += entity->quantity;
						DiamanteEncontrado = true;
						break;
					}
				}
				if (!DiamanteEncontrado)
				{
					inventities.Add(entity);
				}
			}
			else if (entity->type == InventityType::OJO)
			{
				for (int i = 0; i < inventities.Count(); i++) {

					if (inventities.At(i)->data->type == InventityType::OJO) {
						inventities.At(i)->data->quantity += entity->quantity;
						OjoEncontrado = true;
						break;
					}
				}
				if (!OjoEncontrado)
				{
					inventities.Add(entity);
				}
			}

		}
		else {
			inventities.Add(entity);
		}

		ListItem<Inventity*>* item;

		int newId = 0;
		for (item = inventities.start; item != nullptr; item = item->next)
		{

			item->data->id = newId;
			newId++;
		}


	}
}

bool InventoryManager::Update(float dt)
{

	bool ret = true;



	if (mostrar == true)
	{
		OnMovePointer();

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			options = true;
			selected = { PointerPosition.x, PointerPosition.y };
			selectedId = PointerId;

		}

		if (options == true)
		{
			if (app->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN)
			{
				DestroyItem2(selectedId);
				options = false;
				selected = { -1000, -1000 };

			}

			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				equiped = { PointerPosition.x, PointerPosition.y };
				equipedId = PointerId;
				UseItemSelected(equipedId);
				options = false;
				selected = { -1000, -1000 };

			}
		}



	}






	return ret;
}

bool InventoryManager::PostUpdate()
{
	bool ret = true;
	app->tex->GetSize(InventoryBackground, texW, texH);






	if (mostrar == true)
	{
		ListItem<Inventity*>* item;
		Inventity* pEntity = NULL;

		app->render->DrawTexture(EquipedItemText, equiped.x, equiped.y, SDL_FLIP_NONE, 0, 0);

		app->render->DrawTexture(PointerItemText, PointerPosition.x, PointerPosition.y, SDL_FLIP_NONE, 0, 0);
		app->render->DrawTexture(SelectedItemText, selected.x, selected.y, SDL_FLIP_NONE, 0, 0);

		for (item = inventities.start; item != nullptr; item = item->next)
		{
			pEntity = item->data;

			if ((pEntity->stackable))
			{
				std::string quantityStr = std::to_string(pEntity->quantity);

				if (pEntity->type == InventityType::GARRA)
				{

					if (pEntity->quantity > 1)
					{


						if (item->data->id < 5)
						{
							app->render->DrawText(quantityStr.c_str(), 320 + item->data->id * 100, 260, 20, 20);
						}
						else
						{
							app->render->DrawText(quantityStr.c_str(), 485 + (item->data->id - 5) * 100, 420, 20, 20);
						}
					}
					{
						if (item->data->id < 5)
						{
							app->render->DrawTexture(pEntity->icon, 290 + pEntity->id * 100, 230, SDL_FLIP_NONE, 0, 0);
						}
						else
						{
							app->render->DrawTexture(pEntity->icon, 445 + ((pEntity->id - 5) * 100), 380, SDL_FLIP_NONE, 0, 0);
						}

					}

				}
				else if (pEntity->type == InventityType::DIAMANTE)
				{

					if (pEntity->quantity > 1)
					{


						if (item->data->id < 5)
						{
							app->render->DrawText(quantityStr.c_str(), 320 + item->data->id * 100, 260, 20, 20);
						}
						else
						{
							app->render->DrawText(quantityStr.c_str(), 485 + (item->data->id - 5) * 100, 420, 20, 20);
						}
					}
					{
						if (item->data->id < 5)
						{
							app->render->DrawTexture(pEntity->icon, 290 + pEntity->id * 100, 230, SDL_FLIP_NONE, 0, 0);
						}
						else
						{
							app->render->DrawTexture(pEntity->icon, 445 + ((pEntity->id - 5) * 100), 380, SDL_FLIP_NONE, 0, 0);
						}

					}

				}
				else if (pEntity->type == InventityType::OJO)
				{

					if (pEntity->quantity > 1)
					{


						if (item->data->id < 5)
						{
							app->render->DrawText(quantityStr.c_str(), 320 + item->data->id * 100, 260, 20, 20);
						}
						else
						{
							app->render->DrawText(quantityStr.c_str(), 485 + (item->data->id - 5) * 100, 420, 20, 20);
						}
					}
					{
						if (item->data->id < 5)
						{
							app->render->DrawTexture(pEntity->icon, 290 + pEntity->id * 100, 230, SDL_FLIP_NONE, 0, 0);
						}
						else
						{
							app->render->DrawTexture(pEntity->icon, 445 + ((pEntity->id - 5) * 100), 380, SDL_FLIP_NONE, 0, 0);
						}

					}

				}

			}
			else
			{
				if (pEntity->id < 5) //if(inventities.cout() < 5)
				{
					app->render->DrawTexture(pEntity->icon, 445 + pEntity->id * 100, 300, SDL_FLIP_NONE, 0, 0);
				}
				else
				{

					app->render->DrawTexture(pEntity->icon, 445 + ((pEntity->id - 5) * 100), 380, SDL_FLIP_NONE, 0, 0);
				}


			}







		}


	}



	return ret;
}






