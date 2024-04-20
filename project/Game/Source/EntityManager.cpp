#include "EntityManager.h"
#include "Player.h"
#include "Enemy_Ols.h"
#include "Enemy_Osiris.h"
#include "Enemy_Shar.h"
#include "Item.h"
#include "App.h"
#include "Cuerno.h"
#include "Textures.h"
#include "Scene_Testing.h"
#include "Player.h"
#include "DialogTriggerEntity.h"
#include "TPEntity.h"
#include "NPC_Vendedor.h"
#include "NPC_Pescador.h"
#include "NPC_Abuelo.h"
#include "NPC_Bruja.h"
#include "NPC_Bully1.h"
#include "NPC_Bully2.h"
#include "NPC_Herrera.h"
#include "NPC_MujerEnamorada.h"
#include "NPC_MujerPreocupada.h"
#include "Item_diamante.h"
#include "MiniGameFishing.h"
#include "Fishing.h"




#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		RELEASE(item->data);
		item = item->prev;
	}

	entities.Clear();
	tpEntities.Clear();



	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr; 

	//L03: DONE 3a: Instantiate entity according to the type and add the new entity to the list of Entities
	switch (type)
	{
	case EntityType::PLAYER:
		entity = new Player();
		break;
	case EntityType::ENEMY_OSIRIS:
		entity = new Enemy_Osiris();
		break;
	case EntityType::ENEMY_OLS:
		entity = new Enemy_Ols();
		break;
	case EntityType::ENEMY_SHAR:
		entity = new Enemy_Shar();
		break;
	case EntityType::ITEM_CUERNO:
		entity = new Cuerno(type, 1, 100, 300, 5, 2);
		break;
	case EntityType::ITEM_DIAMANTE:
		entity = new Item_Diamante(type, 100, 100, 300, 5, 2);
		break;
	case EntityType::ITEM:
		entity = new Item();
		break;
	case EntityType::DIALOG_TRIGGER:
		entity = new DialogTrigger();
		break;
	case EntityType::TP_ENTITY:
		entity = new TPEntity();
		tpEntities.Add((TPEntity*)entity);
		break;

	case EntityType::NPC_VENDEDOR:
		entity = new NPCVendedor();
		break;
	case EntityType::NPC_PESCADOR:
		entity = new NPCPescador();
		break;
	case EntityType::NPC_BRUJA:
		entity = new NPCBruja();
		break;
	case EntityType::NPC_HERRERA:
		entity = new NPCHerrera();
		break;
	case EntityType::NPC_ABUELO:
		entity = new NPCAbuelo();
		break;
	case EntityType::NPC_MUJER_PREOCUPADA:
		entity = new NPCMujerPreocupada();
		break;
	case EntityType::NPC_MUJER_ENAMORADA:
		entity = new NPCMujerEnamorada();
		break;
	case EntityType::NPC_BULLY_1:
		entity = new NPCBully1();
		break;
	case EntityType::NPC_BULLY_2:
		entity = new NPCBully2();
		break;
	case EntityType::ROD:
		entity = new MiniGameFishing();
		break;

	default:
		break;
	}

	entities.Add(entity);
	entity->Awake();


	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.Del(item);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

void EntityManager::LinkTPEntities()
{
	ListItem<TPEntity*>* itemMajor;
	ListItem<TPEntity*>* itemMinor;

	for (itemMajor = tpEntities.start; itemMajor != NULL; itemMajor = itemMajor->next)
	{
		

		for (itemMinor = tpEntities.start; itemMinor != NULL; itemMinor = itemMinor->next) {
			//Se busca del in al out, es decir (0,1 - 2,3 - 4,5)
			if ((itemMajor->data->tpID == 0 || itemMajor->data->tpID % 2 == 0) && itemMajor->data->tpID+1 == itemMinor->data->tpID) {
				itemMajor->data->targetPosition = itemMinor->data->position;
				itemMinor->data->targetPosition = itemMajor->data->position;

				itemMajor->data->Start();
				itemMinor->data->Start();
				break;
			}
		}
	}
}


void EntityManager::SetPlayer(Player* player)
{
	actualPlayer = player;
}

Player* EntityManager::GetPlayer()
{
	return actualPlayer;
}

MiniGameFishing* EntityManager::GetRod()
{
	return fishing;
}

std::vector<Entity*> EntityManager::GetEnemies() {
    std::vector<Entity*> enemies;
    for (ListItem<Entity*>* item = entities.start; item != NULL; item = item->next) {
        Entity* entity = item->data;
        if (entity->type == EntityType::ENEMY_OSIRIS || entity->type == EntityType::ENEMY_OLS || entity->type == EntityType::ENEMY_SHAR) {
            enemies.push_back(entity);
        }
    }
    return enemies;
}


bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Update(dt);
	}

	return ret;
}

bool EntityManager::PostUpdate()
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->PostUpdate();
	}

	//app->map->UpdateFrontEntities();

	return ret;
}