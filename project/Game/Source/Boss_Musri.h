#pragma once
#ifndef __BOSS_MUSRI_H__
#define __BOSS_MUSRI_H__


#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "Player.h"
#include "Physics.h"

struct SDL_Texture;


struct Branch_Musri {
	enum EntityState_Boss_Musri const next_state;
	Branch_Musri(EntityState_Boss_Musri next) : next_state(next) {}
};

enum class FASE_Musri
{
	FASE_ONE,
	FASE_CHANGE,
	FASE_TWO
};

class Boss_Musri : public Entity
{


public:

	Boss_Musri();

	virtual ~Boss_Musri();

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

	// L07 DONE 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);

	void SetPlayer(Player* player);

	float GetHealth() const;
	void TakeDamage(float damage);
	void stateMachine(float dt, iPoint playerPos);
	//Veneno
	void ApplyPoison(int poisonDamage, float poisonDuration, float poisonTickRate);



	void Fase1(float dt);
	void FaseC(float dt);
	void Fase2(float dt);

	iPoint GetRandomPosicion(int distanceLimit = 10);

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

	//Revivir



private:
	pugi::xml_document configFile;
	pugi::xml_node configNode;

	Animation idleAnim;
	Animation attackEmpujarAnim;
	Animation attackFlechasRapidasAnim;
	Animation attackFlechaCargadaAnim;
	
	bool isFacingLeft = false;

	int TSprite;
	int SpriteX;
	int SpriteY;
	int Photowidth;

	int currentPosX;

	PathFinding* path;
	DynArray<iPoint> lastPath;

	Animation* currentAnimation = nullptr;

	EntityState_Boss_Musri state;
	FASE_Musri fase;


	Animation SPosition;
	SDL_Rect* spritePositions;

	Timer cambiarPosicionTimer;
	Timer dispararRafagasTimer;

	Timer habilidadEmpuje;
	Timer habilidadFlechaCargada;
	Timer habilidadDashInvisible;


	int cambiarPosicionTime;
	int dispararRafagasTime;
	int numeroRafagas;
	
	SDL_Rect limitesSala = { 9800 , 2650, 2200, 1150 };


	//Veneno
	float poisonTimer = 0.0f; // Tiempo desde que se aplic� el veneno
	float poisonDuration = 0.0f; // Duraci�n total del veneno
	float poisonTickRate = 0.0f; // Tiempo entre cada tick de da�o de veneno
	float poisonDamage = 0.0f; // Da�o de veneno por tick
	float timeSinceLastTick = 0.0f; // Tiempo desde el �ltimo tick de da�o de veneno
	bool poisoned = false;

public:

		Branch_Musri transitionTable[static_cast<int>(EntityState_Boss_Musri::STATE_COUNT)][static_cast<int>(EntityState_Boss_Musri::STATE_COUNT)] = {
		//				IDLE						RUNNING								ATTACKING_BASIC								ATTACKING_DISTANCE									DEAD							DASHI						FASE_CHANGE								NONE
		{ {EntityState_Boss_Musri::IDLE}, {EntityState_Boss_Musri::RUNNING}, {EntityState_Boss_Musri::ATTACKING_BASIC},{EntityState_Boss_Musri::ATTACKING_DISTANCE}, {EntityState_Boss_Musri::DEAD}, {EntityState_Boss_Musri::DASHI}, {EntityState_Boss_Musri::FASE_CHANGE}, {EntityState_Boss_Musri::IDLE}}, // IDLE
		{ {EntityState_Boss_Musri::IDLE}, {EntityState_Boss_Musri::RUNNING}, {EntityState_Boss_Musri::ATTACKING_BASIC},{EntityState_Boss_Musri::ATTACKING_DISTANCE}, {EntityState_Boss_Musri::DEAD}, {EntityState_Boss_Musri::DASHI}, {EntityState_Boss_Musri::FASE_CHANGE}, {EntityState_Boss_Musri::IDLE}}, // RUNNING
		{ {EntityState_Boss_Musri::IDLE}, {EntityState_Boss_Musri::RUNNING}, {EntityState_Boss_Musri::ATTACKING_BASIC},{EntityState_Boss_Musri::ATTACKING_DISTANCE}, {EntityState_Boss_Musri::DEAD}, {EntityState_Boss_Musri::DASHI}, {EntityState_Boss_Musri::FASE_CHANGE}, {EntityState_Boss_Musri::IDLE}}, // ATTACKING_BASIC
		{ {EntityState_Boss_Musri::IDLE}, {EntityState_Boss_Musri::RUNNING}, {EntityState_Boss_Musri::ATTACKING_BASIC},{EntityState_Boss_Musri::ATTACKING_DISTANCE}, {EntityState_Boss_Musri::DEAD}, {EntityState_Boss_Musri::DASHI}, {EntityState_Boss_Musri::FASE_CHANGE}, {EntityState_Boss_Musri::IDLE}}, // ATTACKING_DISTANCE
		{ {EntityState_Boss_Musri::IDLE}, {EntityState_Boss_Musri::RUNNING}, {EntityState_Boss_Musri::ATTACKING_BASIC},{EntityState_Boss_Musri::ATTACKING_DISTANCE}, {EntityState_Boss_Musri::DEAD}, {EntityState_Boss_Musri::DASHI}, {EntityState_Boss_Musri::FASE_CHANGE}, {EntityState_Boss_Musri::IDLE}}, // DEAD
		{ {EntityState_Boss_Musri::IDLE}, {EntityState_Boss_Musri::RUNNING}, {EntityState_Boss_Musri::ATTACKING_BASIC},{EntityState_Boss_Musri::ATTACKING_DISTANCE}, {EntityState_Boss_Musri::DEAD}, {EntityState_Boss_Musri::DASHI}, {EntityState_Boss_Musri::FASE_CHANGE}, {EntityState_Boss_Musri::IDLE}}, // DASHI
		{ {EntityState_Boss_Musri::IDLE}, {EntityState_Boss_Musri::RUNNING}, {EntityState_Boss_Musri::ATTACKING_BASIC},{EntityState_Boss_Musri::ATTACKING_DISTANCE}, {EntityState_Boss_Musri::DEAD}, {EntityState_Boss_Musri::DASHI}, {EntityState_Boss_Musri::FASE_CHANGE}, {EntityState_Boss_Musri::IDLE}}, // FASE_CHANGE
		{ {EntityState_Boss_Musri::IDLE}, {EntityState_Boss_Musri::NONE}, {EntityState_Boss_Musri::NONE},{EntityState_Boss_Musri::NONE}, {EntityState_Boss_Musri::NONE}, {EntityState_Boss_Musri::NONE}, {EntityState_Boss_Musri::NONE}, {EntityState_Boss_Musri::IDLE}} // NONE
	};

	EntityState_Boss_Musri currentState = state;
	EntityState_Boss_Musri desiredState = nextState;
	EntityState_Boss_Musri nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(desiredState)].next_state;

};



#endif  