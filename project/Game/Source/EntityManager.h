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
#include "Cofre.h"
#include "Elevator.h"

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

struct TPDoor {
	int sceneLevel;
	int tpID;

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

	bool PostLateUpdate();

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
	std::vector<Entity*> GetEnemiesOls();

	Cofre* EntityManager::GetCofre();

	Elevator* EntityManager::GetAscensor();

	void UpdateEnemyActivation();
	void UpdateRoomActivation();


	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

	void setVacioValue(int& weigh, int& height, float& size, float maxSize, float minSize);



public:

	List<Entity*> entities;
	List<TPEntity*> tpEntities;

	std::vector<DrawableObject> objectsToDraw;

	std::vector<TPDoor*> openDoors;

	bool playerVacio = false;
	bool goAnimation = false;

	float vacioCount = 0;
	bool vacioControl = false;





private:

	//pugi::xml_node configNode;
	Item_Garra* garra = nullptr;
	Player* actualPlayer = nullptr;
	Cofre* cofre = nullptr;
	Elevator* ascensor = nullptr;
	MiniGameFishing* fishing = new MiniGameFishing;

	//pugi::xml_node config;
	
	Animation* currentAnimation = nullptr;
	Animation SPosition;
	Animation eyeAnimation;
	Animation eyeIdle;
	Animation eyeEndIdle;
	SDL_Texture* texture = NULL;
	SDL_Rect* spritePositions = nullptr;

};

#endif // __ENTITYMANAGER_H__
