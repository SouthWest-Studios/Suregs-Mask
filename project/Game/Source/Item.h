#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Item : public Entity
{
public:

	Item();
	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

public:

	bool isPicked = false;


	//Animation Obligatorio
	Animation* currentAnimation = nullptr;
	Animation SPosition;
	SDL_Rect* spritePositions;
	SDL_Rect rect;
	SDL_Texture* texture;
	const char* texturePath;
	int TSprite;
	int SpriteX;
	int SpriteY;
	int Photowidth;

	//Animation personalizar
	Animation testNombre;


private:

	

	
	uint texW, texH;
};

#endif // __ITEM_H__