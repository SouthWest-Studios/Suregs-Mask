#include "EntityManager.h"
#include "Player.h"
#include "Enemy_Ols.h"
#include "Enemy_Osiris.h"
#include "Enemy_Osiris_Variation.h"
#include "Enemy_Shar.h"
#include "Enemy_Muur.h"
#include "Enemy_Boorok.h"
#include "Enemy_Khurt.h"
#include "Enemy_Khurt_Variation.h"
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
#include "NPC_Enamorados.h"
#include "NPC_Abuelito.h"
#include "NPC_Abuelitas.h"
#include "NPC_Guardias.h"
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
#include "Item_Saliva.h"
#include "Boss_Inuit.h"
#include "Boss_Musri.h"
#include "Boss_Surma.h"
#include "Cofre.h"
#include "Item_Pocion_Dano.h"
#include "Item_Pocion_Regeneracion.h"
#include "Item_Pocion_Velocidad.h"
#include "Item_Pocion_Vida_1.h"
#include "Item_Pocion_Vida_2.h"
#include "Item_Pocion_Vida_3.h"
#include "Item_Pocion_Vida_Max.h"
#include "Item_Zafiro.h" 
#include "Elevator.h"
#include "ModuleFadeToBlack.h"


#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager(App* app, bool start_enabled) : Module(app, start_enabled)
{
	name = ("entitymanager");
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
		
		//delete item->data;
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
	case EntityType::ENEMY_OSIRIS_VARIATION:
		entity = new Enemy_Osiris_Variation();
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
	case EntityType::ENEMY_KHURT_VARIATION:
		entity = new Enemy_Khurt_Variation();
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
		entity->id = id;
		break;
	case EntityType::ITEM_POCION_DANO:
		entity = new Item_Pocion_Dano(type, id, 100, 300, 5, 2);
		break;
	case EntityType::ITEM_POCION_REGENERACION:
		entity = new Item_Pocion_Regeneracion(type, id, 100, 300, 5, 2);
		break;
	case EntityType::ITEM_POCION_VELOCIDAD:
		entity = new Item_Pocion_Velocidad(type, id, 100, 300, 5, 2);
		break;
	case EntityType::ITEM_POCION_VIDA_1:
		entity = new Item_Pocion_Vida_1(type, id, 100, 300, 5, 2);
		break;
	case EntityType::ITEM_POCION_VIDA_2:
		entity = new Item_Pocion_Vida_2(type, id, 100, 300, 5, 2);
		break;
	case EntityType::ITEM_POCION_VIDA_3:
		entity = new Item_Pocion_Vida_3(type, id, 100, 300, 5, 2);
		break;
	case EntityType::ITEM_POCION_VIDA_MAX:
		entity = new Item_Pocion_Vida_Max(type, id, 100, 300, 5, 2);
		break;
	case EntityType::ITEM_SALIVA:
		entity = new Item_Saliva(type, id, 100, 300, 5, 2);
		break;
	case EntityType::ITEM_ZAFIRO:
		entity = new Item_Zafiro(type, 100, 100, 300, 5, 2);
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

	case EntityType::PUZZLE_BUTTON:
		entity = new PuzzleButtonEntity();
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
	case EntityType::NPC_ENAMORADOS:
		entity = new NPCEnamorados();
		break;
	case EntityType::NPC_ABUELITO:
		entity = new NPCAbuelito();
		break;
	case EntityType::NPC_ABUELITAS:
		entity = new NPCAbuelitas();
		break;
	case EntityType::NPC_GUARDIAS:
		entity = new NPCGuardias();
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

	case EntityType::BOSS_MUSRI:
		entity = new Boss_Musri();
		break;
	case EntityType::BOSS_SURMA:
		entity = new Boss_Surma();
		break;
	case EntityType::ROD:
		entity = new MiniGameFishing();
		break;
	case EntityType::COFRE:
		entity = new Cofre();
		break;
	case EntityType::ASCENSOR:
		entity = new Elevator();
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
        if (entity->type == EntityType::ENEMY_OSIRIS || entity->type == EntityType::ENEMY_OSIRIS_VARIATION || entity->type == EntityType::ENEMY_OLS || entity->type == EntityType::ENEMY_SHAR || entity->type == EntityType::ENEMY_KHURT) {
            enemies.push_back(entity);
        }
    }
    return enemies;
}

std::vector<Entity*> EntityManager::GetEnemiesOsiris()
{
	std::vector<Entity*> enemiesOsiris;

	for (Entity* enemy : GetEnemies())
	{
		if (enemy->type == EntityType::ENEMY_OSIRIS)
		{
			enemiesOsiris.push_back(enemy);
		}
	}

	return enemiesOsiris;
}

std::vector<Entity*> EntityManager::GetEnemiesOls()
{
	std::vector<Entity*> enemiesOls;

	for (Entity* enemy : GetEnemies())
	{
		if (enemy->type == EntityType::ENEMY_OLS)
		{
			enemiesOls.push_back(enemy);
		}
	}

	return enemiesOls;
}

Cofre* EntityManager::GetCofre()
{
	return cofre;
}

Elevator* EntityManager::GetAscensor()
{
	return ascensor;
}

bool EntityManager::PreUpdate()
{
	UpdateEnemyActivation();
	UpdateRoomActivation();
	//Ejemplo a�adir sprite en los Start():	app->render->objectsToDraw.push_back({ textura, posicion X, posicion Y, punto de anclaje en Y = (posY + num), ancho, largo});

	for (DrawableObject& obj : objectsToDraw)
	{
		if (obj.texture == app->entityManager->GetPlayer()->texture)
		{
			if (app->entityManager->GetPlayer()->isFacingLeft) {
			obj.x = app->entityManager->GetPlayer()->position.x - 70;
			}
			else
			{
				obj.x = app->entityManager->GetPlayer()->position.x - 80;
			}
			obj.y = app->entityManager->GetPlayer()->position.y - 100;
			obj.anchorY = app->entityManager->GetPlayer()->position.y + 218; //Seg�n el sprite, a�adir el valor que corresponda -> !0
			if (app->entityManager->GetPlayer()->currentAnimation != nullptr) {
				obj.currentFrame = app->entityManager->GetPlayer()->currentAnimation->GetCurrentFrame();
			}
			obj.isFacingLeft = app->entityManager->GetPlayer()->isFacingLeft;
			obj.isDynamic = true;
			break;
		}
	}

	for (Entity* entity : app->entityManager->GetEnemiesOsiris())
	{
		Enemy_Osiris* enemy = static_cast<Enemy_Osiris*>(entity);
		// Rest of the code...

		for (DrawableObject& obj : objectsToDraw)
		{
			if (obj.texture == enemy->texture)
			{
				if (enemy->isFacingLeft) {
					obj.x = enemy->position.x - 25;
					obj.y = enemy->position.y - 65;
				}
				else
				{
					obj.x = enemy->position.x - 40;
					obj.y = enemy->position.y - 65;
				}
				obj.anchorY = enemy->position.y + 200; // Según el sprite, añadir el valor que corresponda -> !0
				if (enemy->currentAnimation != nullptr) {
					obj.currentFrame = enemy->currentAnimation->GetCurrentFrame();
				}
				obj.isFacingLeft = enemy->isFacingLeft;
				obj.isDynamic = true;
				break;
			}
		}
	}

	for (Entity* entity : app->entityManager->GetEnemiesOls())
	{
		Enemy_Ols* enemy = static_cast<Enemy_Ols*>(entity);
		// Rest of the code...

		for (DrawableObject& obj : objectsToDraw)
		{
			if (obj.texture == enemy->texture)
			{
				if (enemy->isFacingLeft) {
					obj.x = enemy->position.x - 130;
					obj.y = enemy->position.y - 80;
				}
				else
				{
					obj.x = enemy->position.x - 130;
					obj.y = enemy->position.y - 80;
				}
				obj.anchorY = enemy->position.y + 200; // Según el sprite, añadir el valor que corresponda -> !0
				if (enemy->currentAnimation != nullptr) {
					obj.currentFrame = enemy->currentAnimation->GetCurrentFrame();
				}
				obj.isFacingLeft = enemy->isFacingLeft;
				obj.isDynamic = true;
				break;
			}
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

		if (pEntity->active == false || pEntity->isActive == false) continue;
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

			// PLAYER
			if(obj.texture != nullptr){
				if (obj.texture == GetPlayer()->texture && obj.isFacingLeft) {
					app->render->DrawTexture(obj.texture, obj.x, obj.y, 0.5f, SDL_FLIP_NONE, &obj.currentFrame);
				}
				else if (obj.texture == GetPlayer()->texture && !obj.isFacingLeft) {
					app->render->DrawTexture(obj.texture, obj.x, obj.y, 0.5f, SDL_FLIP_HORIZONTAL, &obj.currentFrame);
				}
				else if (obj.texture != GetPlayer()->texture && obj.isFacingLeft && obj.isDynamic) {
					app->render->DrawTexture(obj.texture, obj.x, obj.y, SDL_FLIP_HORIZONTAL, &obj.currentFrame);
				}
				else if (obj.texture != GetPlayer()->texture && !obj.isFacingLeft && obj.isDynamic) {
					app->render->DrawTexture(obj.texture, obj.x, obj.y, SDL_FLIP_NONE, &obj.currentFrame);
				}

				if (app->entityManager->GetPlayer() != nullptr && obj.texture != app->entityManager->GetPlayer()->texture && !obj.isDynamic) {
					app->render->DrawTexture(obj.texture, obj.x, obj.y, 0.5f, SDL_FLIP_NONE, &obj.currentFrame);
				}
			}

		}

	}
	//app->map->UpdateFrontEntities();

	return ret;
}

void EntityManager::UpdateEnemyActivation() {
	// Obtén la sala actual del jugador
	MapObject* currentRoom = GetPlayer()->GetCurrentRoom();
	//if(currentRoom != nullptr)	printf("current room: %d\n", currentRoom->id); //TESTING

	// Obtén todos los enemigos
	std::vector<Entity*> enemies = GetEnemies();

	// Recorre todos los enemigos
	for (Entity* enemy : enemies) {
		// Asegúrate de que el enemigo tiene una sala asignada
		if (enemy->room != nullptr) {
			// Si el enemigo está en la misma sala que el jugador, actívalo
			if (enemy->room == currentRoom) {
				enemy->isActive = true;
				//printf("Enemy activated\n");
			}
			// Si no, desactívalo
			else {
				enemy->isActive = false;
				enemy->pbodyFoot->body->SetLinearVelocity(b2Vec2(0, 0));
				if(enemy->type == EntityType::ENEMY_OLS)
				{
					Enemy_Ols* olsEnemy = static_cast<Enemy_Ols*>(enemy);
					olsEnemy->DestroyProjectile();
				}
				//printf("Enemy deactivated\n");
			}
		}
	}
}

void EntityManager::UpdateRoomActivation(){
	MapObject* currentRoom = GetPlayer()->GetCurrentRoom();
	std::vector<Entity*> enemies = GetEnemies();

	bool enemyInRoom = false;


    for (Entity* enemy : enemies) {
        // Asegúrate de que el enemigo tiene una sala asignada
        if (enemy->room != nullptr) {
            // Si el enemigo está en la misma sala que el jugador, marca que hay un enemigo en la sala
            if (enemy->room == currentRoom) {
                enemyInRoom = true;
                break;
            }
        }
    }

    // Si no hay enemigos en la sala, activa los tps de la sala
    if (!enemyInRoom) {
        for (ListItem<TPEntity*>* item = tpEntities.start; item != nullptr; item = item->next) {
            if (item->data->room == currentRoom && !item->data->isOpened) {
                item->data->isOpened = true;
				TPDoor* tpdoor = new TPDoor{ item->data->sceneLevel, item->data->tpID };
				openDoors.push_back(tpdoor);
                //printf("TP activated\n");
            }
        }
    }





}

bool EntityManager::LoadState(pugi::xml_node node) {


	//Bucle limpiando el openDoors
	for (int i = 0; i < openDoors.size(); i++) {
		delete openDoors.at(i);
	}



	//Inventity* inventoryItem = app->inventoryManager->CreateItem(, 0, 0, 0, 0, 0, 0);
	for (pugi::xml_node itemNode = node.child("entity_manager").child("doors").child("door"); itemNode; itemNode = itemNode.next_sibling("door")) {
		TPDoor* tpdoor = new TPDoor{ itemNode.attribute("sceneLevel").as_int(), itemNode.attribute("tpID").as_int() };
		//openDoors.push_back(tpdoor);
	}
	return true;
}

bool EntityManager::SaveState(pugi::xml_node node)
{
	pugi::xml_node entityManagerNode = node.append_child("entity_manager");
	pugi::xml_node doorsNode = entityManagerNode.append_child("doors");

	for (int i = 0; i < openDoors.size(); i++) {
		pugi::xml_node doorNode = doorsNode.append_child("door");
		doorNode.append_attribute("sceneLevel").set_value(openDoors.at(i)->sceneLevel);
		doorNode.append_attribute("tpID").set_value(openDoors.at(i)->tpID);
	}

	return true;
}
