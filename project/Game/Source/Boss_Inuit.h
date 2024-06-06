#pragma once
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
	bool BMRfinding(iPoint playerPos);
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
	void resetAnimation();
	bool TimerColdDown(float time);
	bool waveTimerColdDown(float time);
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnEndCollision(PhysBody* physA, PhysBody* physB);
	//Veneno
	void ApplyPoison(int poisonDamage, float poisonDuration, float poisonTickRate);

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
	float attackDamage;
	Timer invulnerabilityTimer;

	float viewDistance;
	float attackDistance;
	int attackTime = 0;
	bool getBossArea = true;
	//Revivir
	Timer atackTimeColdDown;
	Timer timerRecibirDanioColor;
	bool saveXml = false;

private:
	pugi::xml_document configFile;
	pugi::xml_node configNode;

	//physi body
	PhysBody* areaSensor = nullptr;
	PhysBody* atackCube = nullptr;
	PhysBody* atackBMR = nullptr;
	//PhysBody* atackShockWave = nullptr;
	std::unordered_map<int, PhysBody*> shockWaves;


	//animacion
	Animation atk2_boss_inuit;
	Animation atk1_boss_inuit;
	std::vector<Animation> wave_boss_inuit;
	Animation wave_boss_inuit_back;
	Animation idleAnim_boss_inuit;
	Animation boomerang_boss_inuit;
	Animation changeFase_boss_inuit;
	Animation move_inuit;
	Animation idle_sin_inuit;



	bool isFacingLeft = false;
	bool enemyMove = false;
	bool Dead = false;

	iPoint originalPosition;
	iPoint originalWavePosition;
	iPoint bossArea;
	iPoint BMRposition;

	int TSprite;
	int SpriteX;
	int SpriteY;
	int Photowidth;

	int currentPosX;

	PathFinding* path = nullptr;
	DynArray<iPoint> lastPath;

	Animation* currentAnimation = nullptr;
	Animation* currentAnimation1 = nullptr;
	std::vector<Animation*>  currentAnimation2;
	EntityState_Boss_Inuit state;


	Animation SPosition;
	SDL_Rect* spritePositions = nullptr;

	// Audio
	int inuit_get_damage_fx;
	int inuit_melee_attack_fx;
	int inuit_melee_attackAlt_fx;
	int inuit_melee_attackAlt2_fx;
	int inuit_ranged_attack_fx;
	bool inuit_ranged_attack = false;

	FASE fase = FASE::FASE_ONE;
	bool faseTwo = false;
	int faseCount;


	//ataque
	float ataqueTimeClodDown = 0;
	bool inAtack = false;
	bool checkAtackBMR = false;
	float bmrSpeed;
	bool bmrBack = false;
	bool inbmrAtack = false;
	bool atkAnimation = false;
	bool atkTimeReset = false;
	bool startBMR = false;
	bool tocaPared = false;
	int BMRatkSpeed = 2;
	int BMRatkF = 200;
	int atkDistancia;
	bool changeNextAtk =  true;
	BTPDirection playerDireccion;
	std::string printplayerDireccion;
	bool bossReciberDMG = false;


	//shockWave
	bool waveIsMax = false;
	bool waveFinishi = true;
	Timer waveTime;
	float waveTimeClodDown = 0;
	int distance = 0;
	bool goUseWave = false;
	//ulti
	bool ultDef = false;
	bool saveOriginPos = true;
	bool wave0Finishing = false;
	bool wave1Finishing = false;
	bool wave2Finishing = false;
	bool ulti_fishing = false;
	bool inWave = false;
	bool waveTimeStart = false;
	bool useUlt = false;
	bool goUseUlt = false;

	//fase
	float lifeLow40;
	float lifeLow5;
	bool dontMove = false;

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

	int count = 0;

};



#endif  