#ifndef __TPEntity_H__
#define __TPEntity_H__

#include <string>
#include "Entity.h"
#include "Dialog.h"
#include "List.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class TPEntity : public Entity
{
public:

	TPEntity();
	virtual ~TPEntity();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);


private:

	


public:
	int tpID;

private:

	//Cuerpo de fisicas, para detectar cuando el jugador esta cerca
	//y activar el dialogo en caso de que el jugador asi lo quiera
	PhysBody* pbody;

};

#endif // __TPEntityENTITY_H__