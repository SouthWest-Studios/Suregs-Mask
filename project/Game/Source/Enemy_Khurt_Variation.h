#ifndef __ENEMY_KHURT_VARIATION_H__
#define __ENEMY_KHURT_VARIATION_H__


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
struct SDL_Texture;


struct Branch_Khurt_Variation {
	enum EntityState_Khurt const next_state;
	Branch_Khurt_Variation(EntityState_Khurt next) : next_state(next) {}

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
	void Attack(float dt);
	void Charge(float dt, iPoint playerPos);
	void Stunned(float dt);
	void DigUnderground();
	void MoveUnderground(float dt, iPoint playerPos);
	void DigOut(float dt, iPoint playerPos);
	void Die();
	bool Khurtfinding(float dt, iPoint playerPos);

	void OnCollision(PhysBody* physA, PhysBody* physB);

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
	//float attackDamage;
	Timer invulnerabilityTimer;

	float viewDistance;
	float attackDistance;
	float chargeAttackDistance;

private:
	pugi::xml_document configFile;
	pugi::xml_node configNode;

	Emitter* blood = nullptr;

	Animation idleAnim;
	Animation runAnim;
	Animation underAnim_start;
	Animation underAnim_process;
	Animation underAnim_end;
	Animation chargeAnim;
	Animation stunAnim;
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
	EntityState_Khurt state;


	Animation SPosition;
	SDL_Rect* spritePositions;

	Timer timerRecibirDanioColor;

	// Audio
	int khurt_get_damage_fx;
	int khurt_get_damageAlt_fx;

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
	Timer timechargingTimer;
	bool charging;
	bool stunned;
	iPoint Antposition;

	//Dig underground
	Timer digTimer;
	bool digging = false;

public:

	Branch_Khurt_Variation transitionTable[static_cast<int>(EntityState_Khurt::STATE_COUNT)][static_cast<int>(EntityState_Khurt::STATE_COUNT)] = {

		{ {EntityState_Khurt::IDLE}, {EntityState_Khurt::RUNNING}, {EntityState_Khurt::ATTACKING},	 {EntityState_Khurt::DEAD},	  {EntityState_Khurt::STUNNED},	   {EntityState_Khurt::DIGGING_UNDERGROUND},	   {EntityState_Khurt::MOVING_UNDERGROUND},	   {EntityState_Khurt::DIGGING_OUT},        {EntityState_Khurt::IDLE}},  // IDLE
		{ {EntityState_Khurt::IDLE}, {EntityState_Khurt::RUNNING}, {EntityState_Khurt::ATTACKING},	 {EntityState_Khurt::DEAD},	  {EntityState_Khurt::STUNNED},	   {EntityState_Khurt::DIGGING_UNDERGROUND},	   {EntityState_Khurt::NONE},	               {EntityState_Khurt::DIGGING_OUT},        {EntityState_Khurt::IDLE}},  // RUNNING
		{ {EntityState_Khurt::IDLE}, {EntityState_Khurt::RUNNING}, {EntityState_Khurt::ATTACKING},	 {EntityState_Khurt::DEAD},	  {EntityState_Khurt::STUNNED},	   {EntityState_Khurt::DIGGING_UNDERGROUND},	   {EntityState_Khurt::MOVING_UNDERGROUND},	   {EntityState_Khurt::DIGGING_OUT},        {EntityState_Khurt::IDLE}},  // ATTACKING
		{ {EntityState_Khurt::IDLE}, {EntityState_Khurt::RUNNING}, {EntityState_Khurt::ATTACKING},	 {EntityState_Khurt::DEAD},	  {EntityState_Khurt::STUNNED},	   {EntityState_Khurt::DIGGING_UNDERGROUND},	   {EntityState_Khurt::MOVING_UNDERGROUND},	   {EntityState_Khurt::DIGGING_OUT},        {EntityState_Khurt::IDLE}},  // DEAD
		{ {EntityState_Khurt::IDLE}, {EntityState_Khurt::RUNNING}, {EntityState_Khurt::ATTACKING},	 {EntityState_Khurt::DEAD},	  {EntityState_Khurt::STUNNED},	   {EntityState_Khurt::DIGGING_UNDERGROUND},	   {EntityState_Khurt::MOVING_UNDERGROUND},	   {EntityState_Khurt::DIGGING_OUT},        {EntityState_Khurt::IDLE}},  // STUNNED
		{ {EntityState_Khurt::IDLE}, {EntityState_Khurt::RUNNING}, {EntityState_Khurt::ATTACKING},	 {EntityState_Khurt::DEAD},	  {EntityState_Khurt::STUNNED},	   {EntityState_Khurt::DIGGING_UNDERGROUND},	   {EntityState_Khurt::MOVING_UNDERGROUND},	   {EntityState_Khurt::DIGGING_OUT},        {EntityState_Khurt::IDLE}},  // DIGGING_UNDERGROUND
		{ {EntityState_Khurt::IDLE}, {EntityState_Khurt::RUNNING}, {EntityState_Khurt::ATTACKING},	 {EntityState_Khurt::DEAD},	  {EntityState_Khurt::STUNNED},	   {EntityState_Khurt::DIGGING_UNDERGROUND},	   {EntityState_Khurt::MOVING_UNDERGROUND},	   {EntityState_Khurt::DIGGING_OUT},        {EntityState_Khurt::IDLE}},  // MOVING_UNDERGROUND
		{ {EntityState_Khurt::IDLE}, {EntityState_Khurt::RUNNING}, {EntityState_Khurt::ATTACKING},	 {EntityState_Khurt::DEAD},	  {EntityState_Khurt::STUNNED},	   {EntityState_Khurt::DIGGING_UNDERGROUND},	   {EntityState_Khurt::MOVING_UNDERGROUND},	   {EntityState_Khurt::DIGGING_OUT},        {EntityState_Khurt::IDLE}},  // DIGGING_OUT
		{ {EntityState_Khurt::IDLE}, {EntityState_Khurt::NONE},    {EntityState_Khurt::NONE},	     {EntityState_Khurt::NONE},	  {EntityState_Khurt::NONE},	   {EntityState_Khurt::NONE},	                   {EntityState_Khurt::NONE},	               {EntityState_Khurt::NONE},               {EntityState_Khurt::IDLE}},  // NONE
	};

	EntityState_Khurt currentState;
	EntityState_Khurt desiredState;
	EntityState_Khurt nextState;


};



#endif // __ENEMY_KHURT_VARIATION_H__
