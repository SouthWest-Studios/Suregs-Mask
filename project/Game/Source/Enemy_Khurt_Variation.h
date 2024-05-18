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


struct Branch_Khurt_variation {
	enum EntityState_Enemy const next_state;
	Branch_Khurt_variation(EntityState_Enemy next) : next_state(next) {}

};

class AroMagica : public Entity {

public:
	AroMagica(iPoint position, float damage, float duration, float tickRate) : Entity(EntityType::ARO_MAGICA), damage(damage), duration(duration), tickRate(tickRate), active(true) {
		this->position = position;

		lifeTimer.Start();
		tickTimer.Start();

		pbodySensor = app->physics->CreateRectangleSensor(position.x, position.y, 32, 32, bodyType::STATIC);
		pbodySensor->entity = this;
		pbodySensor->listener = this;
		pbodySensor->ctype = ColliderType::AROMAGICA;
	}

	~AroMagica() {
		app->physics->GetWorld()->DestroyBody(pbodySensor->body);
	}

	bool Update(float dt) {
		if (lifeTimer.ReadSec() >= duration) {
			active = false;
			return false;
		}

		if (tickTimer.ReadSec() >= tickRate) {
			tickTimer.Start();
			// Aplicar daño al jugador si está en el rastro
		}

		return true;
	}
	void OnCollision(PhysBody* physA, PhysBody* physB) {
		if (physB->ctype == ColliderType::PLAYER) {
			Player* player = (Player*)physB->entity;
			player->TakeDamage(damage);
		}
	}

private:
	float damage;
	float duration;
	float tickRate;
	Timer lifeTimer;
	Timer tickTimer;
	bool active;

	SDL_Texture* texture;
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

	std::vector<AroMagica*> magicTrails;
	Timer AroTimer;
	float aroInterval = 1.0f;

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
