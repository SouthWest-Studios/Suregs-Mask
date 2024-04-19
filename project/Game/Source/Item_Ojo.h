#ifndef __ITEMOJO_H__
#define __ITEMOJO_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Item_Ojo : public Entity
{
public:
	/*Espada() : Entity(EntityType::ESPADA, 0, 100, 0, 0, 0) {}*/

	Item_Ojo(EntityType type, int id, int ataque,int durabilidad, int magia, float peso); // Constructor con parámetros
	virtual ~Item_Ojo();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();


public:

	bool isPicked = false;

	
	EntityType type;
	int ataque = 100;
	int durabilidad; // Durabilidad de la espada
	int magia;
	float peso; // Peso de la espada
	PhysBody* pbody;
	SDL_Texture* texture;
	iPoint position;
	pugi::xml_node config;
private:

	
	const char* texturePath;
	uint texW, texH;	

	//L07 DONE 4: Add a physics to an item
	
};

#endif // __ITEMOJO_H__