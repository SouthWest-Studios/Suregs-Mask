#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy_Osiris.h"
#include "Enemy_Ols.h"
#include "Enemy_Shar.h"
#include "TPEntity.h"
#include "List.h"
#include "MiniGameFishing.h"

class EntityManager : public Module
{
public:

	EntityManager(App* app, bool start_enabled = true);

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type);

	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

	void LinkTPEntities();


	void SetPlayer(Player* player);
	Player* GetPlayer();

	MiniGameFishing* GetRod();
	

public:

	List<Entity*> entities;
	List<TPEntity*> tpEntities;

private:
	Player* actualPlayer;
	MiniGameFishing* fishing = new MiniGameFishing;

};

#endif // __ENTITYMANAGER_H__
