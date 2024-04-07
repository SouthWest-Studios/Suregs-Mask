#ifndef __ENEMY_SHAR_H__
#define __ENEMY_SHAR_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "Player.h"

struct SDL_Texture;


struct Branch_Shar {
	enum EntityState const next_state;

};

class Enemy_Shar : public Entity
{


public:

	Enemy_Shar();

	virtual ~Enemy_Shar();

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
	float attackDamage;

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

	Branch_Shar transitionTable[static_cast<int>(EntityState::STATE_COUNT)][static_cast<int>(EntityState::STATE_COUNT)] = {
		// isMoving               isAttacking						 isDead                else
		{ {EntityState::RUNNING}, {EntityState::ATTACKING}, {EntityState::DEAD}, {EntityState::IDLE}}, // IDLE
		{ {EntityState::RUNNING}, {EntityState::ATTACKING}, {EntityState::DEAD}, {EntityState::IDLE}}, // RUNNING
		{ {EntityState::IDLE},	  {EntityState::IDLE},		{EntityState::DEAD}, {EntityState::IDLE}}, // ATTACKING
		{ {EntityState::DEAD},	  {EntityState::DEAD},		{EntityState::DEAD}, {EntityState::IDLE}}  // DEAD
	};

	EntityState currentState = state;


	EntityState nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(currentState)].next_state;

};



#endif // __ENEMY_SHAR_H__