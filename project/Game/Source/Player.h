#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Timer.h"
#include "Fishing.h"
#include "Physics.h"

struct SDL_Texture;


struct Branch {
	enum EntityState const next_state;
	//Branch() : next_state(EntityState::STATE_COUNT) {}

};

enum class Mask{
	MASK0,
	MASK1
};

struct Stats {
	int health;
	int movementSpeed;
	int attackDamage;
};

struct MaskStats {
    int healthModifier;
    int movementSpeedModifier;
    int attackDamageModifier;
};

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

	void DoNothing(float dt);
	void Run(float dt);
	void Attack(float dt);

	//Branch transitionTable[static_cast<int>(EntityState::STATE_COUNT)][static_cast<int>(EntityState::STATE_COUNT)];
	// L07 DONE 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);
    
    Entity* GetEnemyWithHighestHealthWithinRadius(iPoint position, int radius);
	
	//Funciones ataques mascara
	void CastLightning();

	void AreaAttack(float dt);

	//Estadísticas

    Stats baseStats;
    Stats currentStats;

private:
	void CameraMovement(float dt);
	void GodMode(float dt);
	void PlayerMovement(float dt);
	void FishingDirecction(bool verticalMovement, bool horizontalMovement);
	void MaskAttack(float dt);
	void ChangeMask();

		

public:

	//L02: DONE 2: Declare player parameters
	float speed = 0.2f;
	SDL_Texture* texture = NULL;
	
	uint texW, texH;

	//Audio fx
	int pickCoinFxId;


	Animation* currentAnimation = nullptr;
	EntityState state;

	//Animation
	Animation SPosition;
	SDL_Rect* spritePositions;
	pugi::xml_node config;
	int TSprite;
	int SpriteX;
	int SpriteY;
	int Photowidth;

	//Rodar
	
	bool getPlayerPosition = false;
	int rodar_PlayerPosition;
	int rodar_PotisionX;
	int currentPosX;
	
	float speedDash = 3;

	//Ataque
	bool isAttacking = false;
	PhysBody* attackSensor = nullptr;
	float attackDamage = 50;
	
	//Mascara
	Mask currentMask;

	bool isAttackingMask = true;
	PhysBody* mask1AttackSensor = nullptr;
	int attackMask1Width = 300;  
    int attackMask1Height = 300; 


	//Fishing
	//New//
	bool playermove = false;
	Direction player_Direction;

	//Player move
	bool pressingUp;
	bool pressingDown;
	bool pressingLeft;
	bool pressingRight;


private:
	Animation idleAnim;
	Animation runAnim;

	bool isFacingLeft = false;
	bool isDashing = false;

	bool godmode = false;

	bool camaralibre = false;

	int cdTimerDashMS;

	Timer timerDash;
	Timer cdTimerDash; 

	int cdTimerAttackMS = 250;
	Timer timerAttack;

	fPoint lastMovementDirection;

	int cdTimerMaskAttackMS = 1000;
	Timer timerMaskAttack;

public:

	Branch transitionTable[static_cast<int>(EntityState::STATE_COUNT)][static_cast<int>(EntityState::STATE_COUNT)] = {
		// isMoving					isAttacking				isDead               isReviving					else                MASK_ATTACK
		{ {EntityState::RUNNING}, {EntityState::ATTACKING}, {EntityState::DEAD}, {EntityState::IDLE}, {EntityState::IDLE}, {EntityState::MASK_ATTACK}}, // IDLE
		{ {EntityState::RUNNING}, {EntityState::ATTACKING}, {EntityState::DEAD}, {EntityState::IDLE}, {EntityState::IDLE}, {EntityState::MASK_ATTACK}}, // RUNNING
		{ {EntityState::IDLE},	  {EntityState::IDLE},		{EntityState::DEAD}, {EntityState::IDLE}, {EntityState::IDLE}, {EntityState::MASK_ATTACK}}, // ATTACKING
		{ {EntityState::DEAD},	  {EntityState::DEAD},		{EntityState::DEAD}, {EntityState::IDLE}, {EntityState::IDLE}, {EntityState::MASK_ATTACK}}, // DEAD
		{ {EntityState::IDLE},	  {EntityState::IDLE},	    {EntityState::DEAD}, {EntityState::IDLE}, {EntityState::IDLE}, {EntityState::MASK_ATTACK}}, // MASK_ATTACK
		{ {EntityState::IDLE},	  {EntityState::IDLE},	    {EntityState::IDLE}, {EntityState::IDLE}, {EntityState::IDLE}, {EntityState::IDLE}} // REVIVING
	};

	EntityState currentState = state;


	EntityState nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(currentState)].next_state;

};



#endif // __PLAYER_H__