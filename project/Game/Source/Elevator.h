#ifndef __ELEVATOR_H__
#define __ELEVATOR_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "SString.h"

struct SDL_Texture;

class Elevator : public Entity
{
public:
	/*Espada() : Entity(EntityType::ESPADA, 0, 100, 0, 0, 0) {}*/

	Elevator(); 
	virtual ~Elevator();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	bool CleanUp();


public:
	pugi::xml_document configFile;
	pugi::xml_node configNode;
	
	EntityType type;

	PhysBody* pbody;
	PhysBody* pbody2;
	SDL_Texture* texture = NULL;
	SDL_Texture* Menutexture = NULL;
	SDL_Texture* listTexture = NULL;
	SDL_Texture* PointerTexture = NULL;
	
	iPoint position;
	iPoint PointerPosition = { 500, 230 };

private:
	bool abierto = false;
	
	int chest_fx;

	const char* texturePath;
	const char* openTexturePath;
	uint texW, texH;

	//L07 DONE 4: Add a physics to an item
	
};

#endif // __Cuerno_H__