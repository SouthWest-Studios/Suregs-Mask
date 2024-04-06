#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Timer.h"

struct SDL_Texture;


struct Branch {
	enum EntityState const next_state;
	//Branch() : next_state(EntityState::STATE_COUNT) {}

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


private:
	void CameraMovement(float dt);
	void GodMode(float dt);
	void PlayerMovement(float dt);
	void Rodar(float dt);

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
	bool inRodar = false;
	bool getPlayerPosition = false;
	int rodar_PlayerPosition;
	int rodar_PotisionX;
	int currentPosX;
	
	float speedDash = 3;
	Timer timerDash;

private:
	Animation idleAnim;
	Animation runAnim;

	bool isFacingLeft = false;

	bool godmode = false;

	bool camaralibre = false;

	

public:

	Branch transitionTable[static_cast<int>(EntityState::STATE_COUNT)][static_cast<int>(EntityState::STATE_COUNT)] = {
		// isMoving               isAttacking            else
		{ {EntityState::RUNNING}, {EntityState::ATTACKING}, {EntityState::IDLE}}, // IDLE
		{ {EntityState::RUNNING}, {EntityState::ATTACKING}, {EntityState::IDLE}}, // RUNNING
		{ {EntityState::RUNNING}, {EntityState::ATTACKING}, {EntityState::IDLE}} // RUNNING
	};

	EntityState currentState = state;


	EntityState nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(currentState)].next_state;

};



#endif // __PLAYER_H__