
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
Inventity* InventoryManager::CreateItem(EntityType type, std::string descripcioon, std::string tipoo, std::string nombre)
{
	Inventity* entity = nullptr;

	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	if (parseResult) {
		configNode = configFile.child("config");
	}

	highestId = (inventities.Count() > 0) ? inventities.end->data->id : -1;

	entity = new Inventity();
	entity->id = highestId + 1;
	entity->stackable = true;
	entity->desc = descripcioon;
	entity->tipo = tipoo;
	entity->name = nombre.c_str();
	switch (type)
	{
	case EntityType::ITEM_DIAMANTE:
		/*entity->iconPath = (char*)configNode.child("item_diamante").attribute("texturePath").as_string();
		entity->icon = app->tex->Load(entity->iconPath);*/
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_diamante.png");
		entity->type = InventityType::DIAMANTE;
		entity->closeUpItems = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_diamante.png");
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
	case EntityType::ITEM_MONEDA:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_moneda.png");
		entity->type = InventityType::MONEDA;
		break;
	case EntityType::ITEM_SALIVA:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_saliva.png");
		entity->type = InventityType::SALIVA;
		break;
	case EntityType::ITEM_POCION_VIDA_1:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_pocion_vida_1.png");
		entity->type = InventityType::POCION_VIDA_1;
		break;
	case EntityType::ITEM_POCION_VIDA_2:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_pocion_vida_2.png");
		entity->type = InventityType::POCION_VIDA_2;
		break;
	case EntityType::ITEM_POCION_VIDA_3:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_pocion_vida_3.png");
		entity->type = InventityType::POCION_VIDA_3;
		break;
	case EntityType::ITEM_POCION_VIDA_MAX:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_pocion_vida_max.png");
		entity->type = InventityType::POCION_VIDA_MAX;
		break;
	case EntityType::ITEM_POCION_REGENERACION:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_pocion_regeneracion.png");
		entity->type = InventityType::POCION_REGENERACION;
		break;
	case EntityType::ITEM_POCION_DANO:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_pocion_dano.png");
		entity->type = InventityType::POCION_DANO;
		break;
	case EntityType::ITEM_POCION_VELOCIDAD:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_pocion_velocidad.png");
		entity->type = InventityType::POCION_VELOCIDAD;
		break;
	case EntityType::ITEM_ORBE_MAGICO:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_orbe_magico.png");
		entity->type = InventityType::ORBE_MAGICO;
		break;
	default:
		break;
	}


	AddItem(entity);

	return entity;
}

Inventity* InventoryManager::CreateItem(InventityType type) {

	pugi::xml_document configFile;
	pugi::xml_node entitiesDataNode;
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	entitiesDataNode = configFile.child("config").child("entities_data");
	pugi::xml_node itemConfigurationNode;

	Inventity* inventity = nullptr;


	switch (type) {
		case InventityType::GARRA:
			itemConfigurationNode = entitiesDataNode.child("item_garra");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_GARRA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::OJO:
			itemConfigurationNode = entitiesDataNode.child("item_ojo");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_OJO, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::DIAMANTE:
			itemConfigurationNode = entitiesDataNode.child("item_diamante");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_DIAMANTE, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::VISCERA:
			itemConfigurationNode = entitiesDataNode.child("item_viscera");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_VISCERAS, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::RUBI:
			itemConfigurationNode = entitiesDataNode.child("item_rubi");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_RUBI, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::POLVORA:
			itemConfigurationNode = entitiesDataNode.child("item_polvora");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_POLVORA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::HUESO:
			itemConfigurationNode = entitiesDataNode.child("item_hueso");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_HUESO, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::DIENTE:
			itemConfigurationNode = entitiesDataNode.child("item_diente");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_DIENTE, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::COLA:
			itemConfigurationNode = entitiesDataNode.child("item_cola");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_COLA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::CARBON:
			itemConfigurationNode = entitiesDataNode.child("item_carbon");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_CARBON, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::AMATISTA:
			itemConfigurationNode = entitiesDataNode.child("item_amatista");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_AMATISTA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::PEZ_PEQUENO:
			itemConfigurationNode = entitiesDataNode.child("item_pez_pequeno");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_PEZ_PEQUENO, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::PEZ_MEDIANO:
			itemConfigurationNode = entitiesDataNode.child("item_pez_mediano");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_PEZ_MEDIANO, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::PEZ_GRANDE:
			itemConfigurationNode = entitiesDataNode.child("item_pez_grande");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_PEZ_GRANDE, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::MONEDA:
			itemConfigurationNode = entitiesDataNode.child("item_moneda");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_MONEDA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::SALIVA:
			itemConfigurationNode = entitiesDataNode.child("item_saliva");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_SALIVA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::POCION_VIDA_1:
			itemConfigurationNode = entitiesDataNode.child("item_pocion_vida_1");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_POCION_VIDA_1, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::POCION_VIDA_2:
			itemConfigurationNode = entitiesDataNode.child("item_pocion_vida_2");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_POCION_VIDA_2, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::POCION_VIDA_3:
			itemConfigurationNode = entitiesDataNode.child("item_pocion_vida_3");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_POCION_VIDA_3, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::POCION_VIDA_MAX:
			itemConfigurationNode = entitiesDataNode.child("item_pocion_vida_max");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_POCION_VIDA_MAX, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::POCION_REGENERACION:
			itemConfigurationNode = entitiesDataNode.child("item_pocion_regenracion");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_POCION_REGENERACION, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::POCION_DANO:
			itemConfigurationNode = entitiesDataNode.child("item_pocion_dano");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_POCION_DANO, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::POCION_VELOCIDAD:
			itemConfigurationNode = entitiesDataNode.child("item_pocion_velocidad");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_POCION_VELOCIDAD, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::ORBE_MAGICO:
			itemConfigurationNode = entitiesDataNode.child("item_pocion_orbe_magico");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_ORBE_MAGICO, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
	}


	return inventity;


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
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_ARMADURA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::GARRA:
			itemConfigurationNode = entitiesDataNode.child("item_garra");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_GARRA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::DIAMANTE:
			itemConfigurationNode = entitiesDataNode.child("item_diamante");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_DIAMANTE, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::OJO:
			itemConfigurationNode = entitiesDataNode.child("item_ojo");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_OJO, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::ESPADA2:
			itemConfigurationNode = entitiesDataNode.child("item_garra");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_ARMADURA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::ITEM:
			itemConfigurationNode = entitiesDataNode.child("item_garra");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_ARMADURA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::VISCERA:
			itemConfigurationNode = entitiesDataNode.child("item_viscera");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_VISCERAS, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::RUBI:
			itemConfigurationNode = entitiesDataNode.child("item_rubi");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_RUBI, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::POLVORA:
			itemConfigurationNode = entitiesDataNode.child("item_polvora");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_POLVORA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::NOTA:
			itemConfigurationNode = entitiesDataNode.child("item_nota");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_NOTA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::HUESO:
			itemConfigurationNode = entitiesDataNode.child("item_hueso");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_HUESO, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::DIENTE:
			itemConfigurationNode = entitiesDataNode.child("item_diente");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_DIENTE, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::COLA:
			itemConfigurationNode = entitiesDataNode.child("item_cola");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_COLA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::CARBON:
			itemConfigurationNode = entitiesDataNode.child("item_carbon");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_CARBON, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::AMATISTA:
			itemConfigurationNode = entitiesDataNode.child("item_amatista");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_AMATISTA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::PEZ_PEQUENO:
			itemConfigurationNode = entitiesDataNode.child("item_pez_pequeno");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_PEZ_PEQUENO, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::PEZ_MEDIANO:
			itemConfigurationNode = entitiesDataNode.child("item_pez_mediano");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_PEZ_MEDIANO, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::PEZ_GRANDE:
			itemConfigurationNode = entitiesDataNode.child("item_pez_grande");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_PEZ_GRANDE, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;

		case InventityType::UNKNOWN:
			break;
		default:
			break;
		}
		itemLoaded->quantity = itemNode.attribute("quantity").as_int();

	}

	//inventoryMoneyNode.append_attribute("quantity").set_value(monedasObtenidas);

	monedasObtenidas = node.child("inventory").child("money").attribute("quantity").as_int(0);


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

	pugi::xml_node inventoryMoneyNode = inventoryNode.append_child("money");
	inventoryMoneyNode.append_attribute("quantity").set_value(monedasObtenidas);

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

int InventoryManager::GetInventityQuantity(InventityType type)
{
	int cantidad = 0;

	for (int i = 0; i < inventities.Count(); i++) {
		if (inventities.At(i)->data->type == type) {
			cantidad = inventities.At(i)->data->quantity;
			break;
		}
	}
	return cantidad;
}

void InventoryManager::DestroyItem(InventityType type, int cantidad)
{
	ListItem<Inventity*>* item;

	for (item = inventities.start; item != NULL; item = item->next)
	{

		if (item->data->type == type) 
		{
			if (item->data->stackable)
			{
				item->data->quantity -= cantidad;
				if (item->data->quantity <= 0)
				{
					item->data->quantity = 0;
					inventities.Del(item);
					
					delete item->data;
				}
			}
			else
			{
				inventities.Del(item);
				delete item->data; 
			}


			break;
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

		/*if (options == true)
		{*/
			if (app->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN)
			{
				DestroyItemById(selectedId);
				options = false;
				selected = { -1000, -1000 };

			}

			/*if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			{
				equiped = { PointerPosition.x, PointerPosition.y };
				equipedId = PointerId;
				UseItemSelected(equipedId);
				options = false;
				selected = { -1000, -1000 };

			}*/
		/*}*/



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
			if (pEntity->quantity > 0)
			{
				std::string quantityStr = std::to_string(pEntity->quantity);
				app->render->DrawText(quantityStr.c_str(), horizontalPosition, verticalPosition, 20, 20, 0, 0, 0, 0);			
				
			}
			horizontalPosition = 260 + columnIndex * 105; // Calcula la posición horizontal para pEntity
			verticalPosition = 160 + rowIndex * 104; // Calcula la posición vertical para pEntity

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
					app->render->DrawText(itum->data->name.GetString(), 680, 340, 90, 60, 0, 165, 42, 42);
					break;
				case InventityType::RUBI:
				case InventityType::POLVORA:
				case InventityType::DIAMANTE:
				case InventityType::CARBON:
				case InventityType::AMATISTA:
					app->render->DrawText(itum->data->tipo.c_str(), 680, 340, 90, 60, 0, 65, 136, 165);
					app->render->DrawTexture(itum->data->closeUpItems, 820, 200, 1.8, SDL_FLIP_NONE, 0, 0);
					
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






