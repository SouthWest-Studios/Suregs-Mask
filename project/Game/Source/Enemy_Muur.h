#ifndef __ENEMY_MUUR_H__
#define __ENEMY_MUUR_H__


#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "Player.h"
#include "Physics.h"

struct SDL_Texture;


struct Branch_Muur {
	enum EntityState const next_state;

};

class Enemy_Muur : public Entity
{


public:

	Enemy_Muur();

	virtual ~Enemy_Muur();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void DoNothing(float dt);
	void Chase(float dt, iPoint playerPos);
	void Attack(float dt);
	void Die();
	bool Muurfinding(float dt, iPoint playerPos);
	void ChargeAttack(float dt);

	void OnCollision(PhysBody* physA, PhysBody* physB);

	float GetHealth() const;
	void TakeDamage(float damage);

	//Veneno
	void ApplyPoison(int poisonDamage, float poisonDuration, float poisonTickRate);

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

private:
	pugi::xml_document configFile;
	pugi::xml_node configNode;


	Animation idleAnim;
	Animation runAnim;
	Animation attackAnim;
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
	EntityState state;


	Animation SPosition;
	SDL_Rect* spritePositions;

	Timer timerRecibirDanioColor;

	//Veneno
	float poisonTimer = 0.0f; // Tiempo desde que se aplico el veneno
	float poisonDuration = 0.0f; // Duracion total del veneno
	float poisonTickRate = 0.0f; // Tiempo entre cada tick de daño de veneno
	float poisonDamage = 0.0f; // Daño de veneno por tick
	float timeSinceLastTick = 0.0f; // Tiempo desde el ultimo tick de da�o de veneno
	bool poisoned = false;

public:

	Branch_Osiris transitionTable[static_cast<int>(EntityState::STATE_COUNT)][static_cast<int>(EntityState::STATE_COUNT)] = {
		// isMoving               isAttacking				isDead				else                MASK_ATTACK
	{ {EntityState::RUNNING}, {EntityState::ATTACKING}, {EntityState::DEAD},	 {EntityState::IDLE},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK}}, // IDLE
	{ {EntityState::RUNNING}, {EntityState::ATTACKING}, {EntityState::DEAD},	 {EntityState::IDLE},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK}}, // RUNNING
	{ {EntityState::IDLE},	  {EntityState::IDLE},		{EntityState::DEAD},	 {EntityState::IDLE},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK}}, // ATTACKING
	{ {EntityState::DEAD},	  {EntityState::DEAD},		{EntityState::DEAD},	 {EntityState::IDLE},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK}}, // DEAD
	{ {EntityState::IDLE},	  {EntityState::IDLE},	    {EntityState::DEAD},	 {EntityState::IDLE},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK}}, // MASK_ATTACK
	{ {EntityState::IDLE},	  {EntityState::IDLE},	    {EntityState::DEAD},	 {EntityState::IDLE},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK}}, // MASK_ATTACK
	{ {EntityState::IDLE},	  {EntityState::IDLE},	    {EntityState::DEAD},	 {EntityState::IDLE},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK},	   {EntityState::MASK_ATTACK}} // MASK_ATTACK
	};

	EntityState currentState = state;


	EntityState nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(currentState)].next_state;

};



#endif // __ENEMY_OSIRIS_H__
