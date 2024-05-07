#ifndef __POCIONDANO_H__
#define __POCIONDANO_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "SString.h"

struct SDL_Texture;

class Item_Pocion_Dano : public Entity
{
public:
	/*Espada() : Entity(EntityType::ESPADA, 0, 100, 0, 0, 0) {}*/

	Item_Pocion_Dano(EntityType type, int id, int ataque,int durabilidad, int magia, float peso); // Constructor con parámetros
	virtual ~Item_Pocion_Dano();

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
	SDL_Texture* texture = NULL;
	
	iPoint position;
	pugi::xml_node config;
private:

	
	const char* texturePath;
	uint texW, texH;

	//L07 DONE 4: Add a physics to an item
	
};

#endif // __Cuerno_H__