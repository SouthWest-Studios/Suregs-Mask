#ifndef __ENEMY_GUERRERO_H__
#define __ENEMY_GUERRERO_H__


#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "Player.h"
#include "Physics.h"

struct SDL_Texture;
 

struct Branch_Guerrero {
	enum EntityState_Enemy const next_state;
	Branch_Guerrero(EntityState_Enemy next) : next_state(next) {}
};


class Enemy_Guerrero : public Entity
{


public:

	Enemy_Guerrero();

	virtual ~Enemy_Guerrero();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void Chase(float dt, iPoint playerPos);
	void Attack(float dt);
	void Die();
	bool Guerrerofinding(float dt, iPoint playerPos);

	// L07 DONE 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);

	void SetPlayer(Player* player);

	float GetHealth() const;
	void TakeDamage(float damage);
	void stateMachine(float dt, iPoint playerPos);

	MapObject* GetCurrentRoom();

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
	Timer invulnerabilityTimer;

	float viewDistance;
	float attackDistance;

	bool isFacingLeft = false;
	Animation* currentAnimation = nullptr;

	

private:
	pugi::xml_document configFile;
	pugi::xml_node configNode;
	
	Animation runAnim;
	Animation attackAnim;

	int osiris_get_damage_fx;
	int osiris_death_fx;

	iPoint originalPosition;

	int TSprite;
	int SpriteX;
	int SpriteY;
	int Photowidth;

	int currentPosX;

	PathFinding* path;
	DynArray<iPoint> lastPath;

	EntityState_Enemy state;

	Timer timerRecibirDanioColor;

	Animation SPosition;
	SDL_Rect* spritePositions;

public:

	Branch_Guerrero transitionTable[static_cast<int>(EntityState_Enemy::STATE_COUNT)][static_cast<int>(EntityState_Enemy::STATE_COUNT)] = {
		//		IDLE						RUNNING								ATTACKING					DEAD						REVIVING							DASHI						NONE
		{ {EntityState_Enemy::IDLE}, {EntityState_Enemy::RUNNING}, {EntityState_Enemy::ATTACKING}, {EntityState_Enemy::DEAD}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::IDLE}}, // IDLE
		{ {EntityState_Enemy::IDLE}, {EntityState_Enemy::RUNNING}, {EntityState_Enemy::ATTACKING}, {EntityState_Enemy::DEAD}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::IDLE}}, // RUNNING
		{ {EntityState_Enemy::IDLE}, {EntityState_Enemy::RUNNING}, {EntityState_Enemy::ATTACKING}, {EntityState_Enemy::DEAD}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::IDLE}}, // ATTACKING
		{ {EntityState_Enemy::IDLE}, {EntityState_Enemy::RUNNING}, {EntityState_Enemy::ATTACKING}, {EntityState_Enemy::DEAD}, {EntityState_Enemy::REVIVING}, {EntityState_Enemy::NONE}, {EntityState_Enemy::IDLE}}, // DEAD
		{ {EntityState_Enemy::IDLE}, {EntityState_Enemy::RUNNING}, {EntityState_Enemy::ATTACKING}, {EntityState_Enemy::DEAD}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::IDLE}}, // REVIVING
		{ {EntityState_Enemy::IDLE}, {EntityState_Enemy::RUNNING}, {EntityState_Enemy::ATTACKING}, {EntityState_Enemy::DEAD}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::IDLE}}, // DASHI
		{ {EntityState_Enemy::IDLE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE},  {EntityState_Enemy::IDLE}} // NONE
	};

	EntityState_Enemy currentState;
	EntityState_Enemy desiredState;
	EntityState_Enemy nextState;

};



#endif // __ENEMY_GUERRERO_H__