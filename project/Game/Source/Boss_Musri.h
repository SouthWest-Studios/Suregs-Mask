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
	FASE_TWO,
	FASE_DYNIG
};



struct FlechaMusri {
	fPoint direction;
	PhysBody* pbody;
	Timer lifeTimer;
	
};
struct RastroFlechaCargadaMusri {
	iPoint position;
	PhysBody* pbody;
	Timer lifeTimer;
};
struct FlechaCargadaMusri {
	fPoint direction;
	PhysBody* pbody;
	std::vector<RastroFlechaCargadaMusri> rastroGenerado;
	int maxRastro = 40;
	Timer dejarRastroTimer;
	bool flechaRastroTerminado = false;
	
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



	void Fase1(float dt, iPoint playerPos);
	void FaseC(float dt, iPoint playerPos);
	void Fase2(float dt, iPoint playerPos);
	void FaseDying(float dt, iPoint playerPos);

	iPoint GetRandomPosicion(iPoint actualPosition, int distanceLimitInf = 10, int distanceLimitSup = 15);
	
	MapObject* GetCurrentRoom();

public:

	//L02: DONE 2: Declare player parameters
	SDL_Texture* texture = NULL;
	SDL_Texture* arrowTexture = NULL;
	/*SDL_Texture* arrowChargedTexture = NULL;*/
	SDL_Texture* arrowChargedRastroTexture = NULL;
	pugi::xml_node config;
	uint texW, texH;

	//Estadisticas
	float speed;
	b2Vec2 vel;
	//float health;
	//float maxHealth;
	float attackDamage;
	Timer invulnerabilityTimer;

	float viewDistance;
	float attackDistance;


	//Revivir
	bool saveXml = false;

	//Borrar
	int pivotX = 0;
	int pivotY = 0;

private:
	pugi::xml_document configFile;
	pugi::xml_node configNode;

	
	/*Animation attackEmpujarAnim;
	Animation attackFlechasRapidasAnim;
	Animation attackFlechaCargadaAnim;*/

	Animation atqCargadoAnim;
	Animation atqNormalAnim;
	Animation idleAnim;
	Animation cambioFaseAnim;
	Animation runAnim;
	Animation dashAnim;
	Animation atqMeleeAnim;

	Animation flechaNormalAnim;
	Animation flechaCargadaAnim;


	int musri_dash_fx;
	int musri_get_damage_fx;
	
	bool isFacingLeft = false;

	int TSprite;
	int SpriteX;
	int SpriteY;
	int Photowidth;

	int currentPosX;

	PathFinding* path;
	DynArray<iPoint> lastPath;

	Animation* currentAnimation = nullptr;
	Animation* currentFlechaNormalAnimation = nullptr;
	Animation* currentFlechaCargadaAnimation = nullptr;

	EntityState_Boss_Musri state;
	FASE_Musri fase;


	Animation SPosition;
	SDL_Rect* spritePositions;
	SDL_Rect* spriteArrowPositions;

	Timer cambiarPosicionTimer;
	Timer dispararRafagasTimer;
	Timer dispararFlechaRafagaTimer;
	Timer cargaFlechaRafagaTimer;

	Timer habilidadEmpujeTimer;
	Timer habilidadFlechaCargadaTimer;
	Timer habilidadDashInvisibleTimer;

	Timer muriendoseTimer;

	Timer cambioFaseTimer;
	int cambioFaseMS = 5000;


	int habilidadEmpujeCD = 10000;
	int habilidadRafagasCD = 1000;
	int habilidadCargadaCD = 20000;
	int habilidadDashInvisibleCD = 15000;
	int habilidadRafagasRafagaCD = 100;

	int meleeAttackDistance = 3;
	int cambiarPosicionTime = 15000;
	int dispararRafagasTime;
	int numeroRafagasAct = 0;
	int numeroRafagas = 3;
	float flechaRafagaDamage = 25;
	float flechaCargadaDamage = 50;
	float flechaCargadaRastroDamage = 15;
	int numeroDashesAct = 0;
	int numeroDashes = 3;
	int opacidadQuitadaDashes = 70;

	int alphaTexture = 255;
	
	SDL_Rect limitesSala = { 9900 , 2750, 2050, 1000 };
	iPoint movePosition;

	std::vector<FlechaMusri> flechasLanzadas;
	float velocidadFlechas = 100;
	int fuerzaHabilidadEmpuje = 500;

	std::vector<FlechaCargadaMusri> flechasCargadas;


	//Veneno
	float poisonTimer = 0.0f; // Tiempo desde que se aplic� el veneno
	float poisonDuration = 0.0f; // Duraci�n total del veneno
	float poisonTickRate = 0.0f; // Tiempo entre cada tick de da�o de veneno
	float poisonDamage = 0.0f; // Da�o de veneno por tick
	float timeSinceLastTick = 0.0f; // Tiempo desde el �ltimo tick de da�o de veneno
	bool poisoned = false;


	Timer timerRecibirDanioColor;

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