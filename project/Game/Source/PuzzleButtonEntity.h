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

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);


private:

	


public:
	int buttonID = -1;
	bool pressed = false;
	bool playerOnHover = false;

private:

	//Cuerpo de fisicas, para detectar cuando el jugador esta cerca
	//y activar el dialogo en caso de que el jugador asi lo quiera
	PhysBody* pbody = nullptr;
	SDL_Texture* texture = nullptr;

	SDL_Rect notPressedRect{ 98,0, 98, 79 };
	SDL_Rect pressedRect{ 0, 0, 98, 79 };

	int puzzle_button_fx;
		
};

#endif // __PuzzleButtonEntityENTITY_H__