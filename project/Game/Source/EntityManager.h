#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy_Osiris.h"
#include "Enemy_Ols.h"
#include "Enemy_Shar.h"
#include "List.h"

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


	void SetPlayer(Player* player);
	Player* GetPlayer();

	void SetOsiris(Enemy_Osiris* osiris);
	Enemy_Osiris* GetOsiris();

	void SetOls(Enemy_Ols* ols);
	Enemy_Ols* GetOls();

	void SetShar(Enemy_Shar* shar);
	Enemy_Shar* GetShar();

public:

	List<Entity*> entities;

private:
	Player* actualPlayer;
	Enemy_Osiris* actualOsiris;
	Enemy_Ols* actualOls;
	Enemy_Shar* actualShar;

};

#endif // __ENTITYMANAGER_H__
