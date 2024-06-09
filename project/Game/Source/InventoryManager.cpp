
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
#include "Hud.h"
#include "SString.h"
#include "Elevator.h"
#include "ElevatorMenu.h"
#include "Audio.h"
#include "Menu.h"
#include "Scene_Pueblo.h"
#include "Scene_Pueblo_Tutorial.h"
#include "QuestManager.h"

InventoryManager::InventoryManager(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name = ("iventorymanager");
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

	PointerItemText = app->tex->Load("Assets/Textures/Interfaz/selectAnimation.png");
	SelectedItemText = app->tex->Load("Assets/Textures/Interfaz/selected.png");
	EquipedItemText = app->tex->Load("Assets/Textures/Interfaz/equiped.png");

	change_inventory_mark_fx = app->audio->LoadAudioFx("change_inventory_mark_fx");


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

	//"Assets/Textures/Interfaz/selectAnimation.png"
	pointerSpritePosition = SPosition.SpritesPos(24, 150, 150, 750);
	pointerAnim.LoadAnim("poinerInventory", "selectAnim", pointerSpritePosition);

	currentPointerAnimation = &pointerAnim;

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
Inventity* InventoryManager::CreateItem(EntityType type, std::string descripcioon, std::string tipoo, std::string nombre, bool addInventory)
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
		entity->objectID;
		break;
	case EntityType::ITEM_OJO:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_ojo.png");
		entity->type = InventityType::OJO;
		entity->objectID;
		break;
	case EntityType::ITEM_GARRA:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_garra.png");
		entity->type = InventityType::GARRA;
		entity->objectID;
		break;
	case EntityType::ITEM_HUESO:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_hueso.png");
		entity->type = InventityType::HUESO;
		entity->objectID;
		break;
	case EntityType::ITEM_CARBON:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_carbon.png");
		entity->type = InventityType::CARBON;
		entity->objectID;
		break;
	case EntityType::ITEM_VISCERAS:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_visceras.png");
		entity->type = InventityType::VISCERA;
		entity->objectID;
		break;
	case EntityType::ITEM_RUBI:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_rubi.png");
		entity->type = InventityType::RUBI;
		entity->objectID;
		break;
	case EntityType::ITEM_DIENTE:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_diente.png");
		entity->type = InventityType::DIENTE;
		entity->objectID;
		break;
	case EntityType::ITEM_AMATISTA:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_amatista.png");
		entity->type = InventityType::AMATISTA;
		entity->objectID;
		break;
	case EntityType::ITEM_COLA:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_cola.png");
		entity->type = InventityType::COLA;
		entity->objectID;
		break;
	case EntityType::ITEM_POLVORA:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_polvora.png");
		entity->type = InventityType::POLVORA;
		entity->objectID;
		break;
	case EntityType::ITEM_PEZ_PEQUENO:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_pez_pequeno.png");
		entity->type = InventityType::PEZ_PEQUENO;
		entity->objectID;
		break;
	case EntityType::ITEM_PEZ_MEDIANO:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_pez_mediano.png");
		entity->type = InventityType::PEZ_MEDIANO;
		entity->objectID;
		break;
	case EntityType::ITEM_PEZ_GRANDE:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_pez_grande.png");
		entity->type = InventityType::PEZ_GRANDE;
		entity->objectID;
		break;
	case EntityType::ITEM_MONEDA:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_moneda.png");
		entity->type = InventityType::MONEDA;
		entity->objectID;
		break;
	case EntityType::ITEM_SALIVA:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_saliva.png");
		entity->type = InventityType::SALIVA;
		entity->objectID;
		break;
	case EntityType::ITEM_POCION_VIDA_1:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_pocion_vida_1.png");
		entity->type = InventityType::POCION_VIDA_1;
		entity->objectID = 0;
		break;
	case EntityType::ITEM_POCION_VIDA_2:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_pocion_vida_2.png");
		entity->type = InventityType::POCION_VIDA_2;
		entity->objectID = 1;
		break;
	case EntityType::ITEM_POCION_VIDA_3:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_pocion_vida_3.png");
		entity->type = InventityType::POCION_VIDA_3;
		entity->objectID = 2;
		break;
	case EntityType::ITEM_POCION_VIDA_MAX:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_pocion_vida_max.png");
		entity->type = InventityType::POCION_VIDA_MAX;
		entity->objectID = 3;
		break;
	case EntityType::ITEM_POCION_REGENERACION:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_pocion_regeneracion.png");
		entity->type = InventityType::POCION_REGENERACION;
		entity->objectID = 2;
		break;
	case EntityType::ITEM_POCION_DANO:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_pocion_dano.png");
		entity->type = InventityType::POCION_DANO;
		entity->objectID = 5;
		break;
	case EntityType::ITEM_POCION_VELOCIDAD:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_pocion_velocidad.png");
		entity->type = InventityType::POCION_VELOCIDAD;
		entity->objectID = 6;
		break;
	case EntityType::ITEM_ORBE_MAGICO:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_orbe_magico.png");
		entity->type = InventityType::ORBE_MAGICO;
		entity->objectID = 7;
		break;
	case EntityType::ITEM_ZAFIRO:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_zafiro.png");
		entity->type = InventityType::ZAFIRO;
		break;
	case EntityType::ITEM_ESPADA_NV2:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_espada_nv2.png");
		entity->type = InventityType::ESPADA_NV2;
		break;
	case EntityType::ITEM_ESPADA_NV3:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_espada_nv3.png");
		entity->type = InventityType::ESPADA_NV3;
		break;
	case EntityType::ITEM_ESPADA_NV4:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_espada_nv4.png");
		entity->type = InventityType::ESPADA_NV4;
		break;
	case EntityType::ITEM_ESPADA_NV5:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_espada_nv5.png");
		entity->type = InventityType::ESPADA_NV5;
		break;
	case EntityType::ITEM_ESPADA_NV6:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_espada_nv6.png");
		entity->type = InventityType::ESPADA_NV6;
		break;
	case EntityType::ITEM_ESPADA_NV7:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_espada_nv7.png");
		entity->type = InventityType::ESPADA_NV7;
		break;
	case EntityType::ITEM_ESPADA_NV8:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_espada_nv8.png");
		entity->type = InventityType::ESPADA_NV8;
		break;
	case EntityType::ITEM_ESPADA_NV9:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_espada_nv9.png");
		entity->type = InventityType::ESPADA_NV9;
		break;
	case EntityType::ITEM_ESPADA_NV10:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_espada_nv10.png");
		entity->type = InventityType::ESPADA_NV10;
		break;
	case EntityType::ITEM_ARMADURA_NV2:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_armadura_nv2.png");
		entity->type = InventityType::ARMADURA_NV2;
		break;
	case EntityType::ITEM_ARMADURA_NV3:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_armadura_nv3.png");
		entity->type = InventityType::ARMADURA_NV3;
		break;
	case EntityType::ITEM_ARMADURA_NV4:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_armadura_nv4.png");
		entity->type = InventityType::ARMADURA_NV4;
		break;
	case EntityType::ITEM_ARMADURA_NV5:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_armadura_nv5.png");
		entity->type = InventityType::ARMADURA_NV5;
		break;
	case EntityType::ITEM_ARMADURA_NV6:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_armadura_nv6.png");
		entity->type = InventityType::ARMADURA_NV6;
		break;
	case EntityType::ITEM_ARMADURA_NV7:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_armadura_nv7.png");
		entity->type = InventityType::ARMADURA_NV7;
		break;
	case EntityType::ITEM_ARMADURA_NV8:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_armadura_nv8.png");
		entity->type = InventityType::ARMADURA_NV8;
		break;
	case EntityType::ITEM_ARMADURA_NV9:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_armadura_nv9.png");
		entity->type = InventityType::ARMADURA_NV9;
		break;
	case EntityType::ITEM_ARMADURA_NV10:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_armadura_nv10.png");
		entity->type = InventityType::ARMADURA_NV10;
		break;
	case EntityType::ITEM_BASURA:
		entity->icon = app->tex->Load("Assets/Textures/Interfaz/Resources/textura_basura.png");
		entity->type = InventityType::BASURA;
		break;
	default:   
		break;
	}

	if (type == EntityType::ITEM_MASCARA0)
	{
		numMasks++;
		mask = true;		
	}
	if (type == EntityType::ITEM_MASCARA1)
	{
		numMasks++;
		mask = true;
	}
	if (type == EntityType::ITEM_MASCARA2)
	{
		numMasks++;
		mask = true;
	}
	if (type == EntityType::ITEM_MASCARA3)
	{
		numMasks++;
		mask = true;
	}

	if (mask == false)
	{
		if ((addInventory))
			AddItem(entity);
	}
		
	
		mask = false;

	return entity;
}

Inventity* InventoryManager::CreateItem(InventityType type, bool addInventory) {

	pugi::xml_document configFile;
	pugi::xml_node entitiesDataNode;
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	entitiesDataNode = configFile.child("config").child("entities_data");
	pugi::xml_node itemConfigurationNode;

	Inventity* inventity = nullptr;


	switch (type) {
		case InventityType::GARRA:
			itemConfigurationNode = entitiesDataNode.child("item_garra");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_GARRA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::OJO:
			itemConfigurationNode = entitiesDataNode.child("item_ojo");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_OJO, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::DIAMANTE:
			itemConfigurationNode = entitiesDataNode.child("item_diamante");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_DIAMANTE, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::VISCERA:
			itemConfigurationNode = entitiesDataNode.child("item_viscera");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_VISCERAS, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::RUBI:
			itemConfigurationNode = entitiesDataNode.child("item_rubi");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_RUBI, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::POLVORA:
			itemConfigurationNode = entitiesDataNode.child("item_polvora");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_POLVORA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::HUESO:
			itemConfigurationNode = entitiesDataNode.child("item_hueso");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_HUESO, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::DIENTE:
			itemConfigurationNode = entitiesDataNode.child("item_diente");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_DIENTE, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::COLA:
			itemConfigurationNode = entitiesDataNode.child("item_cola");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_COLA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::CARBON:
			itemConfigurationNode = entitiesDataNode.child("item_carbon");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_CARBON, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::AMATISTA:
			itemConfigurationNode = entitiesDataNode.child("item_amatista");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_AMATISTA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::PEZ_PEQUENO:
			itemConfigurationNode = entitiesDataNode.child("item_pez_pequeno");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_PEZ_PEQUENO, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::PEZ_MEDIANO:
			itemConfigurationNode = entitiesDataNode.child("item_pez_mediano");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_PEZ_MEDIANO, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::PEZ_GRANDE:
			itemConfigurationNode = entitiesDataNode.child("item_pez_grande");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_PEZ_GRANDE, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::MONEDA:
			itemConfigurationNode = entitiesDataNode.child("item_moneda");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_MONEDA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::SALIVA:
			itemConfigurationNode = entitiesDataNode.child("item_saliva");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_SALIVA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::POCION_VIDA_1:
			itemConfigurationNode = entitiesDataNode.child("item_pocion_vida_1");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_POCION_VIDA_1, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::POCION_VIDA_2:
			itemConfigurationNode = entitiesDataNode.child("item_pocion_vida_2");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_POCION_VIDA_2, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::POCION_VIDA_3:
			itemConfigurationNode = entitiesDataNode.child("item_pocion_vida_3");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_POCION_VIDA_3, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::POCION_VIDA_MAX:
			itemConfigurationNode = entitiesDataNode.child("item_pocion_vida_max");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_POCION_VIDA_MAX, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::POCION_REGENERACION:
			itemConfigurationNode = entitiesDataNode.child("item_pocion_regeneracion");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_POCION_REGENERACION, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string("ahahahu"), addInventory);
			break;
		case InventityType::POCION_DANO:
			itemConfigurationNode = entitiesDataNode.child("item_pocion_dano");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_POCION_DANO, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::POCION_VELOCIDAD:
			itemConfigurationNode = entitiesDataNode.child("item_pocion_velocidad");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_POCION_VELOCIDAD, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::ORBE_MAGICO:
			itemConfigurationNode = entitiesDataNode.child("item_pocion_orbe_magico");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_ORBE_MAGICO, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::ESPADA_NV2:
			itemConfigurationNode = entitiesDataNode.child("item_espada_nv2");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_ESPADA_NV2, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::ESPADA_NV3:
			itemConfigurationNode = entitiesDataNode.child("item_espada_nv3");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_ESPADA_NV3, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::ESPADA_NV4:
			itemConfigurationNode = entitiesDataNode.child("item_espada_nv4");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_ESPADA_NV4, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::ESPADA_NV5:
			itemConfigurationNode = entitiesDataNode.child("item_espada_nv5");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_ESPADA_NV5, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::ESPADA_NV6:
			itemConfigurationNode = entitiesDataNode.child("item_espada_nv6");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_ESPADA_NV6, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::ESPADA_NV7:
			itemConfigurationNode = entitiesDataNode.child("item_espada_nv7");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_ESPADA_NV7, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::ESPADA_NV8:
			itemConfigurationNode = entitiesDataNode.child("item_espada_nv8");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_ESPADA_NV8, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::ESPADA_NV9:
			itemConfigurationNode = entitiesDataNode.child("item_espada_nv9");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_ESPADA_NV9, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::ESPADA_NV10:
			itemConfigurationNode = entitiesDataNode.child("item_espada_nv10");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_ESPADA_NV10, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::ARMADURA_NV2:
			itemConfigurationNode = entitiesDataNode.child("item_armadura_nv2");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_ARMADURA_NV2, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::ARMADURA_NV3:
			itemConfigurationNode = entitiesDataNode.child("item_armadura_nv3");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_ARMADURA_NV3, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::ARMADURA_NV4:
			itemConfigurationNode = entitiesDataNode.child("item_armadura_nv4");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_ARMADURA_NV4, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::ARMADURA_NV5:
			itemConfigurationNode = entitiesDataNode.child("item_armadura_nv5");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_ARMADURA_NV5, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::ARMADURA_NV6:
			itemConfigurationNode = entitiesDataNode.child("item_armadura_nv6");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_ARMADURA_NV6, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::ARMADURA_NV7:
			itemConfigurationNode = entitiesDataNode.child("item_armadura_nv7");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_ARMADURA_NV7, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::ARMADURA_NV8:
			itemConfigurationNode = entitiesDataNode.child("item_armadura_nv8");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_ARMADURA_NV8, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::ARMADURA_NV9:
			itemConfigurationNode = entitiesDataNode.child("item_armadura_nv9");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_ARMADURA_NV9, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::ARMADURA_NV10:
			itemConfigurationNode = entitiesDataNode.child("item_armadura_nv10");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_ARMADURA_NV10, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::ZAFIRO:
			itemConfigurationNode = entitiesDataNode.child("item_zafiro");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_ZAFIRO, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;
		case InventityType::BASURA:
			itemConfigurationNode = entitiesDataNode.child("item_basura");
			inventity = app->inventoryManager->CreateItem(EntityType::ITEM_BASURA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""), addInventory);
			break;

			
	}


	return inventity;


}

bool InventoryManager::IsFull()
{

	// Encontrar el ID m�s alto actualmente asignado


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
		case InventityType::POCION_VIDA_1:
			itemConfigurationNode = entitiesDataNode.child("item_pocion_vida_1");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_POCION_VIDA_1, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::POCION_VIDA_2:
			itemConfigurationNode = entitiesDataNode.child("item_pocion_vida_2");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_POCION_VIDA_2, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::POCION_VIDA_3:
			itemConfigurationNode = entitiesDataNode.child("item_pocion_vida_3");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_POCION_VIDA_3, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::SALIVA:
			itemConfigurationNode = entitiesDataNode.child("item_saliva");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_SALIVA, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::POCION_VIDA_MAX:
			itemConfigurationNode = entitiesDataNode.child("item_pocion_vida_max");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_POCION_VIDA_MAX, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::POCION_REGENERACION:
			itemConfigurationNode = entitiesDataNode.child("item_pocion_regeneracion");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_POCION_REGENERACION, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::POCION_DANO:
			itemConfigurationNode = entitiesDataNode.child("item_pocion_dano");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_POCION_DANO, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::POCION_VELOCIDAD:
			itemConfigurationNode = entitiesDataNode.child("item_pocion_velocidad");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_POCION_VELOCIDAD, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::ORBE_MAGICO:
			itemConfigurationNode = entitiesDataNode.child("item_pocion_orbe_magico");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_ORBE_MAGICO, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::ZAFIRO:
			itemConfigurationNode = entitiesDataNode.child("item_zafiro");
			itemLoaded = app->inventoryManager->CreateItem(EntityType::ITEM_ZAFIRO, itemConfigurationNode.attribute("description").as_string(), itemConfigurationNode.attribute("type").as_string(), itemConfigurationNode.attribute("name").as_string(""));
			break;
		case InventityType::UNKNOWN:
			break;
		default:
			break;
		}
		itemLoaded->quantity = itemNode.attribute("quantity").as_int(0);

	}

	//inventoryMoneyNode.append_attribute("quantity").set_value(monedasObtenidas);

	monedasObtenidas = node.child("inventory").child("money").attribute("quantity").as_int(0);
    swordLevel = node.child("inventory").child("swordLevel").attribute("quantity").as_int();
    armorLevel = node.child("inventory").child("armorLevel").attribute("quantity").as_int();
	storyLevel = node.child("inventory").child("storyLevel").attribute("quantity").as_int();
	primaryMaskInventoryManager = node.child("inventory").child("primaryMask").attribute("quantity").as_int();
	secondaryMaskInventoryManager = node.child("inventory").child("secondaryMask").attribute("quantity").as_int();

	//Mascaras (nivel de rama de cada mascara)
	mask0_levelBranch2 = node.child("inventory").child("Mask0").attribute("Branch2").as_int();
	mask0_levelBranch3 = node.child("inventory").child("Mask0").attribute("Branch3").as_int();
	mask0_levelBranch4 = node.child("inventory").child("Mask0").attribute("Branch4").as_int();
	
	mask1_levelBranch1 = node.child("inventory").child("Mask1").attribute("Branch1").as_int();
	mask1_levelBranch2 = node.child("inventory").child("Mask1").attribute("Branch2").as_int();
	mask1_levelBranch3 = node.child("inventory").child("Mask1").attribute("Branch3").as_int();
	mask1_levelBranch4 = node.child("inventory").child("Mask1").attribute("Branch4").as_int();
	
	mask2_levelBranch1 = node.child("inventory").child("Mask2").attribute("Branch1").as_int();
	mask2_levelBranch2 = node.child("inventory").child("Mask2").attribute("Branch2").as_int();
	mask2_levelBranch3 = node.child("inventory").child("Mask2").attribute("Branch3").as_int();
	mask2_levelBranch4 = node.child("inventory").child("Mask2").attribute("Branch4").as_int();
	
	mask3_levelBranch1 = node.child("inventory").child("Mask3").attribute("Branch1").as_int();
	mask3_levelBranch2 = node.child("inventory").child("Mask3").attribute("Branch2").as_int();
	mask3_levelBranch3 = node.child("inventory").child("Mask3").attribute("Branch3").as_int();
	mask3_levelBranch4 = node.child("inventory").child("Mask3").attribute("Branch4").as_int();


	numMasks = node.child("inventory").child("maskNum").attribute("n").as_int();


	maskZeroPoints = node.child("inventory").child("maskPoints").child("maskPointZero").attribute("points").as_int();
	maskOnePoints = node.child("inventory").child("maskPoints").child("maskPointOne").attribute("points").as_int();
	maskTwoPoints = node.child("inventory").child("maskPoints").child("maskPointTwo").attribute("points").as_int();
	maskThreePoints = node.child("inventory").child("maskPoints").child("maskPointThree").attribute("points").as_int();

	maskZeroXP = node.child("inventory").child("masksXP").child("maskZeroXP").attribute("XP").as_int();
	maskOneXP = node.child("inventory").child("masksXP").child("maskOneXP").attribute("XP").as_int();
	maskTwoXP = node.child("inventory").child("masksXP").child("maskTwoXP").attribute("XP").as_int();
	maskThreeXP = node.child("inventory").child("masksXP").child("maskThreeXP").attribute("XP").as_int();

	XPtoLevelUpZero = node.child("inventory").child("XPtoLevelUps").child("XPtoLevelUpZero").attribute("XP").as_int();
	XPtoLevelUpOne = node.child("inventory").child("XPtoLevelUps").child("XPtoLevelUpOne").attribute("XP").as_int();
	XPtoLevelUpTwo = node.child("inventory").child("XPtoLevelUps").child("XPtoLevelUpTwo").attribute("XP").as_int();
	XPtoLevelUpThree = node.child("inventory").child("XPtoLevelUps").child("XPtoLevelUpThree").attribute("XP").as_int();

	maskZeroLevel = node.child("inventory").child("masksLevel").child("maskLevelZero").attribute("level").as_int();
	maskOneLevel = node.child("inventory").child("masksLevel").child("maskLevelOne").attribute("level").as_int();
	maskTwoLevel = node.child("inventory").child("masksLevel").child("maskLevelTwo").attribute("level").as_int();
	maskThreeLevel = node.child("inventory").child("masksLevel").child("maskLevelThree").attribute("level").as_int();

	app->ascensor->mazmorra = node.child("inventory").child("elevator").attribute("mazmorra").as_int();
	app->ascensor->mazmorraActual = node.child("inventory").child("elevator").attribute("mazmorraActual").as_int();


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

	pugi::xml_node inventorySwordNode = inventoryNode.append_child("swordLevel");
    inventorySwordNode.append_attribute("quantity").set_value(swordLevel);

	pugi::xml_node inventoryArmorNode = inventoryNode.append_child("armorLevel");
    inventoryArmorNode.append_attribute("quantity").set_value(armorLevel);

	pugi::xml_node primaryMaskNode = inventoryNode.append_child("primaryMask");
    primaryMaskNode.append_attribute("quantity").set_value(primaryMaskInventoryManager);

	pugi::xml_node secondaryMaskNode = inventoryNode.append_child("secondaryMask");
    secondaryMaskNode.append_attribute("quantity").set_value(secondaryMaskInventoryManager);

	pugi::xml_node storyLevelNode = inventoryNode.append_child("storyLevel");
	storyLevelNode.append_attribute("quantity").set_value(storyLevel);

	pugi::xml_node mask0Node = inventoryNode.append_child("mask0");
	mask0Node.append_attribute("Branch1").set_value(mask0_levelBranch1);
	mask0Node.append_attribute("Branch2").set_value(mask0_levelBranch2);
	mask0Node.append_attribute("Branch3").set_value(mask0_levelBranch3);
	mask0Node.append_attribute("Branch4").set_value(mask0_levelBranch4);

	pugi::xml_node mask1Node = inventoryNode.append_child("mask1");
	mask1Node.append_attribute("Branch1").set_value(mask1_levelBranch1);
	mask1Node.append_attribute("Branch2").set_value(mask1_levelBranch2);
	mask1Node.append_attribute("Branch3").set_value(mask1_levelBranch3);
	mask1Node.append_attribute("Branch4").set_value(mask1_levelBranch4);

	pugi::xml_node mask2Node = inventoryNode.append_child("mask2");
	mask2Node.append_attribute("Branch1").set_value(mask2_levelBranch1);
	mask2Node.append_attribute("Branch2").set_value(mask2_levelBranch2);
	mask2Node.append_attribute("Branch3").set_value(mask2_levelBranch3);
	mask2Node.append_attribute("Branch4").set_value(mask2_levelBranch4);

	pugi::xml_node mask3Node = inventoryNode.append_child("mask3");
	mask3Node.append_attribute("Branch1").set_value(mask3_levelBranch1);
	mask3Node.append_attribute("Branch2").set_value(mask3_levelBranch2);
	mask3Node.append_attribute("Branch3").set_value(mask3_levelBranch3);
	mask3Node.append_attribute("Branch4").set_value(mask3_levelBranch4);

	pugi::xml_node maskNumNode = inventoryNode.append_child("maskNum");

	maskNumNode.append_attribute("n").set_value(numMasks);
	maskNumNode.append_attribute("info").set_value("Esta variable son las mascaras que tienes desbloqueadas");


	pugi::xml_node maskPointsNode = inventoryNode.append_child("maskPoints");
	pugi::xml_node maskPointNode = maskPointsNode.append_child("maskPointZero");
	maskPointNode.append_attribute("points").set_value(maskZeroPoints);

	maskPointNode = maskPointsNode.append_child("maskPointOne");
	maskPointNode.append_attribute("points").set_value(maskOnePoints);

	maskPointNode = maskPointsNode.append_child("maskPointTwo");
	maskPointNode.append_attribute("points").set_value(maskTwoPoints);

	maskPointNode = maskPointsNode.append_child("maskPointThree");
	maskPointNode.append_attribute("points").set_value(maskThreePoints);


	pugi::xml_node masksXPNode = inventoryNode.append_child("masksXP");
	pugi::xml_node maskXPNode = masksXPNode.append_child("maskZeroXP");
	maskXPNode.append_attribute("XP").set_value(maskZeroXP);

	maskXPNode = masksXPNode.append_child("maskOneXP");
	maskXPNode.append_attribute("XP").set_value(maskOneXP);

	maskXPNode = masksXPNode.append_child("maskTwoXP");
	maskXPNode.append_attribute("XP").set_value(maskTwoXP);

	maskXPNode = masksXPNode.append_child("maskThreeXP");
	maskXPNode.append_attribute("XP").set_value(maskThreeXP);

	pugi::xml_node XPtoLevelUpsNode = inventoryNode.append_child("masksXP");
	pugi::xml_node XPtoLevelUpNode = XPtoLevelUpsNode.append_child("XPtoLevelUpZero");
	XPtoLevelUpNode.append_attribute("XP").set_value(XPtoLevelUpZero);

	XPtoLevelUpNode = XPtoLevelUpsNode.append_child("XPtoLevelUpOne");
	XPtoLevelUpNode.append_attribute("XP").set_value(XPtoLevelUpOne);

	XPtoLevelUpNode = XPtoLevelUpsNode.append_child("XPtoLevelUpTwo");
	XPtoLevelUpNode.append_attribute("XP").set_value(XPtoLevelUpTwo);

	XPtoLevelUpNode = XPtoLevelUpsNode.append_child("XPtoLevelUpThree");
	XPtoLevelUpNode.append_attribute("XP").set_value(XPtoLevelUpThree);

	pugi::xml_node masksLevelNode = inventoryNode.append_child("masksLevel");
	pugi::xml_node maskLevelNode = masksLevelNode.append_child("maskLevelZero");
	maskLevelNode.append_attribute("level").set_value(maskZeroLevel);

	maskLevelNode = masksLevelNode.append_child("maskLevelOne");
	maskLevelNode.append_attribute("XP").set_value(maskOneLevel);

	maskLevelNode = masksLevelNode.append_child("maskLevelTwo");
	maskLevelNode.append_attribute("XP").set_value(maskTwoLevel);

	maskLevelNode = masksLevelNode.append_child("maskLevelThree");
	maskLevelNode.append_attribute("XP").set_value(maskThreeLevel);



	pugi::xml_node elevatorNode = inventoryNode.append_child("elevator");
	elevatorNode.append_attribute("mazmorra").set_value(app->ascensor->mazmorra);
	elevatorNode.append_attribute("mazmorraActual").set_value(app->ascensor->mazmorraActual);


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
	// Reasignar los IDs despu�s de la eliminaci�n
	int newId = 0;
	for (item = inventities.start; item != nullptr; item = item->next)
	{

		item->data->id = newId;
		newId++;
	}
}

void InventoryManager::DestroyItemById(int entityId, bool useStack)
{
	ListItem<Inventity*>* item;

	for (item = inventities.start; item != NULL; item = item->next)
	{

		if (item->data->id == entityId) // Comprueba si el ID coincide
		{
			if (item->data->stackable && item->data->quantity > 1 )
			{
				if (useStack == true)
				{
					item->data->quantity--;
				}
				

				else
				{
					inventities.Del(item);
					delete item->data;
				}
			}
			else
			{
				inventities.Del(item);
				delete item->data; // Libera la memoria de la espada eliminada
			}


			break; // Termina el bucle despu�s de eliminar la espada
		}


	}
	// Reasignar los IDs despu�s de la eliminaci�n
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

void InventoryManager::UsePotionSelected(ListItem<Inventity*>* it)
{
	if (it != NULL)
	{
		Inventity* inventity = it->data;


		DestroyItem(it->data->type, 1);

		switch (it->data->type)
		{
			case InventityType::POCION_VIDA_1:
				if (app->entityManager->GetPlayer()->currentStats.currentHealth <= app->entityManager->GetPlayer()->currentStats.maxHealth + app->entityManager->GetPlayer()->armorPerLevel[armorLevel]) {
					app->entityManager->GetPlayer()->currentStats.currentHealth += 20;
					if (app->entityManager->GetPlayer()->currentStats.currentHealth >= app->entityManager->GetPlayer()->currentStats.maxHealth)
					{
						app->entityManager->GetPlayer()->currentStats.currentHealth = app->entityManager->GetPlayer()->currentStats.maxHealth;
					}
					//printf("uso POCION_VIDA_1. Vida: %f \n", app->entityManager->GetPlayer()->currentStats.currentHealth);
				}
				break;
			case InventityType::POCION_VIDA_2:
				if (app->entityManager->GetPlayer()->currentStats.currentHealth <= app->entityManager->GetPlayer()->currentStats.maxHealth + app->entityManager->GetPlayer()->armorPerLevel[armorLevel]) {
					app->entityManager->GetPlayer()->currentStats.currentHealth += 50;
					if (app->entityManager->GetPlayer()->currentStats.currentHealth >= app->entityManager->GetPlayer()->currentStats.maxHealth)
					{
						app->entityManager->GetPlayer()->currentStats.currentHealth = app->entityManager->GetPlayer()->currentStats.maxHealth;
					}
					//printf("uso POCION_VIDA_2. Vida: %f \n", app->entityManager->GetPlayer()->currentStats.currentHealth);
				}
				break;
			case InventityType::POCION_VIDA_3:
				if (app->entityManager->GetPlayer()->currentStats.currentHealth <= app->entityManager->GetPlayer()->currentStats.maxHealth + app->entityManager->GetPlayer()->armorPerLevel[armorLevel]) {
					app->entityManager->GetPlayer()->currentStats.currentHealth += 150;
					if (app->entityManager->GetPlayer()->currentStats.currentHealth >= app->entityManager->GetPlayer()->currentStats.maxHealth)
					{
						app->entityManager->GetPlayer()->currentStats.currentHealth = app->entityManager->GetPlayer()->currentStats.maxHealth;
					}
					//printf("uso POCION_VIDA_3. Vida: %f \n", app->entityManager->GetPlayer()->currentStats.currentHealth);
				}
				break;
			case InventityType::POCION_VIDA_MAX:
				app->entityManager->GetPlayer()->currentStats.currentHealth = app->entityManager->GetPlayer()->currentStats.maxHealth;
				//printf("uso POCION_VIDA_MAX. Vida: %f \n", app->entityManager->GetPlayer()->currentStats.currentHealth);

				break;
			case InventityType::POCION_REGENERACION:
				app->entityManager->GetPlayer()->regenAmount = 50 / 25; //valor de regeneracion de la pocion dividido por la cantidad de veces que se regenerara durante la duraci�n de la pocion
				app->entityManager->GetPlayer()->regenTimer.Start();
				if (app->entityManager->GetPlayer()->currentStats.currentHealth >= app->entityManager->GetPlayer()->currentStats.maxHealth)
				{
					app->entityManager->GetPlayer()->currentStats.currentHealth = app->entityManager->GetPlayer()->currentStats.maxHealth;
				}
				//printf("uso POCION_REGENERACION. Vida: %f \n", app->entityManager->GetPlayer()->currentStats.currentHealth);
				break;
			case InventityType::POCION_DANO:
				//printf("antes uso POCION_DANO. Da�o: %f \n", app->entityManager->GetPlayer()->currentStats.attackDamage + app->entityManager->GetPlayer()->attackDamagePerLevel[swordLevel]);
				app->entityManager->GetPlayer()->originalDamage = app->entityManager->GetPlayer()->attackDamage + app->entityManager->GetPlayer()->attackDamagePerLevel[swordLevel];
				app->entityManager->GetPlayer()->attackDamagePerLevel[swordLevel] *= 1.2f;
				app->entityManager->GetPlayer()->potiondamageTimer.Start();
				app->entityManager->GetPlayer()->damagePotionActive = true;
				//printf("despues uso POCION_DANO. Da�o: %f \n", app->entityManager->GetPlayer()->attackDamagePerLevel[swordLevel]);
				break;
			case InventityType::POCION_VELOCIDAD:
				//printf("antes uso POCION_VELOCIDAD. Velocidad: %f \n", app->entityManager->GetPlayer()->currentStats.movementSpeed);
				app->entityManager->GetPlayer()->originalSpeed = app->entityManager->GetPlayer()->currentStats.movementSpeed;
				app->entityManager->GetPlayer()->currentStats.movementSpeed *= 1.1f;
				app->entityManager->GetPlayer()->potionspeedTimer.Start();
				app->entityManager->GetPlayer()->speedPotionActive = true;
				//printf("despues uso POCION_VELOCIDAD. Velocidad: %f \n", app->entityManager->GetPlayer()->currentStats.movementSpeed);
				break;
			case InventityType::ORBE_MAGICO:
				//printf("uso ORBE_MAGICO");
				for (auto& mask : app->entityManager->GetPlayer()->maskStats) {
					for (auto& branch : mask.second) {
						if (branch.first == Branches::Rama2) {
							for (auto& level : branch.second) {
								level.second.maskCoolDown = 0;
							}
						}
					}
				}
				break;
			case InventityType::UNKNOWN:
				break;
			default:
				printf("Unknown potion type: %d\n", static_cast<int>(it->data->type));
				break;
		}
	}
}


void InventoryManager::OnMovePointer()
{

	if (app->input->GetButton(RIGHT) == KEY_DOWN && PointerPosition.x < 460) {
		app->audio->StopFx(-1);
		app->audio->PlayFx(change_inventory_mark_fx);

		PointerPosition.x += 106;
		PointerId += 1;
		currentPointerAnimation->Reset();
	}
	if (app->input->GetButton(LEFT) == KEY_DOWN && PointerPosition.x > 258) {
		app->audio->StopFx(-1);
		app->audio->PlayFx(change_inventory_mark_fx);

		PointerPosition.x -= 106;
		PointerId -= 1;
		currentPointerAnimation->Reset();
	}

	if (app->input->GetButton(DOWN) == KEY_DOWN && PointerPosition.y < 436) {
		app->audio->StopFx(-1);
		app->audio->PlayFx(change_inventory_mark_fx);

		PointerPosition.y += 103;
		PointerId += 3;
		currentPointerAnimation->Reset();
	}
	if (app->input->GetButton(UP) == KEY_DOWN && PointerPosition.y > 130 ) {
		app->audio->StopFx(-1);
		app->audio->PlayFx(change_inventory_mark_fx);

		PointerPosition.y -= 103;
		PointerId -= 3;
		currentPointerAnimation->Reset();
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
		else
		{
			DestroyItemById(entity->id, false);
		}

		//Mostrar mensaje hud
		app->hud->AcquiredItemTrigger(entity->icon, entity->name.GetString());
	}

	
	
}

void InventoryManager::AddQuantity(Entity* entity)
{
	InventityType inventity;
	
	switch (entity->type)
	{
	case EntityType::ITEM_GARRA:
		inventity = InventityType::GARRA;
		break;
	case EntityType::ITEM_DIAMANTE:
		inventity = InventityType::DIAMANTE;
		break;
	case EntityType::ITEM_OJO:
		inventity = InventityType::OJO;
		break;
	case EntityType::ITEM_HUESO:
		inventity = InventityType::HUESO;
		break;
	case EntityType::ITEM_CARBON:
		inventity = InventityType::CARBON;
		break;
	case EntityType::ITEM_VISCERAS:
		inventity = InventityType::VISCERA;
		break;
	case EntityType::ITEM_RUBI:
		inventity = InventityType::RUBI;
		break;
	case EntityType::ITEM_DIENTE:
		inventity = InventityType::DIENTE;
		break;
	case EntityType::ITEM_AMATISTA:
		inventity = InventityType::AMATISTA;
		break;
	case EntityType::ITEM_COLA:
		inventity = InventityType::COLA;
		break;
	case EntityType::ITEM_POLVORA:
		inventity = InventityType::POLVORA;
		break;
	case EntityType::ITEM_NOTA:
		inventity = InventityType::NOTA;
		break;
	case EntityType::ITEM_POCION_DANO:
		inventity = InventityType::POCION_DANO;
		break;
	case EntityType::ITEM_POCION_REGENERACION:
		inventity = InventityType::POCION_REGENERACION;
		break;
	case EntityType::ITEM_POCION_VELOCIDAD:
		inventity = InventityType::POCION_VELOCIDAD;
		break;
	case EntityType::ITEM_POCION_VIDA_1:
		inventity = InventityType::POCION_VIDA_1;
		break;
	case EntityType::ITEM_POCION_VIDA_2:
		inventity = InventityType::POCION_VIDA_2;
		break;
	case EntityType::ITEM_POCION_VIDA_3:
		inventity = InventityType::POCION_VIDA_3;
		break;
	case EntityType::ITEM_POCION_VIDA_MAX:
		inventity = InventityType::POCION_VIDA_MAX;
		break;
	case EntityType::ITEM_SALIVA:
		inventity = InventityType::SALIVA;
		break;
	default:
		break;
	}
	for (int i = 0; i < inventities.Count(); i++) {
		if (inventities.At(i)->data->type == inventity) {
			inventities.At(i)->data->quantity += 1;
			/*entity->pbody->body->SetActive(false);*/
			entity->active = false;
			break;
		}

	}
	
	
	
}

bool InventoryManager::Update(float dt)
{

	bool ret = true;

	////printf("\n selecion: %d", seleccionFinalPersonaje);

	//Iterates over the entities and calls the Awake
	/*ListItem<Inventity*>* item;
	int id = NULL;

	for (item = inventities.start; item != NULL && ret == true; item = item->next)
	{
		id = item->data->objectID;

		if (id != -1) {
			idSaved = id;
		}
		
	}*/


	if (mostrar == true)
	{
		OnMovePointer();
		if (selectedd == true)
		{
			if (app->input->GetButton(RIGHT) == KEY_DOWN || app->input->GetButton(LEFT) == KEY_DOWN || app->input->GetButton(DOWN) == KEY_DOWN|| app->input->GetButton(UP) == KEY_DOWN)
			{
				selected = { -1000, -1000 };
				selectedd = false;
			}

		}
		if (app->input->GetButton(SELECT) == KEY_DOWN && !app->scene_pueblo->GetRod()->fishing.rodReady  && !app->scene_pueblo_tutorial->GetRod()->fishing.rodReady ) {
			options = true;
			selected = { PointerPosition.x + 8, PointerPosition.y + 9 };
			selectedId = PointerId;
			UseItemSelected(selectedId);
			selectedd = true;
		}
		if (selectedd == true)
		{
			if (app->input->GetButton(BACK) == KEY_DOWN)
			{
				DestroyItemById(selectedId);
				options = false;
				selected = { -1000, -1000 };
				selectedd = false;
			}
		}



	}
	
	//DESCOMENTAR CUANDO SE HAYA IMPLEMENTADO EL TUTORIAL CON SUS BOOLS
	if(app->tutorialHasFinished && dungeon0Entered && app->questManager->GetQuestLineIndex(1) >= 10)
	{
	 	//storyLevel = app->ascensor->totalMazmorras + 2;
	}
	 else if (app->tutorialHasFinished && dungeon0Entered && dungeon0Exited)
	 {
		if(storyLevel < 3) storyLevel = 3;
	 }

	 else if(app->tutorialHasFinished && dungeon0Entered)
	 {
	 	storyLevel = app->ascensor->totalMazmorras + 1;
	 }
	 else if(app->tutorialHasFinished && !dungeon0Entered)
	 {
	 	storyLevel = 2;
	 }
	 else if(app->tutorialHasStarted == false)
	 {
	 	storyLevel = 1;
	 }
	//DESCOMENTAR CUANDO SE HAYA IMPLEMENTADO EL TUTORIAL CON SUS BOOLS

	//BORRAR LA LINEA DE ABAJO CUANDO SE HAYA IMPLEMENTADO EL TUTORIAL CON SUS BOOLS ARRIBA
	/*storyLevel = app->ascensor->totalMazmorras;*/
	printf("\rstoryLevel %d", storyLevel);
	//BORRAR LA LINEA DE ARRIBA CUANDO SE HAYA IMPLEMENTADO EL TUTORIAL CON SUS BOOLS ARRIBA


	 currentPointerAnimation->Update();

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



		///app->render->DrawTexture(PointerItemText, PointerPosition.x, PointerPosition.y, 0.8, SDL_FLIP_NONE, 0, 0);
		if (app->menu->animating == false && app->menu->animatingExit2 == false && app->menu->menuu == true)
		{
			app->render->DrawTexture(PointerItemText, PointerPosition.x, PointerPosition.y, 0.8, SDL_FLIP_NONE, &currentPointerAnimation->GetCurrentFrame(), 0);
			app->render->DrawTexture(SelectedItemText, selected.x, selected.y, 0.8, SDL_FLIP_NONE, 0, 0);
		}
		

		for (item = inventities.start; item != nullptr; item = item->next)
		{
			pEntity = item->data;
			int rowIndex = item->data->id / maxItemsPerRow; // Calcula el �ndice de la fila
			int columnIndex = item->data->id % maxItemsPerRow; // Calcula el �ndice de la columna
			int horizontalPosition = 320 + columnIndex * 105; // Calcula la posici�n horizontal
			int verticalPosition = 230 + rowIndex * 103; // Calcula la posici�n vertical
			
			horizontalPosition = 270 + columnIndex * 108; // Calcula la posici�n horizontal para pEntity
			verticalPosition = 150 + rowIndex * 104; // Calcula la posici�n vertical para pEntity
			if (app->menu->animating == false && app->menu->animatingExit2 == false && app->menu->menuu == true)
			{
				app->render->DrawTexture(pEntity->icon, horizontalPosition, verticalPosition, 0.8, SDL_FLIP_NONE, 0, 0);
			}

		}
		if (inventities.At(PointerId) != nullptr)
		{
			if (inventities.At(PointerId)->data->quantity > 0)
			{
				if (app->menu->animating == false && app->menu->animatingExit2 == false && app->menu->menuu == true)
				{
					std::string quantityStr = "X  " + std::to_string(inventities.At(PointerId)->data->quantity);
					/*app->render->DrawText(quantityStr.c_str(), 700, 400, 70, 70, 0, 0, 0, 0);	*/
					app->render->DrawTextBound(quantityStr.c_str(), 830, 330, 270, { 52,25,0 });
				}
			}
		}
		

	}
	if (mostrar == true)
	{
		ListItem<Inventity*>* itum;
		for (itum = inventities.start; itum != nullptr; itum = itum->next)
		{
			if (PointerId == itum->data->id)
			{
				if (app->menu->animating == false && app->menu->animatingExit2 == false && app->menu->menuu == true)
				{
					/*app->render->DrawText(itum->data->desc.c_str(), 680, 250, 270, 200, 0, 0, 0, 0, false);*/
					app->render->DrawTextBound(itum->data->desc.c_str(), 670, 430, 270, { 52,25,0 });
					app->render->DrawTextBound(itum->data->name.GetString(), 670, 150, 90, { 52,25,0 }, app->render->titleFont);
					app->render->DrawTexture(itum->data->icon, 680, 200, 1.8, SDL_FLIP_NONE, 0, 0);
				}
			} 

		}
	}
	return ret;
}






