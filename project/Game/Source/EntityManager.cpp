#include "EntityManager.h"
#include "Player.h"
#include "Enemy_Ols.h"
#include "Enemy_Osiris.h"
#include "Enemy_Shar.h"

#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene_Testing.h"
#include "Player.h"


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
		item = item->prev;
	}

	entities.Clear();

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
	case EntityType::ITEM:
		entity = new Item();
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


void EntityManager::SetPlayer(Player* player)
{
	actualPlayer = player;
}

Player* EntityManager::GetPlayer()
{
	return actualPlayer;
}

void EntityManager::SetOsiris(Enemy_Osiris* osiris)
{
	actualOsiris = osiris;
}

Enemy_Osiris* EntityManager::GetOsiris()
{
	return actualOsiris;
}

void EntityManager::SetOls(Enemy_Ols* ols)
{
	actualOls = ols;
}

Enemy_Ols* EntityManager::GetOls()
{
	return actualOls;
}

void EntityManager::SetShar(Enemy_Shar* shar)
{
	actualShar = shar;
}

Enemy_Shar* EntityManager::GetShar()
{
	return actualShar;
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