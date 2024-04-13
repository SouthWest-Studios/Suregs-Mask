#ifndef __NPC_PESCADOR_H__
#define __NPC_PESCADOR_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Timer.h"

struct SDL_Texture;


//struct Branch {
//	enum EntityState const next_state;
//	//Branch() : next_state(EntityState::STATE_COUNT) {}
//
//};

class NPCPescador : public Entity
{


public:

	NPCPescador();
	
	virtual ~NPCPescador();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	

	//Branch transitionTable[static_cast<int>(EntityState::STATE_COUNT)][static_cast<int>(EntityState::STATE_COUNT)];
	// L07 DONE 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);

private:
	

public:

	SDL_Texture* texture = NULL;
	uint texW, texH;



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


private:
	Animation idleAnim;

public:

	//Branch transitionTable[static_cast<int>(EntityState::STATE_COUNT)][static_cast<int>(EntityState::STATE_COUNT)] = {
	//	{EntityState::RUNNING, EntityState::ATTACKING, EntityState::DEAD, EntityState::IDLE, EntityState::MASK_ATTACK}, // IDLE
	//	{EntityState::RUNNING, EntityState::ATTACKING, EntityState::DEAD, EntityState::IDLE, EntityState::MASK_ATTACK}, // RUNNING
	//	{EntityState::IDLE, EntityState::IDLE, EntityState::DEAD, EntityState::IDLE, EntityState::MASK_ATTACK}, // ATTACKING
	//	{EntityState::DEAD, EntityState::DEAD, EntityState::DEAD, EntityState::IDLE, EntityState::MASK_ATTACK}, // DEAD
	//	{EntityState::IDLE, EntityState::IDLE, EntityState::DEAD, EntityState::IDLE, EntityState::MASK_ATTACK} // MASK_ATTACK
	//};

	//EntityState currentState = state;


	//EntityState nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(currentState)].next_state;

};



#endif // __NPC_VENDEDOR_H__