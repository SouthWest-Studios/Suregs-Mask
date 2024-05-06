#ifndef __COFRE_H__
#define __COFRE_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "SString.h"

struct SDL_Texture;

class Cofre : public Entity
{
public:
	/*Espada() : Entity(EntityType::ESPADA, 0, 100, 0, 0, 0) {}*/

	Cofre(); 
	virtual ~Cofre();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();


public:

	
	EntityType type;

	PhysBody* pbody;
	SDL_Texture* texture = NULL;
	SDL_Texture* openTexture = NULL;
	
	iPoint position;
	pugi::xml_node config;
private:

	
	const char* texturePath;
	const char* openTexturePath;
	uint texW, texH;

	//L07 DONE 4: Add a physics to an item
	
};

#endif // __Cuerno_H__