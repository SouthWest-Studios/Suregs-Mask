#ifndef __ENEMY_BOOROK_H__
#define __ENEMY_BOOROK_H__


#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "Player.h"
#include "Physics.h"

struct SDL_Texture;


struct Branch_Boorok {
	enum EntityState const next_state;

};

class Enemy_Boorok : public Entity
{


public:

	Enemy_Boorok();

	virtual ~Enemy_Boorok();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void DoNothing(float dt, iPoint playerPos);
	void Chase(float dt, iPoint playerPos);
	void Attack(float dt);
	void Die();
	bool Boorokfinding(float dt, iPoint playerPos);

	void OnCollision(PhysBody* physA, PhysBody* physB);

	float GetHealth() const;
	void TakeDamage(float damage);

	void Sleeping();

	//Ataque cargado
	void chargeAttack(iPoint playerPos);
	void AreaAttack(iPoint playerPos);

	//VENENO <----------
	void ApplyPoison(int poisonDamage, float poisonDuration, float poisonTickRate);
	void CheckPoison();
	//VENENO ---------->



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
	float healthIncrement;
	float attackDamage;
	Timer invulnerabilityTimer;

	float viewDistance;
	float attackDistance;
	float chargeattackDistance;

private:
	pugi::xml_document configFile;
	pugi::xml_node configNode;


	Animation idleAnim;
	Animation sleepAnim;
	Animation runAnim;
	Animation attackAnim;
	Animation dieAnim;
	Animation chargeAttackAnim;

	int muur_get_damage_fx;

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
	EntityState state;


	Animation SPosition;
	SDL_Rect* spritePositions;

	Timer timerRecibirDanioColor;

	//Sleeping
	Timer recoveryTimer;

	//Charge Attack
	Timer chargeAttackTimer;
	iPoint Antposition;
	float areaattackdamage;
	float chargeAttackMaxRange;

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

	Branch_Boorok transitionTable[static_cast<int>(EntityState::STATE_COUNT)][static_cast<int>(EntityState::STATE_COUNT)] = {
		// isMoving               isAttacking			 isDead                isReviving					else					MASK_ATTACK
		{ {EntityState::RUNNING}, {EntityState::ATTACKING}, {EntityState::DEAD},	 {EntityState::IDLE},	  {EntityState::IDLE},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK}}, // IDLE
		{ {EntityState::RUNNING}, {EntityState::ATTACKING}, {EntityState::DEAD},	 {EntityState::IDLE},	  {EntityState::IDLE},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK}}, // RUNNING
		{ {EntityState::IDLE},	  {EntityState::IDLE},		{EntityState::DEAD},	 {EntityState::IDLE},	  {EntityState::IDLE},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK}}, // ATTACKING
		{ {EntityState::DEAD},	  {EntityState::DEAD},		{EntityState::DEAD},	 {EntityState::IDLE},	  {EntityState::IDLE},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK}}, // DEAD
		{ {EntityState::REVIVING},{EntityState::REVIVING},	{EntityState::DEAD},	 {EntityState::REVIVING}, {EntityState::REVIVING}, {EntityState::REVIVING},			{EntityState::MASK_ATTACK},			{EntityState::MASK_ATTACK}}, // REVIVING
		{ {EntityState::IDLE},	  {EntityState::IDLE},	    {EntityState::DEAD},	 {EntityState::IDLE},	  {EntityState::IDLE},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK}}, // MASK_ATTACK
		{ {EntityState::IDLE},	  {EntityState::IDLE},	    {EntityState::DEAD},	 {EntityState::IDLE},	  {EntityState::IDLE},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK}}, // MASK_ATTACK
		{ {EntityState::IDLE},	  {EntityState::IDLE},	    {EntityState::DEAD},	 {EntityState::IDLE},	  {EntityState::IDLE},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK}} // MASK_ATTACK
	};

	EntityState currentState = state;


	EntityState nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(currentState)].next_state;

};



#endif // __ENEMY_BOOROK_H__