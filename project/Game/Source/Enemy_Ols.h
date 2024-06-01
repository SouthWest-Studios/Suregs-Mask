#ifndef __ENEMY_OLS_H__
#define __ENEMY_OLS_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "Player.h"
#include "Physics.h"
#include "Item_Garra.h"
#include "Particle.h"
#include "ParticlePool.h"
#include "Emitter.h"

struct SDL_Texture;


struct Branch_Ols {
	enum EntityState_Enemy const next_state;
	Branch_Ols(EntityState_Enemy next) : next_state(next) {}
};

class Enemy_Ols : public Entity
{


public:

	Enemy_Ols();

	virtual ~Enemy_Ols();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void DoNothing(float dt);
	void Chase(float dt, iPoint playerPos);
	void Attack(float dt, iPoint playerPos);
	void Die();
	bool Olsfinding(float dt);
	bool Flee(float dt);

	float GetHealth() const;

	void TakeDamage(float damage);

	void stateMachine(float dt, iPoint playerPos);

	void ApplyPoison(int poisonDamage, float poisonDuration, float poisonTickRate);
	void CheckPoison();

	// L07 DONE 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);

	void SetPlayer(Player* player);

	void UpdateAttackSensor(float dt);

	MapObject* GetCurrentRoom();
	void DestroyProjectile();
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
	//float attackDamage;
	Timer invulnerabilityTimer;

	float viewDistance;
	float attackDistance;
	float minDistance;

	Animation* currentAnimation = nullptr;
	EntityState_Enemy state;


	Animation SPosition;
	SDL_Rect* spritePositions;

	int TSprite;
	int SpriteX;
	int SpriteY;
	int Photowidth;

	int currentPosX;

	PathFinding* path;
	Player* player;
	const DynArray<iPoint>* lastPath;

	//Ataque
	float attackRange;
    float projectileSpeed; 
	float projectileDamage;
	PhysBody* attackSensor = nullptr;
	SDL_Texture* projectileTexture = nullptr;
	iPoint projectilePosition;
	Timer attackSensorTimer;
	Timer attackCooldownTimer;
	bool canAttack = true;

	bool isFacingLeft = false;
	
private:
	Animation idleAnim;
	Animation runAnim;
	Animation attackAnim;
	Animation dieAnim;

	int ols_attack_fx;
	int ols_get_damage_fx;
	int ols_get_damageAlt_fx;
	int ols_get_damageAlt2_fx;
	int ols_death_fx;

	Emitter* blood;

	Item_Garra* garra;
	pugi::xml_document configFile;
	pugi::xml_node configNode;

	Timer timerRecibirDanioColor;

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

	Branch_Ols transitionTable[static_cast<int>(EntityState_Enemy::STATE_COUNT)][static_cast<int>(EntityState_Enemy::STATE_COUNT)] = {
		//		IDLE						RUNNING								ATTACKING					DEAD						REVIVING							DASHI						NONE
		{ {EntityState_Enemy::IDLE}, {EntityState_Enemy::RUNNING}, {EntityState_Enemy::ATTACKING}, {EntityState_Enemy::DEAD}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::IDLE}}, // IDLE
		{ {EntityState_Enemy::IDLE}, {EntityState_Enemy::RUNNING}, {EntityState_Enemy::ATTACKING}, {EntityState_Enemy::DEAD}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::IDLE}}, // RUNNING
		{ {EntityState_Enemy::IDLE}, {EntityState_Enemy::RUNNING}, {EntityState_Enemy::ATTACKING}, {EntityState_Enemy::DEAD}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::IDLE}}, // ATTACKING
		{ {EntityState_Enemy::IDLE}, {EntityState_Enemy::RUNNING}, {EntityState_Enemy::ATTACKING}, {EntityState_Enemy::DEAD}, {EntityState_Enemy::REVIVING}, {EntityState_Enemy::NONE}, {EntityState_Enemy::IDLE}}, // DEAD
		{ {EntityState_Enemy::IDLE}, {EntityState_Enemy::RUNNING}, {EntityState_Enemy::ATTACKING}, {EntityState_Enemy::DEAD}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::IDLE}}, // REVIVING
		{ {EntityState_Enemy::IDLE}, {EntityState_Enemy::RUNNING}, {EntityState_Enemy::ATTACKING}, {EntityState_Enemy::DEAD}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::IDLE}}, // DASHI
		{ {EntityState_Enemy::IDLE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE}, {EntityState_Enemy::NONE},  {EntityState_Enemy::IDLE}} // NONE
	};

	/*EntityState_Enemy currentState = state;
	EntityState_Enemy desiredState = nextState;
	EntityState_Enemy nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(desiredState)].next_state;*/

	EntityState_Enemy currentState;
	EntityState_Enemy desiredState;
	EntityState_Enemy nextState;

};



#endif // __ENEMY_OLS_H__
