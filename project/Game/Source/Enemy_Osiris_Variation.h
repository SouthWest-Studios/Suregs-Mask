#ifndef __ENEMY_OSIRIS_VARIATION_H__
#define __ENEMY_OSIRIS_VARIATION_H__


#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "Player.h"
#include "Physics.h"

struct SDL_Texture;


struct Branch_Osiris_Variation {
	enum EntityState_Enemy const next_state;
	Branch_Osiris_Variation(EntityState_Enemy next) : next_state(next) {}
};


class Enemy_Osiris_Variation : public Entity
{


public:

	Enemy_Osiris_Variation();

	virtual ~Enemy_Osiris_Variation();

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
	bool Osirisfinding(float dt, iPoint playerPos);

	// L07 DONE 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);

	void SetPlayer(Player* player);

	float GetHealth() const;
	void TakeDamage(float damage);
	void stateMachine(float dt, iPoint playerPos);

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
	Timer invulnerabilityTimer;

	float viewDistance;
	float attackDistance;

	bool isFacingLeft = false;
	Animation* currentAnimation = nullptr;
	//Revivir



private:
	pugi::xml_document configFile;
	pugi::xml_node configNode;


	Animation idleAnim;
	Animation runAnim;
	Animation attackAnim;
	Animation dieAnim;
	Animation takeDamageAnim;
	Animation reviveAnim;


	int osiris_get_damage_fx;
	int osiris_death_fx;


	iPoint originalPosition;

	bool isDead = false;
	bool isReviving = false;
	bool hasRevived = false;
	bool tempo = false;
	Timer reviveTimer;

	const float reviveTime = 5.0f;

	int TSprite;
	int SpriteX;
	int SpriteY;
	int Photowidth;

	int currentPosX;

	PathFinding* path;
	DynArray<iPoint> lastPath;

	EntityState_Enemy state;


	Animation SPosition;
	SDL_Rect* spritePositions;

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

	Branch_Osiris_Variation transitionTable[static_cast<int>(EntityState_Enemy::STATE_COUNT)][static_cast<int>(EntityState_Enemy::STATE_COUNT)] = {
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




#endif // __ENEMY_OSIRIS_VARIATION_H__