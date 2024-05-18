#pragma once
#ifndef __BOSS_SURMA_H__
#define __BOSS_SURMA_H__


#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "Player.h"
#include "Physics.h"

struct SDL_Texture;


struct Branch_Surma {
	enum EntityState_Boss_Surma const next_state;
	Branch_Surma(EntityState_Boss_Surma next) : next_state(next) {}
};


enum class FASE_Surma
{
	FASE_ONE,
	FASE_CHANGE,
	FASE_TWO,
	FASE_DYNIG
};


struct ExplosionAtaque
{
	PhysBody* pbody;
	Timer lifeTime;
};

class Boss_Surma : public Entity
{


public:

	Boss_Surma();

	virtual ~Boss_Surma();

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

	float GetHealth() const;
	void TakeDamage(float damage);
	void stateMachine(float dt, iPoint playerPos);
	//Veneno
	void ApplyPoison(int poisonDamage, float poisonDuration, float poisonTickRate);



	void Fase1(float dt, iPoint playerPos);
	void FaseC(float dt, iPoint playerPos);
	void Fase2(float dt, iPoint playerPos);
	void FaseDying(float dt, iPoint playerPos);

	

public:

	//L02: DONE 2: Declare player parameters
	SDL_Texture* texture = NULL;
	pugi::xml_node config;
	uint texW, texH;

	//Estadisticas
	float speed;
	float speedSecondFase;
	b2Vec2 vel;
	float attackDamage;
	Timer invulnerabilityTimer;

	float viewDistance;
	float attackDistance;




private:
	pugi::xml_document configFile;
	pugi::xml_node configNode;

	Animation idleAnim;
	Animation runAnim;
	Animation combo1Anim;
	Animation combo2Anim;
	Animation combo3Anim;
	Animation ataqueCargadoAnim;
	Animation ataqueCargadoEjecutarAnim;
	Animation ataqueCargadoExplosionAnim;
	Animation cansadoAnim;
	Animation ataqueRapidoAnim;
	Animation cambioFaseAnim;
	Animation muerteAnim;

	int Surma_dash_fx;
	
	bool isFacingLeft = false;

	int TSprite;
	int SpriteX;
	int SpriteY;
	int Photowidth;

	int currentPosX;

	PathFinding* path;
	DynArray<iPoint> lastPath;

	Animation* currentAnimation = nullptr;

	EntityState_Boss_Surma state;
	FASE_Surma fase;


	Animation SPosition;
	SDL_Rect* spritePositions;

	Timer cargaAtaqueTimer;
	Timer cansadoTimer;
	Timer explosionEspadaTimer;

	Timer cambioFaseTimer;

	int meleeAttackDistance = 4;

	int cargaAtaqueMS = 1000;
	int cansadoMS = 4000;
	int cambioFaseMS = 2000;

	bool realizandoCombo = false;
	bool jugadorCerca = false;


	fPoint sprieOffsetL = fPoint(-220, -200);
	fPoint sprieOffsetR = fPoint(-100, -200);

	SDL_Color actualColorTint{ 255, 255, 255 };
	float actualScale = 1;

	int cantidadCombosRealizados = 0;


	ExplosionAtaque* explosionActual = nullptr;
	float tamañoExplosionActual = 1;


	float ataqueNormalDamage = 250;
	float ataqueCargadoDamage = 300;
	float ataqueExplosionDamage = 50;



	//Veneno
	float poisonTimer = 0.0f; // Tiempo desde que se aplic� el veneno
	float poisonDuration = 0.0f; // Duraci�n total del veneno
	float poisonTickRate = 0.0f; // Tiempo entre cada tick de da�o de veneno
	float poisonDamage = 0.0f; // Da�o de veneno por tick
	float timeSinceLastTick = 0.0f; // Tiempo desde el �ltimo tick de da�o de veneno
	bool poisoned = false;


	Timer timerRecibirDanioColor;

public:

		Branch_Surma transitionTable[static_cast<int>(EntityState_Boss_Surma::STATE_COUNT)][static_cast<int>(EntityState_Boss_Surma::STATE_COUNT)] = {
		//				IDLE						RUNNING								ATTACKING_BASIC								ATTACKING_DISTANCE									DEAD							DASHI						FASE_CHANGE								NONE
		{ {EntityState_Boss_Surma::IDLE}, {EntityState_Boss_Surma::RUNNING}, {EntityState_Boss_Surma::ATTACKING_BASIC},{EntityState_Boss_Surma::ATTACKING_DISTANCE}, {EntityState_Boss_Surma::DEAD}, {EntityState_Boss_Surma::DASHI}, {EntityState_Boss_Surma::FASE_CHANGE}, {EntityState_Boss_Surma::IDLE}}, // IDLE
		{ {EntityState_Boss_Surma::IDLE}, {EntityState_Boss_Surma::RUNNING}, {EntityState_Boss_Surma::ATTACKING_BASIC},{EntityState_Boss_Surma::ATTACKING_DISTANCE}, {EntityState_Boss_Surma::DEAD}, {EntityState_Boss_Surma::DASHI}, {EntityState_Boss_Surma::FASE_CHANGE}, {EntityState_Boss_Surma::IDLE}}, // RUNNING
		{ {EntityState_Boss_Surma::IDLE}, {EntityState_Boss_Surma::RUNNING}, {EntityState_Boss_Surma::ATTACKING_BASIC},{EntityState_Boss_Surma::ATTACKING_DISTANCE}, {EntityState_Boss_Surma::DEAD}, {EntityState_Boss_Surma::DASHI}, {EntityState_Boss_Surma::FASE_CHANGE}, {EntityState_Boss_Surma::IDLE}}, // ATTACKING_BASIC
		{ {EntityState_Boss_Surma::IDLE}, {EntityState_Boss_Surma::RUNNING}, {EntityState_Boss_Surma::ATTACKING_BASIC},{EntityState_Boss_Surma::ATTACKING_DISTANCE}, {EntityState_Boss_Surma::DEAD}, {EntityState_Boss_Surma::DASHI}, {EntityState_Boss_Surma::FASE_CHANGE}, {EntityState_Boss_Surma::IDLE}}, // ATTACKING_DISTANCE
		{ {EntityState_Boss_Surma::IDLE}, {EntityState_Boss_Surma::RUNNING}, {EntityState_Boss_Surma::ATTACKING_BASIC},{EntityState_Boss_Surma::ATTACKING_DISTANCE}, {EntityState_Boss_Surma::DEAD}, {EntityState_Boss_Surma::DASHI}, {EntityState_Boss_Surma::FASE_CHANGE}, {EntityState_Boss_Surma::IDLE}}, // DEAD
		{ {EntityState_Boss_Surma::IDLE}, {EntityState_Boss_Surma::RUNNING}, {EntityState_Boss_Surma::ATTACKING_BASIC},{EntityState_Boss_Surma::ATTACKING_DISTANCE}, {EntityState_Boss_Surma::DEAD}, {EntityState_Boss_Surma::DASHI}, {EntityState_Boss_Surma::FASE_CHANGE}, {EntityState_Boss_Surma::IDLE}}, // DASHI
		{ {EntityState_Boss_Surma::IDLE}, {EntityState_Boss_Surma::RUNNING}, {EntityState_Boss_Surma::ATTACKING_BASIC},{EntityState_Boss_Surma::ATTACKING_DISTANCE}, {EntityState_Boss_Surma::DEAD}, {EntityState_Boss_Surma::DASHI}, {EntityState_Boss_Surma::FASE_CHANGE}, {EntityState_Boss_Surma::IDLE}}, // FASE_CHANGE
		{ {EntityState_Boss_Surma::IDLE}, {EntityState_Boss_Surma::NONE}, {EntityState_Boss_Surma::NONE},{EntityState_Boss_Surma::NONE}, {EntityState_Boss_Surma::NONE}, {EntityState_Boss_Surma::NONE}, {EntityState_Boss_Surma::NONE}, {EntityState_Boss_Surma::IDLE}} // NONE
	};

	EntityState_Boss_Surma currentState = state;
	EntityState_Boss_Surma desiredState = nextState;
	EntityState_Boss_Surma nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(desiredState)].next_state;

};



#endif  