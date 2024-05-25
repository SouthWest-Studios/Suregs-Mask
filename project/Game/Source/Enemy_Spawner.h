#ifndef __ENEMY_Spawner_H__
#define __ENEMY_Spawner_H__


#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "Player.h"
#include "Physics.h"

struct SDL_Texture;

class Enemy_Spawner : public Entity
{


public:

	Enemy_Spawner();

	virtual ~Enemy_Spawner();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	

public:
	pugi::xml_node configEntities;
	

private:

	pugi::xml_document configFile;
	SDL_Texture* texture;
	

	Timer contadorSpawnerTimer;
	int contadorSpawnerMS = 5000;
	

public:

	

};



#endif // __ENEMY_Spawner_H__