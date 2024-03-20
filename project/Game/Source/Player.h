#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	// L07 DONE 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);


private:
	void CameraMovement(float dt);
	void GodMode(float dt);
	void PlayerMovement(float dt);

public:

	//L02: DONE 2: Declare player parameters
	float speed = 0.2f;
	SDL_Texture* texture = NULL;
	pugi::xml_node config;
	uint texW, texH;

	//Audio fx
	int pickCoinFxId;


	Animation* currentAnimation = nullptr;


	Animation SPosition;
	SDL_Rect* spritePositions;

	int TSprite;
	int SpriteX;
	int SpriteY;
	int PhotoWeight;

private:
	Animation idleAnim;
	Animation runAnim;

	bool isFacingLeft = false;

	bool godmode = false;

	bool camaralibre = false;

};

#endif // __PLAYER_H__