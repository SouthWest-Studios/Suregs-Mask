#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Timer.h"
#include "Fishing.h"
#include "Physics.h"
#include "Map.h"
#include <map>

struct SDL_Texture;


struct Branch {
	enum EntityState const next_state;
	Branch(EntityState next) : next_state(next) {}
};

enum RoomType {
	ROOM_SQUARE,
	ROOM_RECTANGLE_W,
	ROOM_RECTANGLE_H,
	ROOM_L_SHAPED,
	ROOM_UNKNOWN
};

enum class Mask{
	NOMASK,
	MASK0,
	MASK1,
	MASK2,
	MASK3
};

struct Stats {
	float maxHealth;
	float currentHealth;
	float movementSpeed;
	float attackSpeed;
	float attackDamage;
	float armor;
};

struct MaskStats {
	//Modificadores
	float maxHealthModifier;
	float movementSpeedModifier;
    float attackDamageModifier;
	float attackSpeedModifier;

	//Habilidades
	float maskDamage;
	float maskCoolDown;
	bool firstTimeUsed;

	//Mask0
	int numberLightning;

	//Mask1
	float rangeBallModifier;

	//Mask2
	float distanceDashModifier;
	Timer invisibilityTimer;
	float invisibilityDuration;

	//Mask3


	//Venenos
	int poisonDamage;
	float poisonDuration;
	float poisonTickRate;
};

struct PassiveStats {
	float damageBoost;
	float rangeBoost;
	float dashBoost;
	Timer invisibilityTimer;
	float invisibilityDuration;
};

class Player : public Entity
{


public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void DoNothing(float dt);
	void Run(float dt);
	void Dashi(float dt);
	void Attack(float dt);

	float GetRealMovementSpeed() const;

	//Branch transitionTable[static_cast<int>(EntityState::STATE_COUNT)][static_cast<int>(EntityState::STATE_COUNT)];
	// L07 DONE 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);
    
    Entity* GetEnemyWithHighestHealthWithinRadius(iPoint position, int radius);
	
	//Funciones ataques mascara
	void CastLightning();
	void CastMultipleLightnings();
	void AreaAttack(float dt);
	void DashAttack(float dt);

	//Mascaras
	void UnequipMasks();
	void ChangeMask();
	void EquipPrimaryMask(Mask mask);
	void EquipSecondaryMask(Mask mask);
	void ApplyPoison(Entity* entity);
	void SetInvisible();
	void SetPassiveInvisible();

	Mask* GetPrimaryMask();
	Mask* GetSecondaryMask();
	
	MaskStats* GetMaskStats(Mask mask);

	//Estadísticas

    Stats baseStats;
    Stats currentStats;
	
	std::map<Mask, std::map<int, MaskStats>> maskStats; 
	std::map<Mask, std::map<int, PassiveStats>> passiveStats;
	std::map<Mask, int> maskLevels;
	//MaskStats maskStats[5];
	//PassiveStats passiveStats[5];

private:
	void CameraMovement(float dt);
	void GodMode(float dt);
	void PlayerMovement(float dt);
	void FishingDirecction(bool verticalMovement, bool horizontalMovement);
	void MaskAttack(float dt);
	void TakeDamage(float damage);
	void stateMachine(float dt);

public:

	//L02: DONE 2: Declare player parameters
	float speed = 0.2f;
	SDL_Texture* texture = NULL;
	
	uint texW, texH;

	//Audio fx
	int run_fx;
	int dash_fx;
	int basic_combo_attack1_fx;
	int player_get_damage_fx;

	Animation* currentAnimation = nullptr;
	EntityState state;

	//Animation
	Animation SPosition;
	SDL_Rect* spritePositions;
	pugi::xml_node config;
	int TSprite;
	int SpriteX;
	int SpriteY;
	int Photowidth;

	bool inAnimation = false;
	//Dash
	bool playerInDashi = false;
	int rodar_PlayerPosition;
	int rodar_PotisionX;
	int currentPosX;
	PhysBody* dashCollision = nullptr;
	b2Vec2 velocity = b2Vec2(0, 0);
	
	float speedDash = 3;

	//Ataque
	int attackWidth = 50; 
    int attackHeight = 50;
	bool isAttacking = false;
	bool basicAttackDealed = false;
	PhysBody* attackSensor = nullptr;
	Timer damageTimer;
	float damageCooldown = 0.2f;

	Timer collisionAttackTimer;
	
	//Pasiva de la mascara 1
	PhysBody* mask1PassiveSensor = nullptr;

	//Mascara
	Mask primaryMask;
	Mask secondaryMask;

	bool isAttackingMask = true;
	PhysBody* mask1AttackSensor = nullptr;
	int attackMask1Width = 300;  
    int attackMask1Height = 300; 
	
	Timer collisionMask1Timer;

	Timer timerChangeMask;
	const int changeMaskCooldown = 5000; //5s
	
	bool isInvisible = false;

	//Fishing
	//New//
	bool playermove = false;
	Direction player_Direction;

	//Player move
	bool pressingUp;
	bool pressingDown;
	bool pressingLeft;
	bool pressingRight;

	bool isFacingLeft = false;

	//Camera
	MapObject* GetCurrentRoom();
	int clamp(int val, int min, int max);
	RoomType roomType;
	void DetermineRoomType(MapObject* room);

private:
	Animation idleAnim;
	Animation runAnim;
	Animation dashiAnim;

	bool isDashing = false;

	bool godmode = false;

	bool camaralibre = false;

	int cdTimerDashMS;

	Timer timerDash;
	Timer cdTimerDash; 

	int cdTimerAttackMS;
	Timer timerAttack;

	fPoint lastMovementDirection;

	int cdTimerMaskAttackMS = 1000;
	Timer timerMaskAttack;

	Timer timerBetweenLightnings;

public:

	Branch transitionTable[static_cast<int>(EntityState::STATE_COUNT)][static_cast<int>(EntityState::STATE_COUNT)] = {

	//		IDLE					RUNNING					ATTACKING					DEAD				REVIVING				MASK_ATTACK					  DASHI				   NONE
	{ {EntityState::IDLE}, {EntityState::RUNNING}, {EntityState::ATTACKING}, {EntityState::DEAD}, {EntityState::REVIVING}, {EntityState::MASK_ATTACK}, {EntityState::NONE}, {EntityState::IDLE}}, // IDLE
	{ {EntityState::IDLE}, {EntityState::RUNNING}, {EntityState::ATTACKING}, {EntityState::DEAD}, {EntityState::REVIVING}, {EntityState::MASK_ATTACK}, {EntityState::DASHI}, {EntityState::IDLE}}, // RUNNING
	{ {EntityState::IDLE}, {EntityState::RUNNING}, {EntityState::ATTACKING}, {EntityState::DEAD}, {EntityState::REVIVING}, {EntityState::MASK_ATTACK}, {EntityState::DASHI}, {EntityState::IDLE}}, // ATTACKING
	{ {EntityState::IDLE}, {EntityState::RUNNING}, {EntityState::ATTACKING}, {EntityState::DEAD}, {EntityState::REVIVING}, {EntityState::MASK_ATTACK}, {EntityState::DASHI}, {EntityState::IDLE}}, // DEAD
	{ {EntityState::IDLE}, {EntityState::RUNNING}, {EntityState::ATTACKING}, {EntityState::DEAD}, {EntityState::REVIVING}, {EntityState::MASK_ATTACK}, {EntityState::DASHI}, {EntityState::IDLE}}, // REVIVING
	{ {EntityState::IDLE}, {EntityState::RUNNING}, {EntityState::ATTACKING}, {EntityState::DEAD}, {EntityState::REVIVING}, {EntityState::MASK_ATTACK}, {EntityState::DASHI}, {EntityState::IDLE}}, // MASK_ATTACK
	{ {EntityState::IDLE}, {EntityState::RUNNING}, {EntityState::ATTACKING}, {EntityState::DEAD}, {EntityState::REVIVING}, {EntityState::MASK_ATTACK}, {EntityState::DASHI}, {EntityState::IDLE}}, // DASHI
	{ {EntityState::IDLE}, {EntityState::NONE}, {EntityState::NONE}, {EntityState::NONE}, {EntityState::NONE}, {EntityState::NONE}, {EntityState::NONE}, {EntityState::IDLE}} // NONE
	};

	EntityState currentState = state;
	EntityState desiredState = nextState; 
	EntityState nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(desiredState)].next_state;
};



#endif // __PLAYER_H__
