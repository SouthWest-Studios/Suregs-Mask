﻿#pragma once
#ifndef __BOSS_INUIT_H__
#define __BOSS_INUIT_H__


#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "Player.h"
#include "Physics.h"
#include <unordered_map>

struct SDL_Texture;


struct Branch_Inuit {
	enum EntityState_Boss_Inuit const next_state;
	Branch_Inuit(EntityState_Boss_Inuit next) : next_state(next) {}
};

enum class FASE
{
	FASE_ONE,
	FASE_CHANGE,
	FASE_TWO
};

class Boss_Inuit : public Entity
{


public:

	Boss_Inuit();

	virtual ~Boss_Inuit();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void DoNothing(float dt);

	void Chase(float dt, iPoint playerPos);
	void Attack(float dt);
	void Die();
	void Revive();
	bool Bossfinding(float dt, iPoint playerPos);
	void deleteCollision(PhysBody* phy);
	void SetPlayer(Player* player);
	float GetHealth() const;
	void TakeDamage(float damage);
	void atackBoomerang(BTPDirection direccion);
	BTPDirection calculate_direction();
	std::string directionToString(BTPDirection direction);
	void stateMachine(float dt, iPoint playerPos);
	void shock_wave(int posX, int posY, float shockSpeed, float size, int tag);
	void ulti_Atack();
	bool TimerColdDown(float time);
	bool waveTimerColdDown(float time);
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnEndCollision(PhysBody* physA, PhysBody* physB);
	//Veneno
	void ApplyPoison(int poisonDamage, float poisonDuration, float poisonTickRate);

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
	Timer invulnerabilityTimer;

	float viewDistance;
	float attackDistance;
	int attackTime;
	bool getBossArea = true;
	//Revivir

	Timer atackTimeColdDown;


private:
	pugi::xml_document configFile;
	pugi::xml_node configNode;

	//physi body
	PhysBody* areaSensor;
	PhysBody* atackCube;
	PhysBody* atackBMR;
	//PhysBody* atackShockWave = nullptr;
	std::unordered_map<int, PhysBody*> shockWaves;


	//animacion
	Animation idleAnim;
	Animation runAnim;
	Animation attackAnim;
	Animation dieAnim;

	bool isFacingLeft = false;

	iPoint originalPosition;
	iPoint originalWavePosition;
	iPoint bossArea;

	int TSprite;
	int SpriteX;
	int SpriteY;
	int Photowidth;

	int currentPosX;

	PathFinding* path;
	DynArray<iPoint> lastPath;

	Animation* currentAnimation = nullptr;
	EntityState_Boss_Inuit state;


	Animation SPosition;
	SDL_Rect* spritePositions;

	FASE fase;

	//ataque
	float ataqueTimeClodDown = 0;
	bool inAtack = false;
	bool checkAtackBMR = false;
	float bmrSpeed;
	bool bmrBack = false;
	bool inbmrAtack = false;

	BTPDirection playerDireccion;
	std::string printplayerDireccion;


	//shockWave
	//float shockSize;
	bool waveIsMax = false;
	bool waveFinishi = true;
	Timer waveTime;
	float waveTimeClodDown = 0;
	int distance = 0;
	//ulti
	bool ultDef = false;
	bool saveOriginPos = true;


	const float reviveTime = 5.0f;
	bool isDead = false;
	bool isReviving = false;
	bool hasRevived = false;
	bool tempo = false;
	Timer reviveTimer;
	//Veneno
	float poisonTimer = 0.0f; // Tiempo desde que se aplic� el veneno
	float poisonDuration = 0.0f; // Duraci�n total del veneno
	float poisonTickRate = 0.0f; // Tiempo entre cada tick de da�o de veneno
	float poisonDamage = 0.0f; // Da�o de veneno por tick
	float timeSinceLastTick = 0.0f; // Tiempo desde el �ltimo tick de da�o de veneno
	bool poisoned = false;

public:

	Branch_Inuit transitionTable[static_cast<int>(EntityState_Boss_Inuit::STATE_COUNT)][static_cast<int>(EntityState_Boss_Inuit::STATE_COUNT)] = {
		//				IDLE						RUNNING								ATTACKING_BASIC								ATTACKING_DISTANCE									DEAD							DASHI						FASE_CHANGE								NONE
		{ {EntityState_Boss_Inuit::IDLE}, {EntityState_Boss_Inuit::RUNNING}, {EntityState_Boss_Inuit::ATTACKING_BASIC},{EntityState_Boss_Inuit::ATTACKING_DISTANCE}, {EntityState_Boss_Inuit::DEAD}, {EntityState_Boss_Inuit::DASHI}, {EntityState_Boss_Inuit::FASE_CHANGE}, {EntityState_Boss_Inuit::IDLE}}, // IDLE
		{ {EntityState_Boss_Inuit::IDLE}, {EntityState_Boss_Inuit::RUNNING}, {EntityState_Boss_Inuit::ATTACKING_BASIC},{EntityState_Boss_Inuit::ATTACKING_DISTANCE}, {EntityState_Boss_Inuit::DEAD}, {EntityState_Boss_Inuit::DASHI}, {EntityState_Boss_Inuit::FASE_CHANGE}, {EntityState_Boss_Inuit::IDLE}}, // RUNNING
		{ {EntityState_Boss_Inuit::IDLE}, {EntityState_Boss_Inuit::RUNNING}, {EntityState_Boss_Inuit::ATTACKING_BASIC},{EntityState_Boss_Inuit::ATTACKING_DISTANCE}, {EntityState_Boss_Inuit::DEAD}, {EntityState_Boss_Inuit::DASHI}, {EntityState_Boss_Inuit::FASE_CHANGE}, {EntityState_Boss_Inuit::IDLE}}, // ATTACKING_BASIC
		{ {EntityState_Boss_Inuit::IDLE}, {EntityState_Boss_Inuit::RUNNING}, {EntityState_Boss_Inuit::ATTACKING_BASIC},{EntityState_Boss_Inuit::ATTACKING_DISTANCE}, {EntityState_Boss_Inuit::DEAD}, {EntityState_Boss_Inuit::DASHI}, {EntityState_Boss_Inuit::FASE_CHANGE}, {EntityState_Boss_Inuit::IDLE}}, // ATTACKING_DISTANCE
		{ {EntityState_Boss_Inuit::IDLE}, {EntityState_Boss_Inuit::RUNNING}, {EntityState_Boss_Inuit::ATTACKING_BASIC},{EntityState_Boss_Inuit::ATTACKING_DISTANCE}, {EntityState_Boss_Inuit::DEAD}, {EntityState_Boss_Inuit::DASHI}, {EntityState_Boss_Inuit::FASE_CHANGE}, {EntityState_Boss_Inuit::IDLE}}, // DEAD
		{ {EntityState_Boss_Inuit::IDLE}, {EntityState_Boss_Inuit::RUNNING}, {EntityState_Boss_Inuit::ATTACKING_BASIC},{EntityState_Boss_Inuit::ATTACKING_DISTANCE}, {EntityState_Boss_Inuit::DEAD}, {EntityState_Boss_Inuit::DASHI}, {EntityState_Boss_Inuit::FASE_CHANGE}, {EntityState_Boss_Inuit::IDLE}}, // DASHI
		{ {EntityState_Boss_Inuit::IDLE}, {EntityState_Boss_Inuit::RUNNING}, {EntityState_Boss_Inuit::ATTACKING_BASIC},{EntityState_Boss_Inuit::ATTACKING_DISTANCE}, {EntityState_Boss_Inuit::DEAD}, {EntityState_Boss_Inuit::DASHI}, {EntityState_Boss_Inuit::FASE_CHANGE}, {EntityState_Boss_Inuit::IDLE}}, // FASE_CHANGE
		{ {EntityState_Boss_Inuit::IDLE}, {EntityState_Boss_Inuit::NONE}, {EntityState_Boss_Inuit::NONE},{EntityState_Boss_Inuit::NONE}, {EntityState_Boss_Inuit::NONE}, {EntityState_Boss_Inuit::NONE}, {EntityState_Boss_Inuit::NONE}, {EntityState_Boss_Inuit::IDLE}} // NONE
	};

	EntityState_Boss_Inuit currentState = state;
	EntityState_Boss_Inuit desiredState = nextState;
	EntityState_Boss_Inuit nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(desiredState)].next_state;

};



#endif  