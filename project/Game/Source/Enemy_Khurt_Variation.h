#ifndef __ENEMY_KHURT_VARIATION_H__
#define __ENEMY_KHURT_VARIATION_H__


#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "Player.h"
#include "Physics.h"
#include "App.h"

#include <vector>

struct SDL_Texture;

struct MagicTrail {
	PhysBody* sensor;
	Timer durationTimer;
};

struct Branch_Khurt_variation {
	enum EntityState_Enemy const next_state;
	Branch_Khurt_variation(EntityState_Enemy next) : next_state(next) {}

};

class Enemy_Khurt_Variation : public Entity
{


public:

	Enemy_Khurt_Variation();

	virtual ~Enemy_Khurt_Variation();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void DoNothing(float dt);
	void Chase(float dt, iPoint playerPos);
	void Attack(float dt, iPoint playerPos);
	void Die();
	//bool Khurtfinding(float dt, iPoint playerPos);

	void HandleDigging(float dt, iPoint playerPos);
	void CreateAroMagica();
	void ClearMagicTrails();
	PhysBody* CreateMagicTrailSensor(int x, int y, int width, int height);

	void OnCollision(PhysBody* physA, PhysBody* physB);

	float GetHealth() const;
	void TakeDamage(float damage);

	//Attack
	void Charge(float dt, iPoint playerPos);
	void Stunned(float dt);

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
	//float attackDamage;
	Timer invulnerabilityTimer;

	float viewDistance;
	float attackDistance;

private:
	pugi::xml_document configFile;
	pugi::xml_node configNode;


	Animation idleAnim;
	Animation runAnim;
	Animation underAnim_start;
	Animation underAnim_process;
	Animation underAnim_finish;
	Animation stunAnim;
	Animation attack;
	Animation dieAnim;

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

	//VENENO <----------
	bool firstTimePoisonRecibed = false;
	Timer poisonTimer;
	Timer poisonTickTimer;
	float poisonDuration = 0.0f; // Duración total del veneno
	float poisonTickRate = 0.0f; // Tiempo entre cada tick de da�o de veneno
	float poisonDamage = 0.0f; // Daño de veneno por tick
	bool poisoned = false;
	//VENENO ---------->

	//Charge Attack
	Timer stunTimer;
	Timer chargeTimer;
	bool charging = false;
	bool chargetimer = false;
	bool chargeimpulse = false;
	bool stunned = false;
	bool stunnedtimer = false;
	iPoint Antposition;

	//Dig underground
	Timer digTimer;
	bool digtimer = false;
	bool digging = false;

	std::vector<MagicTrail> magicTrails;

public:

	Branch_Khurt_variation transitionTable[static_cast<int>(EntityState_Enemy::STATE_COUNT)][static_cast<int>(EntityState_Enemy::STATE_COUNT)] = {
		// isMoving               isAttacking			 isDead                isReviving					else					MASK_ATTACK
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




#endif // __ENEMY_KHURT_VARIATION_H__
