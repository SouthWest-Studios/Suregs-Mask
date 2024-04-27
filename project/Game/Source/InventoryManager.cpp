
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
Inventity* InventoryManager::CreateItem(EntityType type, std::string descripcioon, std::string tipoo)
{
	Inventity* entity = nullptr;


	//L03: DONE 3a: Instantiate entity according to the type and add the new entity to the list of Entities
	//switch (type)
	//{
	//case EntityType::ITEM:
	//{
	//	int newId = 0;
	//	for (ListItem<Inventity*>* item = inventities.start; item != nullptr; item = item->next)
	//	{

	//		item->data->id = newId;
	//		newId++;
	//	}
	//	if (inventities.Count() > 0)
	//	{
	//		highestId = inventities.end->data->id;
	//	}
	//	else
	//	{
	//		highestId = -1;
	//	}

	//	Iteminv* itemm = new Iteminv();
	//	itemm->id = highestId + 1;
	//	itemm->type = InventityType::ITEM;
	//	itemm->icon = app->tex->Load("Assets/Textures/CoinIcon.png");
	//	itemm->stackable = true;
	//	entity = itemm;

	//	break;
	//}

	//case EntityType::ITEM_GARRA:
	//{
	//	int newId = 0;
	//	for (ListItem<Inventity*>* item = inventities.start; item != nullptr; item = item->next)
	//	{

	//		item->data->id = newId;
	//		newId++;
	//	}
	//	/*for (ListItem<Inventity*>* item = inventities.start; item != NULL; item = item->next)
	//	{
	//		if (item->data->id > highestId)
	//		{
	//			highestId = item->data->id;
	//		}
	//	}*/
	//	if (inventities.Count() > 0)
	//	{
	//		highestId = inventities.end->data->id;
	//	}
	//	else
	//	{
	//		highestId = -1;
	//	}


	//	GarraInv* garra = new GarraInv();
	//	garra->id = highestId + 1;
	//	garra->type = InventityType::GARRA;
	//	garra->stackable = true;
	//	garra->desc = descripcioon;
	//	garra->tipo = tipoo;

	//	/*sword->damage = ataque;
	//	sword->durability = durabilidad;
	//	sword->magic = magia;
	//	sword->weight = peso;*/
	//	garra->icon = app->tex->Load("Assets/Textures/Interfaz/garraInv.png");
	//	entity = garra;
	//	break;
	//}
	//case EntityType::ITEM_DIAMANTE:
	//{
	//	int newId = 0;
	//	for (ListItem<Inventity*>* item = inventities.start; item != nullptr; item = item->next)
	//	{

	//		item->data->id = newId;
	//		newId++;
	//	}
	//	/*for (ListItem<Inventity*>* item = inventities.start; item != NULL; item = item->next)
	//	{
	//		if (item->data->id > highestId)
	//		{
	//			highestId = item->data->id;
	//		}
	//	}*/
	//	if (inventities.Count() > 0)
	//	{
	//		highestId = inventities.end->data->id;
	//	}
	//	else
	//	{
	//		highestId = -1;
	//	}


	//	DiamanteInv* diamante = new DiamanteInv();
	//	diamante->id = highestId + 1;
	//	diamante->type = InventityType::DIAMANTE;
	//	diamante->stackable = true;
	//	diamante->desc = descripcioon;
	//	diamante->tipo = tipoo;
	//	/*sword->damage = ataque;
	//	sword->durability = durabilidad;
	//	sword->magic = magia;
	//	sword->weight = peso;*/
	//	diamante->icon = app->tex->Load("Assets/Textures/Interfaz/diamante_inv.png");
	//	entity = diamante;
	//	break;
	//}
	//case EntityType::ITEM_OJO:
	//{
	//	int newId = 0;
	//	for (ListItem<Inventity*>* item = inventities.start; item != nullptr; item = item->next)
	//	{

	//		item->data->id = newId;
	//		newId++;
	//	}
	//	/*for (ListItem<Inventity*>* item = inventities.start; item != NULL; item = item->next)
	//	{
	//		if (item->data->id > highestId)
	//		{
	//			highestId = item->data->id;
	//		}
	//	}*/
	//	if (inventities.Count() > 0)
	//	{
	//		highestId = inventities.end->data->id;
	//	}
	//	else
	//	{
	//		highestId = -1;
	//	}


	//	OjoInv* ojo = new OjoInv();
	//	ojo->id = highestId + 1;
	//	ojo->type = InventityType::OJO;
	//	ojo->stackable = true;
	//	ojo->desc = descripcioon;
	//	ojo->tipo = tipoo;
	//	/*sword->damage = ataque;
	//	sword->durability = durabilidad;
	//	sword->magic = magia;
	//	sword->weight = peso;*/
	//	ojo->icon = app->tex->Load("Assets/Textures/Interfaz/ojoInv.png");
	//	entity = ojo;
	//	break;
	//}
	//case EntityType::ITEM_COLA:
	//{

	//	
	//	



	//	OjoInv* ojo = new OjoInv();
	//	ojo->id = highestId + 1;
	//	ojo->type = InventityType::OJO;
	//	ojo->stackable = true;
	//	ojo->desc = descripcioon;
	//	ojo->tipo = tipoo;
	//	/*sword->damage = ataque;
	//	sword->durability = durabilidad;
	//	sword->magic = magia;
	//	sword->weight = peso;*/
	//	ojo->icon = app->tex->Load("Assets/Textures/Interfaz/ojoInv.png");
	//	entity = ojo;
	//	break;
	//}
	//case EntityType::ITEM_ARMADURA:
	//{
	//	for (ListItem<Inventity*>* item = inventities.start; item != NULL; item = item->next)
	//	{
	//		if (item->data->id > highestId)
	//		{
	//			highestId = item->data->id;
	//		}
	//	}

	//	/*ArmaduraInv* armadura = new ArmaduraInv();
	//	armadura->id = highestId + 1;
	//	armadura->type = InventityType::ARMADURA;
	//	armadura->defense = defensa;
	//	armadura->durability = durabilidad;
	//	armadura->weight = peso;
	//	armadura->icon = app->tex->Load("Assets/Textures/armaduraicon.png");
	//	entity = armadura;*/
	//}

	//default:
	//	break;
	//}

	highestId = (inventities.Count() > 0) ? inventities.end->data->id : -1;
	entity = new Inventity();
	entity->id = highestId + 1;
	entity->stackable = true;
	entity->desc = descripcioon;
	entity->tipo = tipoo;
	switch (type)
	{
	case EntityType::ITEM_DIAMANTE:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_diamante.png");
		entity->type = InventityType::DIAMANTE;
		break;
	case EntityType::ITEM_OJO:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_ojo.png");
		entity->type = InventityType::OJO;
		break;
	case EntityType::ITEM_GARRA:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_garra.png");
		entity->type = InventityType::GARRA;
		break;
	case EntityType::ITEM_HUESO:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_hueso.png");
		entity->type = InventityType::HUESO;
		break;
	case EntityType::ITEM_CARBON:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_carbon.png");
		entity->type = InventityType::CARBON;
		break;
	case EntityType::ITEM_VISCERAS:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_visceras.png");
		entity->type = InventityType::VISCERA;
		break;
	case EntityType::ITEM_RUBI:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_rubi.png");
		entity->type = InventityType::RUBI;
		break;
	case EntityType::ITEM_DIENTE:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_diente.png");
		entity->type = InventityType::DIENTE;
		break;
	case EntityType::ITEM_AMATISTA:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_amatista.png");
		entity->type = InventityType::AMATISTA;
		break;
	case EntityType::ITEM_COLA:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_cola.png");
		entity->type = InventityType::COLA;
		break;
	case EntityType::ITEM_POLVORA:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_polvora.png");
		entity->type = InventityType::POLVORA;
		break;
	case EntityType::ITEM_PEZ_PEQUENO:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_pez_pequeno.png");
		entity->type = InventityType::PEZ_PEQUENO;
		break;
	case EntityType::ITEM_PEZ_MEDIANO:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_pez_mediano.png");
		entity->type = InventityType::PEZ_MEDIANO;
		break;
	case EntityType::ITEM_PEZ_GRANDE:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_pez_grande.png");
		entity->type = InventityType::PEZ_GRANDE;
		break;
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
	if (inventities.Count() == 12) {
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

	pugi::xml_document configFile;
	pugi::xml_node entitiesDataNode;
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	entitiesDataNode = configFile.child("config").child("entities_data");

	//Inventity* inventoryItem = app->inventoryManager->CreateItem(, 0, 0, 0, 0, 0, 0);
	for (pugi::xml_node itemNode = node.child("inventory").child("inventity"); itemNode; itemNode = itemNode.next_sibling("inventity"))
	{

		Inventity* itemLoaded = nullptr;
		pugi::xml_node itemConfigurationNode;
		switch ((InventityType)itemNode.attribute("type").as_int())
		{

		case InventityType::ARMADURA:
			itemConfigurationNode = entitiesDataNode.child("item_garra");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_ARMADURA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string());
			break;
		case InventityType::GARRA:
			itemConfigurationNode = entitiesDataNode.child("item_garra");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_GARRA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string());
			break;
		case InventityType::DIAMANTE:
			itemConfigurationNode = entitiesDataNode.child("item_diamante");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_DIAMANTE, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string());
			break;
		case InventityType::OJO:
			itemConfigurationNode = entitiesDataNode.child("item_ojo");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_OJO, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string());
			break;
		case InventityType::ESPADA2:
			itemConfigurationNode = entitiesDataNode.child("item_garra");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_ARMADURA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string());
			break;
		case InventityType::ITEM:
			itemConfigurationNode = entitiesDataNode.child("item_garra");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_ARMADURA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string());
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

void InventoryManager::DestroyItemById(int entityId)
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

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN && PointerPosition.x < 300) {
		PointerPosition.x += 106;
		PointerId += 1;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN && PointerPosition.x > 176) {
		PointerPosition.x -= 106;
		PointerId -= 1;
	}

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && PointerPosition.y < 200) {
		PointerPosition.y += 103;
		PointerId += 3;
	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN && PointerPosition.y > -60) {
		PointerPosition.y -= 103;
		PointerId -= 3;
	}
}

void InventoryManager::AddItem(Inventity* entity)
{

	bool itemEncontrado = false;
	if (entity != nullptr) {

		for (int i = 0; i < inventities.Count(); i++) {
			if (inventities.At(i)->data->type == entity->type) {
				inventities.At(i)->data->quantity += entity->quantity;
				itemEncontrado = true;
				break;
			}

		}
		if (!itemEncontrado)
		{
			inventities.Add(entity);
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
				DestroyItemById(selectedId);
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

		app->render->DrawTexture(EquipedItemText, equiped.x, equiped.y, 0.8, SDL_FLIP_NONE, 0, 0);

		app->render->DrawTexture(PointerItemText, PointerPosition.x, PointerPosition.y, 0.8, SDL_FLIP_NONE, 0, 0);
		app->render->DrawTexture(SelectedItemText, selected.x, selected.y, 0.8, SDL_FLIP_NONE, 0, 0);

		for (item = inventities.start; item != nullptr; item = item->next)
		{
			pEntity = item->data;
			int rowIndex = item->data->id / maxItemsPerRow; // Calcula el índice de la fila
			int columnIndex = item->data->id % maxItemsPerRow; // Calcula el índice de la columna
			int horizontalPosition = 320 + columnIndex * 105; // Calcula la posición horizontal
			int verticalPosition = 230 + rowIndex * 103; // Calcula la posición vertical
			if (pEntity->quantity > 1)
			{
				std::string quantityStr = std::to_string(pEntity->quantity);
				app->render->DrawText(quantityStr.c_str(), horizontalPosition, verticalPosition, 20, 20, 0, 0, 0, 0);			
				
			}
			
			horizontalPosition = 280 + columnIndex * 105; // Calcula la posición horizontal para pEntity
			verticalPosition = 180 + rowIndex * 104; // Calcula la posición vertical para pEntity

			app->render->DrawTexture(pEntity->icon, horizontalPosition, verticalPosition, 0.8, SDL_FLIP_NONE, 0, 0);

		}


	}
	if (mostrar == true)
	{
		ListItem<Inventity*>* itum;
		for (itum = inventities.start; itum != nullptr; itum = itum->next)
		{
			if (PointerId == itum->data->id)
			{
				app->render->DrawText(itum->data->desc.c_str(), 680, 450, 270, 400, 0, 0, 0, 0, false);
				switch (itum->data->type)	
				{
				case InventityType::OJO:
				case InventityType::GARRA:
				case InventityType::COLA:
				case InventityType::VISCERA:
				case InventityType::HUESO:
				case InventityType::DIENTE:
					app->render->DrawText(itum->data->tipo.c_str(), 680, 340, 90, 60, 0, 165, 42, 42);
					break;
				case InventityType::RUBI:
				case InventityType::POLVORA:
				case InventityType::DIAMANTE:
				case InventityType::CARBON:
				case InventityType::AMATISTA:
					app->render->DrawText(itum->data->tipo.c_str(), 680, 340, 90, 60, 0, 65, 136, 165);
					break;
				default:
					app->render->DrawText(itum->data->tipo.c_str(), 680, 340, 90, 60, 0, 135, 206, 235);
					break;
				}
			}

		}
	}
	return ret;
}






