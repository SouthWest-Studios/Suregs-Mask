#include "EntityManager.h"
#include "Player.h"
#include "Enemy_Ols.h"
#include "Enemy_Osiris.h"
#include "Enemy_Shar.h"
#include "Enemy_Muur.h"
#include "Enemy_Boorok.h"
#include "Item.h"
#include "App.h"
#include "Item_Garra.h"
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
#include "Item_Ojo.h"
#include "Item_Hueso.h"
#include "Item_Carbon.h"
#include "Item_Viscera.h"
#include "Item_Rubi.h"
#include "Item_Diente.h"
#include "Item_Amatista.h"
#include "Item_Cola.h"
#include "Item_Polvora.h"
#include "Item_Nota.h"
#include "Boss_Inuit.h"
#include "Enemy_Khurt.h"

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

	objectsToDraw.clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type, int id)
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
	case EntityType::ENEMY_MUUR:
		entity = new Enemy_Muur();
		break;
	case EntityType::ENEMY_KHURT:
		entity = new Enemy_Khurt();
		break;
	case EntityType::ENEMY_BOOROK:
		entity = new Enemy_Boorok();
		break;


	case EntityType::ITEM_GARRA:
		entity = new Item_Garra(type, 1, 100, 300, 5, 2);
		break;
	case EntityType::ITEM_DIAMANTE:
		entity = new Item_Diamante(type, 100, 100, 300, 5, 2);
		break;
	case EntityType::ITEM_OJO:
		entity = new Item_Ojo(type, 100, 100, 300, 5, 2);
		break;
	case EntityType::ITEM_HUESO:
		entity = new Item_Hueso(type, 100, 100, 300, 5, 2);
		break;
	case EntityType::ITEM_CARBON:
		entity = new Item_Carbon(type, 100, 100, 300, 5, 2);
		break;
	case EntityType::ITEM_VISCERAS:
		entity = new Item_Viscera(type, 100, 100, 300, 5, 2);
		break;
	case EntityType::ITEM_RUBI:
		entity = new Item_Rubi(type, 100, 100, 300, 5, 2);
		break;
	case EntityType::ITEM_DIENTE:
		entity = new Item_Diente(type, 100, 100, 300, 5, 2);
		break;
	case EntityType::ITEM_AMATISTA:
		entity = new Item_Amatista(type, 100, 100, 300, 5, 2);
		break;
	case EntityType::ITEM_COLA:
		entity = new Item_Cola(type, 100, 100, 300, 5, 2);
		break;
	case EntityType::ITEM_POLVORA:
		entity = new Item_Polvora(type, 100, 100, 300, 5, 2);
		break;
	case EntityType::ITEM_NOTA:
		entity = new Item_Nota(type, id, 100, 300, 5, 2);
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
	case EntityType::BOSS_INUIT:
		entity = new Boss_Inuit();
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
		if (item->data == entity)
		{
			//if (item->data->type == EntityType::ENEMY_OSIRIS || item->data->type == EntityType::ENEMY_OLS || item->data->type == EntityType::ENEMY_SHAR)
			//{
			//	garra = (Item_Garra*)app->entityManager->CreateEntity(EntityType::ITEM_GARRA);
			//	/*garra->config = configNode.child("entities_data").child("item_garra");*/
			//	garra->position = iPoint(entity->position.x,entity->position.y);
			//	garra->Start();
			//}
		}
		
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
        if (entity->type == EntityType::ENEMY_OSIRIS || entity->type == EntityType::ENEMY_OLS || entity->type == EntityType::ENEMY_SHAR || entity->type == EntityType::ENEMY_KHURT) {
            enemies.push_back(entity);
        }
    }
    return enemies;
}

bool EntityManager::PreUpdate()
{
	//Ejemplo a�adir sprite en los Start():	app->render->objectsToDraw.push_back({ textura, posicion X, posicion Y, punto de anclaje en Y = (posY + num), ancho, largo});

	for (DrawableObject& obj : objectsToDraw)
	{
		if (obj.texture == app->entityManager->GetPlayer()->texture)
		{
			if (app->entityManager->GetPlayer()->isFacingLeft) {
			obj.x = app->entityManager->GetPlayer()->position.x - 100;
			}
			else
			{
				obj.x = app->entityManager->GetPlayer()->position.x - 45;
			}
			obj.y = app->entityManager->GetPlayer()->position.y - 100;
			obj.anchorY = app->entityManager->GetPlayer()->position.y + 218; //Seg�n el sprite, a�adir el valor que corresponda -> !0
			if (app->entityManager->GetPlayer()->currentAnimation != nullptr) {
				obj.currentFrame = app->entityManager->GetPlayer()->currentAnimation->GetCurrentFrame();
			}
			obj.isFacingLeft = app->entityManager->GetPlayer()->isFacingLeft;
			break;
		}
	}

	std::sort(objectsToDraw.begin(), objectsToDraw.end(),
		[](const DrawableObject& a, const DrawableObject& b)
		{
			return a.anchorY < b.anchorY;
		});

	return true;
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

	for (const DrawableObject& obj : objectsToDraw)
	{
		// Verifica si la posici�n del objeto est?dentro de los l�mites de la c�mara
		if (obj.x + obj.width >= -app->render->camera.x && obj.x <= -app->render->camera.x + app->render->camera.w &&
			obj.y + obj.height >= -app->render->camera.y && obj.y <= -app->render->camera.y + app->render->camera.h){
			if (obj.isFacingLeft) {
				app->render->DrawTexture(obj.texture, obj.x, obj.y, 0.5f, SDL_FLIP_NONE, &obj.currentFrame);
			}
			else if (!obj.isFacingLeft) {
				app->render->DrawTexture(obj.texture, obj.x, obj.y, 0.5f, SDL_FLIP_HORIZONTAL, &obj.currentFrame);
			}
		
			if (app->entityManager->GetPlayer() != nullptr && obj.texture != app->entityManager->GetPlayer()->texture) {
				app->render->DrawTexture(obj.texture, obj.x, obj.y);
			}
		}

	}
	//app->map->UpdateFrontEntities();

	return ret;
}