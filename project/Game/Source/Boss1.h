﻿//#pragma once
//#ifndef __ENEMY_OSIRIS_H__
//#define __ENEMY_OSIRIS_H__
//
//
//#include "Entity.h"
//#include "Point.h"
//#include "SDL/include/SDL.h"
//#include "Animation.h"
//#include "Pathfinding.h"
//#include "Player.h"
//#include "Physics.h"
//
//struct SDL_Texture;
//
//
//struct Branch_Osiris {
//	enum EntityState_Enemy const next_state;
//	Branch_Osiris(EntityState_Enemy next) : next_state(next) {}
//};
//
//
//class Enemy_Osiris : public Entity
//{
//
//
//public:
//
//	Enemy_Osiris();
//
//	virtual ~Enemy_Osiris();
//
//	bool Awake();
//
//	bool Start();
//
//	bool Update(float dt);
//
//	bool PostUpdate();
//
//	bool CleanUp();
//
//	void DoNothing(float dt);
//	void Chase(float dt, iPoint playerPos);
//	void Attack(float dt);
//	void Die();
//	void Revive();
//	bool Osirisfinding(float dt, iPoint playerPos);
//
//	// L07 DONE 6: Define OnCollision function for the player. 
//	void OnCollision(PhysBody* physA, PhysBody* physB);
//
//	void SetPlayer(Player* player);
//
//	float GetHealth() const;
//	void TakeDamage(float damage);
//	void stateMachine(float dt, iPoint playerPos);
//	//Veneno
//	void ApplyPoison(int poisonDamage, float poisonDuration, float poisonTickRate);
//
//public:
//
//	//L02: DONE 2: Declare player parameters
//	SDL_Texture* texture = NULL;
//	pugi::xml_node config;
//	uint texW, texH;
//
//	//Estadisticas
//	float speed;
//	b2Vec2 vel;
//	float health;
//	float maxHealth;
//	float attackDamage;
//	Timer invulnerabilityTimer;
//
//	float viewDistance;
//	float attackDistance;
//
//	//Revivir
//
//
//
//private:
//	pugi::xml_document configFile;
//	pugi::xml_node configNode;
//
//
//	Animation idleAnim;
//	Animation runAnim;
//	Animation attackAnim;
//	Animation dieAnim;
//
//	bool isFacingLeft = false;
//
//	iPoint originalPosition;
//
//	bool isDead = false;
//	bool isReviving = false;
//	bool hasRevived = false;
//	bool tempo = false;
//	Timer reviveTimer;
//
//	const float reviveTime = 5.0f;
//
//	int TSprite;
//	int SpriteX;
//	int SpriteY;
//	int Photowidth;
//
//	int currentPosX;
//
//	PathFinding* path;
//	DynArray<iPoint> lastPath;
//
//	Animation* currentAnimation = nullptr;
//	EntityState_Enemy state;
//
//
//	Animation SPosition;
//	SDL_Rect* spritePositions;
//
//	Timer timerRecibirDanioColor;
//
//	//Veneno
//	float poisonTimer = 0.0f; // Tiempo desde que se aplic� el veneno
//	float poisonDuration = 0.0f; // Duraci�n total del veneno
//	float poisonTickRate = 0.0f; // Tiempo entre cada tick de da�o de veneno
//	float poisonDamage = 0.0f; // Da�o de veneno por tick
//	float timeSinceLastTick = 0.0f; // Tiempo desde el �ltimo tick de da�o de veneno
//	bool poisoned = false;
//
//public:
//
//	Branch_Osiris transitionTable[static_cast<int>(EntityState_Enemy::STATE_COUNT)][static_cast<int>(EntityState_Enemy::STATE_COUNT)] = {
//		//		IDLE						RUNNING								ATTACKING					DEAD						REVIVING							DASHI						NONE
//		{ {EntityState_Enemy::IDLE}, {EntityState_Enemy::RUNNING}, {EntityState_Enemy::ATTACKING}, {EntityState_Enemy::DEAD}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::IDLE}}, // IDLE
//		{ {EntityState_Enemy::IDLE}, {EntityState_Enemy::RUNNING}, {EntityState_Enemy::ATTACKING}, {EntityState_Enemy::DEAD}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::IDLE}}, // RUNNING
//		{ {EntityState_Enemy::IDLE}, {EntityState_Enemy::RUNNING}, {EntityState_Enemy::ATTACKING}, {EntityState_Enemy::DEAD}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::IDLE}}, // ATTACKING
//		{ {EntityState_Enemy::IDLE}, {EntityState_Enemy::RUNNING}, {EntityState_Enemy::ATTACKING}, {EntityState_Enemy::DEAD}, {EntityState_Enemy::REVIVING}, {EntityState_Enemy::NONE}, {EntityState_Enemy::IDLE}}, // DEAD
//		{ {EntityState_Enemy::IDLE}, {EntityState_Enemy::RUNNING}, {EntityState_Enemy::ATTACKING}, {EntityState_Enemy::DEAD}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::IDLE}}, // REVIVING
//		{ {EntityState_Enemy::IDLE}, {EntityState_Enemy::RUNNING}, {EntityState_Enemy::ATTACKING}, {EntityState_Enemy::DEAD}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::IDLE}}, // DASHI
//		{ {EntityState_Enemy::IDLE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE},  {EntityState_Enemy::IDLE}} // NONE
//	};
//
//	EntityState_Enemy currentState = state;
//	EntityState_Enemy desiredState = nextState;
//	EntityState_Enemy nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(desiredState)].next_state;
//
//};
//
//
//
//#endif // __ENEMY_OSIRIS_H__