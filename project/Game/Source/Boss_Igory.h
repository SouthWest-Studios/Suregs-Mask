#pragma once
#pragma once
#ifndef __BOSS_IGORY_H__
#define __BOSS_IGORY_H__


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
#include <unordered_map>

struct SDL_Texture;


struct Branch_Igory {
	enum EntityState_Boss_Igory const next_state;
	Branch_Igory(EntityState_Boss_Igory next) : next_state(next) {}
};

enum class FASE_Igory
{
	FASE_ONE,
	FASE_TWO,
	FASE_THREE
};

class Boss_Igory : public Entity
{


public:

	Boss_Igory();

	virtual ~Boss_Igory();

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
	float GetHealth() const;
	void TakeDamage(float damage);
	void stateMachine(float dt, iPoint playerPos);
	bool TimerColdDown(float time);
	bool GeneraColdDown(float time);
	void OnCollision(PhysBody* physA, PhysBody* physB);
	//void OnEndCollision(PhysBody* physA, PhysBody* physB);
	void resetAnimation();
	void showAnimation();
	bool AtqColdDown();
	void takeHit();


	//Veneno
	void ApplyPoison(int poisonDamage, float poisonDuration, float poisonTickRate);

	MapObject* GetCurrentRoom();

public:

	//L02: DONE 2: Declare player parameters
	SDL_Texture* texture = nullptr;
	pugi::xml_node config;
	uint texW, texH;

	//Estadisticas
	float speed;
	b2Vec2 vel;
	float health;
	float maxHealth;
	float attackDamage;

	bool saveXml = false;

	float viewDistance;
	float attackDistance;
	int attackTime;
	bool getBossArea = true;

	Timer timerRecibirDanioColor;

	
	//Start
	bool playerInFight = false;
	bool isDead = false;

	//Dialogo
	pugi::xml_document dialogoFile;
	pugi::xml_node dialogNode;
	bool dialogoMostrado = false;
	bool startDialogo = false;
	bool startSelecion = false;
	bool closeFinalSelecion = false;
	bool dieDefinit = false;
	int seleccionFinalPersonaje = -1;
	Timer startDialogoTimer;

	//Kill padre
	bool cankillPadre = false;
	bool killPadre = false;
	bool deletePadre = false;
	int padreTranparente = 255;
	//GuiControl* control = nullptr;
	//unirPadre
	bool unirPadre = false;
	//tiempo de genera enemigo
	Timer generaTimeColdDown;
	Timer curaTimer;
private:
	pugi::xml_document configFile;
	pugi::xml_node configNode;

	//physi body
	PhysBody* areaSensor = nullptr;
	PhysBody* atackCube = nullptr;

	Emitter* particulaFase2 = nullptr;
	Emitter* particulaFase3 = nullptr;
	bool partFase2Created = false;
	bool partFase3Created = false;

	//animacion
	Animation atqCargado_fase2_boss_Igory;
	Animation atqCargado_fase1_boss_Igory;
	Animation walk_boss_Igory;
	Animation cura_boss_Igory;
	Animation dash_idle_boss_Igory;
	Animation dash_inDashi_boss_Igory;
	Animation dash_DashiAtq_boss_Igory;
	Animation atq1_boss_Igory;
	Animation geneSure_boss_Igory;
	Animation idle_boss_Igory;
	Animation atq2_boss_Igory;
	Animation atq3_boss_Igory;
	Animation dead_boss_Igory;
	Animation hit_boss_Igory;
	Animation reviver_boss_Igory;


	bool isFacingLeft = false;

	iPoint originalPosition;
	iPoint originalWavePosition;
	iPoint bossArea;

	int TSprite;
	int SpriteX;
	int SpriteY;
	int Photowidth;

	int currentPosX;

	PathFinding* path = nullptr;
	DynArray<iPoint> lastPath;

	Animation* currentAnimation = nullptr;
	EntityState_Boss_Igory state;


	Animation SPosition;
	SDL_Rect* spritePositions = nullptr;

	FASE_Igory fase;

	// Audio
	int father_get_damage_fx;
	int father_get_damageAlt_fx;
	int father_get_damageAlt2_fx;
	int father_melee_attack_fx;
	int father_melee_attackAlt_fx; 
	int father_generate_sureg_fx;
	bool father_get_hit = false;

	Timer getHitTimer;

	


	//ataque
	float ataqueTimeClodDown = 0;
	bool inAtack = false;
	bool ataqColdDown = false;
	bool startColdDown = false;
	bool checkColdDown = false;
	bool atqGoNext = true;
	Timer atackTimeColdDown;

	//fase
	float lifeLow80;
	float lifeLow40;
	bool dontMove = false;
	bool faseTwo = false;
	bool faseThree = false;
	bool faseAni = false;

	//generaSureg
	bool showSuregAni = false;
	bool inSuregAni = false;
	bool goColdDown = false;


	//Takehit
	bool inTakeHit = false;

	//ataqueDashi
	int atqDashQuali = 0;
	bool inAtqDashi = false;
	bool inIdleDashi = false;
	bool inDashDashi = false;
	bool inAtaqueDashi = false;
	bool empujaPlayer = false;
	int fuerzaHabilidadEmpuje = 300;
	iPoint getPlayerPosition;
	Timer inDashiTime;
	Timer habilidadEmpujeTimer;

	//Curar
	bool inCurar = false;
	bool goCurar = false;
	bool shieldBroken = false;
	float shield = 1500;
	bool stun = false;
	
	Timer shieldTimer;
	Timer healTimer;
	Timer stunTimer;


	//Veneno
	float poisonTimer = 0.0f; // Tiempo desde que se aplic� el veneno
	float poisonDuration = 0.0f; // Duraci�n total del veneno
	float poisonTickRate = 0.0f; // Tiempo entre cada tick de da�o de veneno
	float poisonDamage = 0.0f; // Da�o de veneno por tick
	float timeSinceLastTick = 0.0f; // Tiempo desde el �ltimo tick de da�o de veneno
	bool poisoned = false;

public:

	Branch_Igory transitionTable[static_cast<int>(EntityState_Boss_Igory::STATE_COUNT)][static_cast<int>(EntityState_Boss_Igory::STATE_COUNT)] = {
		//			IDLE,								RUNNING,							ATTACKING_BASIC,							ATTACKING_CHARGE,						ATTACKING_DASHI,							DEAD,							FASE_CHANGE,						TAKEHIT,							GENERATESUREG								HEAL,							NONE,
		{ {EntityState_Boss_Igory::IDLE}, {EntityState_Boss_Igory::RUNNING}, {EntityState_Boss_Igory::ATTACKING_BASIC},{EntityState_Boss_Igory::ATTACKING_CHARGE}, {EntityState_Boss_Igory::ATTACKING_DASHI}, {EntityState_Boss_Igory::DEAD}, {EntityState_Boss_Igory::FASE_CHANGE}, {EntityState_Boss_Igory::TAKEHIT},{EntityState_Boss_Igory::GENERATESUREG}, {EntityState_Boss_Igory::HEAL}, {EntityState_Boss_Igory::IDLE}}, // IDLE
		{ {EntityState_Boss_Igory::IDLE}, {EntityState_Boss_Igory::RUNNING}, {EntityState_Boss_Igory::ATTACKING_BASIC},{EntityState_Boss_Igory::ATTACKING_CHARGE}, {EntityState_Boss_Igory::ATTACKING_DASHI}, {EntityState_Boss_Igory::DEAD}, {EntityState_Boss_Igory::FASE_CHANGE}, {EntityState_Boss_Igory::TAKEHIT},{EntityState_Boss_Igory::GENERATESUREG}, {EntityState_Boss_Igory::HEAL}, {EntityState_Boss_Igory::IDLE}}, // RUNNING
		{ {EntityState_Boss_Igory::IDLE}, {EntityState_Boss_Igory::RUNNING}, {EntityState_Boss_Igory::ATTACKING_BASIC},{EntityState_Boss_Igory::ATTACKING_CHARGE}, {EntityState_Boss_Igory::ATTACKING_DASHI}, {EntityState_Boss_Igory::DEAD}, {EntityState_Boss_Igory::FASE_CHANGE}, {EntityState_Boss_Igory::TAKEHIT}, {EntityState_Boss_Igory::GENERATESUREG},{EntityState_Boss_Igory::HEAL}, {EntityState_Boss_Igory::IDLE}}, // ATTACKING_BASIC
		{ {EntityState_Boss_Igory::IDLE}, {EntityState_Boss_Igory::RUNNING}, {EntityState_Boss_Igory::ATTACKING_BASIC},{EntityState_Boss_Igory::ATTACKING_CHARGE}, {EntityState_Boss_Igory::ATTACKING_DASHI}, {EntityState_Boss_Igory::DEAD}, {EntityState_Boss_Igory::FASE_CHANGE}, {EntityState_Boss_Igory::TAKEHIT}, {EntityState_Boss_Igory::GENERATESUREG},{EntityState_Boss_Igory::HEAL}, {EntityState_Boss_Igory::IDLE}}, // ATTACKING_CHARGE
		{ {EntityState_Boss_Igory::IDLE}, {EntityState_Boss_Igory::RUNNING}, {EntityState_Boss_Igory::ATTACKING_BASIC},{EntityState_Boss_Igory::ATTACKING_BASIC}, {EntityState_Boss_Igory::ATTACKING_BASIC}, {EntityState_Boss_Igory::DEAD}, {EntityState_Boss_Igory::FASE_CHANGE}, {EntityState_Boss_Igory::TAKEHIT}, {EntityState_Boss_Igory::GENERATESUREG},{EntityState_Boss_Igory::HEAL}, {EntityState_Boss_Igory::IDLE}}, // ATTACKING_DASHI
		{ {EntityState_Boss_Igory::NONE}, {EntityState_Boss_Igory::NONE}, {EntityState_Boss_Igory::NONE},{EntityState_Boss_Igory::NONE}, {EntityState_Boss_Igory::NONE}, {EntityState_Boss_Igory::DEAD}, {EntityState_Boss_Igory::NONE}, {EntityState_Boss_Igory::NONE}, {EntityState_Boss_Igory::NONE},{EntityState_Boss_Igory::NONE}, {EntityState_Boss_Igory::IDLE}}, // DEAD
		{ {EntityState_Boss_Igory::IDLE}, {EntityState_Boss_Igory::RUNNING}, {EntityState_Boss_Igory::ATTACKING_BASIC},{EntityState_Boss_Igory::ATTACKING_BASIC}, {EntityState_Boss_Igory::ATTACKING_BASIC}, {EntityState_Boss_Igory::DEAD}, {EntityState_Boss_Igory::FASE_CHANGE}, {EntityState_Boss_Igory::TAKEHIT},{EntityState_Boss_Igory::GENERATESUREG}, {EntityState_Boss_Igory::HEAL}, {EntityState_Boss_Igory::IDLE}}, // FASE_CHANGE
		{ {EntityState_Boss_Igory::IDLE}, {EntityState_Boss_Igory::RUNNING}, {EntityState_Boss_Igory::ATTACKING_BASIC},{EntityState_Boss_Igory::ATTACKING_CHARGE}, {EntityState_Boss_Igory::ATTACKING_DASHI}, {EntityState_Boss_Igory::DEAD}, {EntityState_Boss_Igory::FASE_CHANGE}, {EntityState_Boss_Igory::TAKEHIT}, {EntityState_Boss_Igory::GENERATESUREG},{EntityState_Boss_Igory::HEAL}, {EntityState_Boss_Igory::IDLE}}, // TAKEHIT
		{ {EntityState_Boss_Igory::IDLE}, {EntityState_Boss_Igory::RUNNING}, {EntityState_Boss_Igory::ATTACKING_BASIC},{EntityState_Boss_Igory::ATTACKING_CHARGE}, {EntityState_Boss_Igory::ATTACKING_DASHI}, {EntityState_Boss_Igory::DEAD}, {EntityState_Boss_Igory::FASE_CHANGE}, {EntityState_Boss_Igory::TAKEHIT}, {EntityState_Boss_Igory::GENERATESUREG},{EntityState_Boss_Igory::HEAL}, {EntityState_Boss_Igory::IDLE}}, // GENERATESUREG
		{ {EntityState_Boss_Igory::IDLE}, {EntityState_Boss_Igory::RUNNING}, {EntityState_Boss_Igory::ATTACKING_BASIC},{EntityState_Boss_Igory::ATTACKING_CHARGE}, {EntityState_Boss_Igory::ATTACKING_DASHI}, {EntityState_Boss_Igory::DEAD}, {EntityState_Boss_Igory::FASE_CHANGE}, {EntityState_Boss_Igory::TAKEHIT}, {EntityState_Boss_Igory::GENERATESUREG},{EntityState_Boss_Igory::HEAL}, {EntityState_Boss_Igory::IDLE}}, // HEAL
		{ {EntityState_Boss_Igory::IDLE}, {EntityState_Boss_Igory::NONE}, {EntityState_Boss_Igory::NONE},{EntityState_Boss_Igory::NONE}, {EntityState_Boss_Igory::NONE}, {EntityState_Boss_Igory::NONE}, {EntityState_Boss_Igory::NONE}, {EntityState_Boss_Igory::NONE},{EntityState_Boss_Igory::NONE}, {EntityState_Boss_Igory::NONE}, {EntityState_Boss_Igory::IDLE}} // NONE
	};


	EntityState_Boss_Igory currentState = state;
	EntityState_Boss_Igory desiredState = nextState;
	EntityState_Boss_Igory nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(desiredState)].next_state;

};



#endif  