#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "Player.h"
#include "Osiris.h"
#include "Ols.h"
#include "Shar.h"
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

	void SetOsiris(Osiris* osiris);
	Osiris* GetOsiris();

	void SetOls(Ols* ols);
	Ols* GetOls();

	void SetShar(Shar* shar);
	Shar* GetShar();

public:

	List<Entity*> entities;

private:
	Player* actualPlayer;
	Osiris* actualOsiris;
	Ols* actualOls;
	Shar* actualShar;

};

#endif // __ENTITYMANAGER_H__
