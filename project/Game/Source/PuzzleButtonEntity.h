#ifndef __PuzzleButtonEntity_H__
#define __PuzzleButtonEntity_H__

#include <string>
#include "Entity.h"
#include "Dialog.h"
#include "List.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class PuzzleButtonEntity : public Entity
{
public:

	PuzzleButtonEntity();
	virtual ~PuzzleButtonEntity();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);


private:

	


public:
	int buttonID = -1;
	bool pressed = false;

private:

	//Cuerpo de fisicas, para detectar cuando el jugador esta cerca
	//y activar el dialogo en caso de que el jugador asi lo quiera
	PhysBody* pbody;

};

#endif // __PuzzleButtonEntityENTITY_H__