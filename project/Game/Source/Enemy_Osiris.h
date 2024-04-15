#ifndef __ENEMY_OSIRIS_H__
#define __ENEMY_OSIRIS_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "Player.h"
#include "Physics.h"

struct SDL_Texture;
 

struct Branch_Osiris {
	enum EntityState const next_state;

};

class Enemy_Osiris : public Entity
{


public:

	Enemy_Osiris();

	virtual ~Enemy_Osiris();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void DoNothing(float dt);
	void Chase(float dt);
	void Attack(float dt);
	void Die(float dt);
	void Revive(float dt);
	bool Osirisfinding(float dt);

	// L07 DONE 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);

	void SetPlayer(Player* player);

	float GetHealth() const;

public:

	//L02: DONE 2: Declare player parameters
	SDL_Texture* texture = NULL;
	pugi::xml_node config;
	uint texW, texH;

	//Estadisticas
	float speed;
	b2Vec2 vel;
	float health;
	float maxHealth;
	float attackDamage;

	//Revivir
	bool hasRevived = false;
	float deathTime = 0.0f;
	float reviveDelay = 2.0f;

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
	DynArray<iPoint> lastPath;

private:
	Animation idleAnim;
	Animation runAnim;
	Animation attackAnim;
	Animation dieAnim;

	bool isFacingLeft = false;

public:

	Branch_Osiris transitionTable[static_cast<int>(EntityState::STATE_COUNT)][static_cast<int>(EntityState::STATE_COUNT)] = {
		// isMoving               isAttacking				isDead				isReviving					else                MASK_ATTACK
		{ {EntityState::RUNNING}, {EntityState::ATTACKING}, {EntityState::DEAD}, {EntityState::IDLE},		{EntityState::IDLE}, {EntityState::MASK_ATTACK}}, // IDLE
		{ {EntityState::RUNNING}, {EntityState::ATTACKING}, {EntityState::DEAD}, {EntityState::IDLE},		{EntityState::IDLE}, {EntityState::MASK_ATTACK}}, // RUNNING
		{ {EntityState::IDLE},	  {EntityState::IDLE},		{EntityState::DEAD}, {EntityState::IDLE},		{EntityState::IDLE}, {EntityState::MASK_ATTACK}}, // ATTACKING
		{ {EntityState::DEAD},	  {EntityState::DEAD},		{EntityState::DEAD}, {EntityState::REVIVING},	{EntityState::IDLE}, {EntityState::MASK_ATTACK}}, // DEAD
		{ {EntityState::RUNNING}, {EntityState::ATTACKING}, {EntityState::DEAD}, {EntityState::IDLE},		{EntityState::IDLE}, {EntityState::MASK_ATTACK}}, // REVIVING
		{ {EntityState::IDLE},	  {EntityState::IDLE},	    {EntityState::DEAD}, {EntityState::IDLE},		{EntityState::IDLE}, {EntityState::MASK_ATTACK}} // MASK_ATTACK
	};

	EntityState currentState = state;


	EntityState nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(currentState)].next_state;

};



#endif // __ENEMY_OSIRIS_H__