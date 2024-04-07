#ifndef __OLS_H__
#define __OLS_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "Player.h"

struct SDL_Texture;


struct Branch_Ols {
	enum EntityState const next_state;

};

class Ols : public Entity
{


public:

	Ols();

	virtual ~Ols();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void DoNothing(float dt);
	void Chase(float dt);
	void Attack(float dt);
	void Die(float dt);

	// L07 DONE 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);

	void SetPlayer(Player* player);

public:

	//L02: DONE 2: Declare player parameters
	SDL_Texture* texture = NULL;
	pugi::xml_node config;
	uint texW, texH;
	//Estadisticas
	float speed;
	float health;
	float maxHealth;

	Animation* currentAnimation = nullptr;
	EntityState state;


	Animation SPosition;
	SDL_Rect* spritePositions;

	int TSprite;
	int SpriteX;
	int SpriteY;
	int Photowidth;

	int currentPosX;

	PathFinding* path;
	Player* player;

private:
	Animation idleAnim;
	Animation runAnim;
	Animation attackAnim;
	Animation dieAnim;

	bool isFacingLeft = false;

public:

	Branch_Ols transitionTable[static_cast<int>(EntityState::STATE_COUNT)][static_cast<int>(EntityState::STATE_COUNT)] = {
		// isMoving               isAttacking						 isDead                else
		{ {EntityState::RUNNING}, {EntityState::ATTACKING}, {EntityState::DEAD}, {EntityState::IDLE}}, // IDLE
		{ {EntityState::RUNNING}, {EntityState::ATTACKING}, {EntityState::DEAD}, {EntityState::IDLE}}, // RUNNING
		{ {EntityState::IDLE},	  {EntityState::IDLE},		{EntityState::DEAD}, {EntityState::IDLE}}, // ATTACKING
		{ {EntityState::DEAD},	  {EntityState::DEAD},		{EntityState::DEAD}, {EntityState::IDLE}}  // DEAD
	};

	EntityState currentState = state;


	EntityState nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(currentState)].next_state;

};



#endif // __OLS_H__
