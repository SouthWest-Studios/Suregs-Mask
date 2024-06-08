﻿#ifndef __ENEMY_BOOROK_H__
#define __ENEMY_BOOROK_H__


#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "Player.h"
#include "Physics.h"
#include "Particle.h"
#include "ParticlePool.h"
#include "Emitter.h"

struct SDL_Texture;


struct Branch_Boorok {
	enum EntityState_Enemy const next_state;
	Branch_Boorok(EntityState_Enemy next) : next_state(next) {}
};

class Enemy_Boorok : public Entity
{


public:

	Enemy_Boorok();

	virtual ~Enemy_Boorok();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void DoNothing(float dt, iPoint playerPos);
	void Chase(float dt, iPoint playerPos);
	void Attack(float dt, iPoint playerPos);
	void Die();
	bool Boorokfinding(float dt, iPoint playerPos);

	void OnCollision(PhysBody* physA, PhysBody* physB);

	float GetHealth() const;
	void TakeDamage(float damage);

	void Sleeping();

	//Ataque cargado
	void chargeAttack(iPoint playerPos);
	void AreaAttack(iPoint playerPos);

	//VENENO <----------
	void ApplyPoison(int poisonDamage, float poisonDuration, float poisonTickRate);
	void CheckPoison();
	//VENENO ---------->

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
	float healthIncrement;
	float attackDamage;
	Timer invulnerabilityTimer;

	float viewDistance;
	float attackDistance;
	float chargeattackDistance;

private:
	pugi::xml_document configFile;
	pugi::xml_node configNode;

	Emitter* blood = nullptr;

	Animation sleepAnim;
	Animation runAnim;
	Animation attackAnim;
	Animation reciebeDamage;
	Animation idleAnim;
	Animation wakeupAnim;
	Animation chargeAttackAnim;
	Animation dieAnim;

	int boorok_attack_fx;
	int boorok_get_damage_fx;
	int boorok_get_damageAlt_fx;
	int boorok_death_fx;
	bool death_fx = false;

	bool isFacingLeft = false;

	iPoint originalPosition;

	int TSprite;
	int SpriteX;
	int SpriteY;
	int Photowidth;

	int currentPosX;

	PathFinding* path;
	DynArray<iPoint> lastPath;

	Animation* currentAnimation = nullptr;
	EntityState_Enemy state;


	Animation SPosition;
	SDL_Rect* spritePositions;

	Timer timerRecibirDanioColor;

	//Sleeping
	bool isSleeping = false;
	bool isWakingUp = false;
	Timer recoveryTimer;
	Timer isabletosleepTimer;

	//Charge Attack
	Timer chargeAttackTimer;
	iPoint Antposition;
	float areaattackdamage;
	float chargeAttackMaxRange;
	bool isCharging = false;

	//VENENO <----------
	bool firstTimePoisonRecibed = false;
	Timer poisonTimer;
	Timer poisonTickTimer;
	float poisonDuration = 0.0f; // Duraci�n total del veneno
	float poisonTickRate = 0.0f; // Tiempo entre cada tick de da�o de veneno
	float poisonDamage = 0.0f; // Da�o de veneno por tick
	bool poisoned = false;
	//VENENO ---------->

public:

	Branch_Boorok transitionTable[static_cast<int>(EntityState_Enemy::STATE_COUNT)][static_cast<int>(EntityState_Enemy::STATE_COUNT)] = {
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



#endif // __ENEMY_BOOROK_H__