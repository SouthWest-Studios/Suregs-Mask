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
#include "Particle.h"
#include "ParticlePool.h"
#include "Emitter.h"

struct SDL_Texture;


struct Branch {
	enum EntityStatePlayer const next_state;
	Branch(EntityStatePlayer next) : next_state(next) {}
};

enum RoomType {
	ROOM_SQUARE,
	ROOM_RECTANGLE_W,
	ROOM_RECTANGLE_H,
	ROOM_L_DL,
	ROOM_L_DR,
	ROOM_L_UL,
	ROOM_L_UR,
	ROOM_L_D,
	ROOM_L_U,
	ROOM_L_L,
	ROOM_L_R,
	ROOM_UNKNOWN
};

enum class Mask {
	NOMASK,
	MASK0,
	MASK1,
	MASK2,
	MASK3
};

enum class Branches {
	Modifiers,
	Rama1,
	Rama2,
	Rama3,
	Rama4
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
	Timer maskCoolDownTimer;
	float originalmaskCoolDown;
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
	float durationTime;
	float maxActiveHealthModifier;
	float maxActiveDamageModifier;

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
	float velocityBoost;
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
	void Dead();

	//Animacion
	void ResetAnimacion();
	void TakeDamage(float damage);

	float GetRealMovementSpeed() const;

	//Pociones
	void EfectoPociones(float dt);
	void Regenpocion(float dt);
	void Speedpocion(float dt);
	void Damagepocion(float dt);
	void UsePotion();

	//Branch transitionTable[static_cast<int>(EntityStatePlayer::STATE_COUNT)][static_cast<int>(EntityStatePlayer::STATE_COUNT)];
	// L07 DONE 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void OnEndCollision(PhysBody* physA, PhysBody* physB);
	pugi::xml_node find_child_by_attribute(pugi::xml_node parent, const char* name, const char* attr_name, const char* attr_value);

	Entity* GetEnemyWithHighestHealthWithinRadius(iPoint position, int radius);

	//Funciones ataques mascara
	void CastLightning();
	void CastMultipleLightnings();
	void AreaAttack(float dt);
	void DashAttack(float dt);

	//Slow
	void SlowDown(float slowFactor);
	void ResetSpeed();

	//Mascaras
	void UnequipMasks();
	void ChangeMask();
	void EquipPrimaryMask(Mask mask);
	void EquipSecondaryMask(Mask mask);
	void ApplyPoison(Entity* entity);
	void SetInvisible();
	void SetPassiveInvisible();
	void Mask3Statistics();



	Mask GetPrimaryMask();
	Mask GetSecondaryMask();

	MaskStats* GetMaskStats(Mask mask);

	void UpdateStats();

	bool PlayerTimerColdDown(float time);


	//Estadísticas

	Stats baseStats;

	Stats currentStats{10,10,10,10,10};

	std::map<int, PassiveStats> branchPassiveStats;
	std::map<Branches, decltype(branchPassiveStats)> maskBranchPassiveStats;
	std::map<Mask, decltype(maskBranchPassiveStats)> passiveStats;

	std::map<int, MaskStats> branchLevels;
	std::map<Branches, decltype(branchLevels)> maskBranches;
	std::map<Mask, decltype(maskBranches)> maskStats;

	std::map<Mask, std::map<Branches, int>> maskLevels;

	//MasksLevel
	int maskZeroXP;
	int maskOneXP;
	int maskTwoXP;
	int maskThreeXP;

	int maskZeroLevel;
	int maskOneLevel;
	int maskTwoLevel;
	int maskThreeLevel;

	int maxMaskLevel = 8;

	int matrizLevelXP[8] = { 0, 800, 1200, 600, 900, 1500, 600, 1400 };

	int XPtoLevelUpZero = 1000;
	int XPtoLevelUpOne = 1000;
	int XPtoLevelUpTwo = 1000;
	int XPtoLevelUpThree = 1000;

	int maskZeroPoints;
	int maskOnePoints;
	int maskTwoPoints;
	int maskThreePoints;

	bool levelUpZero= false;
	bool levelUpOne = false;
	bool levelUpTwo = false;
	bool levelUpThree = false;

	Timer timerLevelUpZero;
	Timer timerLevelUpOne;
	Timer timerLevelUpTwo;
	Timer timerLevelUpThree;

	bool timerLevelUpZeroStarted = false;
	bool timerLevelUpOneStarted = false;
	bool timerLevelUpTwoStarted = false;
	bool timerLevelUpThreeStarted = false;

	bool playerTpBossPadre = false;

private:
	void CameraMovement(float dt);
	void GodMode(float dt);
	void PlayerMovement(float dt);
	void FishingDirecction(float verticalMovement, float horizontalMovement);
	void MaskAttack(float dt);

	void stateMachine(float dt);

public:

	
	//L02: DONE 2: Declare player parameters
	float speed = 0.2f;
	float attackMovement = 25;
	SDL_Texture* texture = NULL;

	uint texW, texH;

	//Audio fx
	int run_fx;
	int runAlt_fx;
	int runAlt2_fx;
	int dash_fx;
	int switch_masks_fx;
	int basic_combo_attack1_fx;
	int basic_combo_attack2_fx;
	int basic_combo_attack3_fx;
	int basic_combo_attack3Alt_fx;
	int basic_combo_attack3Alt2_fx;
	int player_get_damage_fx;
	int player_death_fx;
	int use_potion_fx;
	int get_item_fx;

	Animation* currentAnimation = nullptr;
	EntityStatePlayer state;

	float TimeClodDown;
	Timer TimerColdDown;

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
	b2Vec2 velocityNormalized;

	float speedDash = 3;

	//Ataque
	int attackWidth = 85;
	int attackHeight = 85;
	bool isAttacking = false;
	PhysBody* attackSensor = nullptr;
	Timer damageTimer;
	float damageCooldown = 0.2f;
	bool hasAttacked = false;
	bool hasMaskAttacked = false;
	int atackNum = 0;
	bool checkAtk = false;
	//Dead
	bool DeadTP = false;
	bool coinsRemoved = false;

	//TakeDMG
	bool inTakeDMG = false;

	//Animacion Mascara ataque
	bool inMaskAtack;

	//pocion
	bool inPocionAnim = false;
	b2Vec2 posInicioPlayer = b2Vec2(0, 0);
	bool vacio = false;
	bool insideVacio = false;

	Timer collisionAttackTimer;

	//Espada & Armadura
	int attackDamagePerLevel[10];
	int armorPerLevel[10];

	// int swordLevel = 1;
	// int armorLevel = 1;

	//POCIONES
	//Regen
	Timer regenTimer;
	int regenAmount = 0;
	int totalRegen = 0;

	//Damage
	Timer potiondamageTimer;
	float originalDamage;
	bool damagePotionActive = false;

	//Speed
	Timer potionspeedTimer;
	float originalSpeed;
	bool speedPotionActive = false;

	//Slow speed
	float mainSpeed = 0.2f;
	Timer slowSpeedTimer;
	bool isSlowed = false;

	//Orbe (mascaras)
	Mask currentMask;
	bool maskPotionActive = false;

	//Pasiva de la mascara 1
	PhysBody* mask1PassiveSensor = nullptr;

	//Mascara
	Mask primaryMask = Mask::NOMASK;
	Mask secondaryMask = Mask::NOMASK;


	bool isAttackingMask = false;
	PhysBody* mask1AttackSensor = nullptr;
	int attackMask1Width = 300;
	int attackMask1Height = 300;

	Timer collisionMask1Timer;

	Timer timerChangeMask;

	const int changeMaskCooldown = 5000; //5s

	bool isInvisible = false;

	Timer mask3Timer;
	bool mask3Active = false;

	fPoint posMask3Particle;
	Emitter* mask3Particle = nullptr;
	bool mask3ParticleCreated = false;

	//Fishing
	//New//
	bool playermove = false;
	bool getPlayerTouch = false;
	Direction player_Direction;

	//Player move
	bool pressingUp;
	bool pressingDown;
	bool pressingLeft;
	bool pressingRight;

	bool isFacingLeft = false;

	//Camera
	MapObject* GetCurrentRoom();
	RoomType roomType;
	void DetermineRoomType(MapObject* room);
	void DetermineRoomTypel(MapObject* room);

private:
	Animation idle_player;
	Animation run_player;
	Animation dashi_player;
	Animation atack1_player;
	Animation atack2_player;
	Animation atack3_player;
	Animation dead_player;
	Animation pocion_player;
	Animation maskBola_player;
	Animation maskRayo_player;
	Animation takeDMG_player;


	bool isDashing = false;

	bool godmode = false;

	bool camaralibre = false;

	int cdTimerDashMS;

	Timer timerDash;
	Timer cdTimerDash;

	int cdTimerAttackMS;
	Timer timerAttack;

	fPoint lastMovementDirection;

	Timer timerBetweenLightnings;

	//Atack
	bool atack_Anim = false;
	
	Emitter* blood = nullptr;
	bool particulaBlood = false;


	bool dialogoMascara0 = false;
	bool dialogoMascara1 = false;
	bool dialogoMascara2 = false;
	bool dialogoMascara3 = false;

public:

	bool changeLife = false;

	Branch transitionTable[static_cast<int>(EntityStatePlayer::STATE_COUNT)][static_cast<int>(EntityStatePlayer::STATE_COUNT)] = {

		//		IDLE					RUNNING					ATTACKING					DEAD				REVIVING				MASK_ATTACK					  DASHI				   NONE
		{ {EntityStatePlayer::IDLE}, {EntityStatePlayer::RUNNING}, {EntityStatePlayer::ATTACKING}, {EntityStatePlayer::DEAD}, {EntityStatePlayer::REVIVING}, {EntityStatePlayer::MASK_ATTACK}, {EntityStatePlayer::POCION},{EntityStatePlayer::NONE}, {EntityStatePlayer::IDLE}}, // IDLE
		{ {EntityStatePlayer::IDLE}, {EntityStatePlayer::RUNNING}, {EntityStatePlayer::ATTACKING}, {EntityStatePlayer::DEAD}, {EntityStatePlayer::REVIVING}, {EntityStatePlayer::MASK_ATTACK}, {EntityStatePlayer::POCION},{EntityStatePlayer::DASHI}, {EntityStatePlayer::IDLE}}, // RUNNING
		{ {EntityStatePlayer::IDLE}, {EntityStatePlayer::RUNNING}, {EntityStatePlayer::ATTACKING}, {EntityStatePlayer::DEAD}, {EntityStatePlayer::REVIVING}, {EntityStatePlayer::MASK_ATTACK}, {EntityStatePlayer::NONE},{EntityStatePlayer::DASHI}, {EntityStatePlayer::IDLE}}, // ATTACKING
		{ {EntityStatePlayer::NONE}, {EntityStatePlayer::NONE}, {EntityStatePlayer::NONE}, {EntityStatePlayer::DEAD}, {EntityStatePlayer::REVIVING}, {EntityStatePlayer::NONE}, {EntityStatePlayer::NONE},{EntityStatePlayer::NONE}, {EntityStatePlayer::IDLE}}, // DEAD
		{ {EntityStatePlayer::IDLE}, {EntityStatePlayer::RUNNING}, {EntityStatePlayer::ATTACKING}, {EntityStatePlayer::DEAD}, {EntityStatePlayer::REVIVING}, {EntityStatePlayer::MASK_ATTACK},{EntityStatePlayer::NONE}, {EntityStatePlayer::DASHI}, {EntityStatePlayer::IDLE}}, // REVIVING
		{ {EntityStatePlayer::IDLE}, {EntityStatePlayer::RUNNING}, {EntityStatePlayer::ATTACKING}, {EntityStatePlayer::DEAD}, {EntityStatePlayer::REVIVING}, {EntityStatePlayer::MASK_ATTACK}, {EntityStatePlayer::NONE},{EntityStatePlayer::DASHI}, {EntityStatePlayer::IDLE}}, // MASK_ATTACK
		{ {EntityStatePlayer::IDLE}, {EntityStatePlayer::RUNNING}, {EntityStatePlayer::ATTACKING}, {EntityStatePlayer::DEAD}, {EntityStatePlayer::REVIVING}, {EntityStatePlayer::MASK_ATTACK}, {EntityStatePlayer::POCION},{EntityStatePlayer::DASHI}, {EntityStatePlayer::IDLE}}, // POCION
		{ {EntityStatePlayer::IDLE}, {EntityStatePlayer::RUNNING}, {EntityStatePlayer::ATTACKING}, {EntityStatePlayer::DEAD}, {EntityStatePlayer::REVIVING}, {EntityStatePlayer::MASK_ATTACK}, {EntityStatePlayer::NONE},{EntityStatePlayer::DASHI}, {EntityStatePlayer::IDLE}}, // DASHI
		{ {EntityStatePlayer::IDLE}, {EntityStatePlayer::NONE}, {EntityStatePlayer::NONE}, {EntityStatePlayer::NONE}, {EntityStatePlayer::NONE}, {EntityStatePlayer::NONE},{EntityStatePlayer::NONE}, {EntityStatePlayer::NONE}, {EntityStatePlayer::IDLE}} // NONE
	};

	/*EntityStatePlayer currentState = state;
	EntityStatePlayer desiredState = nextState;
	EntityStatePlayer nextState = transitionTable[static_cast<int>(currentState)][static_cast<int>(desiredState)].next_state;*/
	EntityStatePlayer currentState;
	EntityStatePlayer desiredState;
	EntityStatePlayer nextState;

	bool die = false;
};



#endif // __PLAYER_H__
