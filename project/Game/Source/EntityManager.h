#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy_Osiris.h"
#include "Item_garra.h"
#include "Enemy_Shar.h"
#include "TPEntity.h"
#include "List.h"
#include "MiniGameFishing.h"

struct DrawableObject
{
	SDL_Texture* texture;
	int x, y;
	int anchorY;
	int width, height;
	SDL_Rect currentFrame;
	bool isFacingLeft;
	bool isDynamic;
};

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
	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type, int id = 0);

	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

	void LinkTPEntities();


	void SetPlayer(Player* player);
	Player* GetPlayer();

	MiniGameFishing* GetRod();
	std::vector<Entity*> GetEnemies();

	std::vector<Entity*> GetEnemiesOsiris();
	

public:

	List<Entity*> entities;
	List<TPEntity*> tpEntities;

	std::vector<DrawableObject> objectsToDraw;


private:
	pugi::xml_node configNode;
	Item_Garra* garra;
	Player* actualPlayer;
	MiniGameFishing* fishing = new MiniGameFishing;

};

#endif // __ENTITYMANAGER_H__
