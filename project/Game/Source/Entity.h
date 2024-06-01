#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"

struct MapObject;

enum class EntityType
{
	PLAYER,
	ITEM,

	ENEMY_GUERRERO,
	ENEMY_OSIRIS,
	ENEMY_OSIRIS_VARIATION,
	ENEMY_OLS,
	ENEMY_OLS_VARIATION,
	ENEMY_SHAR,
	ENEMY_MUUR,
	ENEMY_MUUR_VARIATION,
	ENEMY_KHURT,
	ENEMY_KHURT_VARIATION,
	ENEMY_BOOROK,
	ENEMY_BOOROK_VARIATION,
	ENEMY_SPAWNER,
	ARO_MAGICA,

	DIALOG_TRIGGER,
	TP_ENTITY,

	PUZZLE_BUTTON,
	PUZZLE_LEVER,
	PUZZLE_BOX,

	ITEM_GARRA,
	ITEM_DIAMANTE,
	ITEM_OJO,
	ITEM_ARMADURA,
	ITEM_VISCERAS,
	ITEM_RUBI,
	ITEM_POLVORA,
	ITEM_NOTA,
	ITEM_HUESO,
	ITEM_DIENTE,
	ITEM_CARBON,
	ITEM_AMATISTA,
	ITEM_COLA,
	ITEM_PEZ_PEQUENO,
	ITEM_PEZ_MEDIANO,
	ITEM_PEZ_GRANDE,
	ITEM_MONEDA,
	ITEM_SALIVA,
	ITEM_POCION_VIDA_1,
	ITEM_POCION_VIDA_2,
	ITEM_POCION_VIDA_3,
	ITEM_POCION_VIDA_MAX,
	ITEM_POCION_REGENERACION,
	ITEM_POCION_DANO,
	ITEM_POCION_VELOCIDAD,
	ITEM_ORBE_MAGICO,
	ITEM_ZAFIRO,
	ITEM_ESPADA_NV2,
	ITEM_ESPADA_NV3,
	ITEM_ESPADA_NV4,
	ITEM_ESPADA_NV5,
	ITEM_ESPADA_NV6,
	ITEM_ESPADA_NV7,
	ITEM_ESPADA_NV8,
	ITEM_ESPADA_NV9,
	ITEM_ESPADA_NV10,
	ITEM_ARMADURA_NV2,
	ITEM_ARMADURA_NV3,
	ITEM_ARMADURA_NV4,
	ITEM_ARMADURA_NV5,
	ITEM_ARMADURA_NV6,
	ITEM_ARMADURA_NV7,
	ITEM_ARMADURA_NV8,
	ITEM_ARMADURA_NV9,
	ITEM_ARMADURA_NV10,
	ITEM_BASURA,
	ITEM_MASCARA0,
	ITEM_MASCARA1,
	ITEM_MASCARA2,
	ITEM_MASCARA3,

	NPC_VENDEDOR,
	NPC_PESCADOR,
	NPC_BRUJA,
	NPC_HERRERA,
	NPC_ABUELA,
	NPC_PADRE,
	NPC_ABUELO,
	NPC_MUJER_PREOCUPADA,
	NPC_MUJER_ENAMORADA,
	NPC_ENAMORADOS,
	NPC_ABUELITO,
	NPC_ABUELITAS,
	NPC_GUARDIAS,
	NPC_BULLY_1,
	NPC_BULLY_2,
	ROD,

	BOSS_INUIT,
	BOSS_MUSRI,
	BOSS_SURMA,
	BOSS_IGORY,

	COFRE,
	ASCENSOR,
	ESTATUA,

	UNKNOWN

	
};

class PhysBody;



enum class EntityStatePlayer
{
	IDLE,
	RUNNING,
	ATTACKING,
	DEAD,
	REVIVING,
	MASK_ATTACK,
	POCION,
	DASHI,
	NONE,
	STATE_COUNT
};

enum class EntityState
{
	IDLE,
	RUNNING,
	ATTACKING,
	DEAD,
	REVIVING,
	MASK_ATTACK,
	DASHI,
	NONE,
	STATE_COUNT
};


enum class EntityState_Enemy
{
	IDLE,
	RUNNING,
	ATTACKING,
	DEAD,
	REVIVING,
	DASHI,
	NONE,
	STATE_COUNT
};

enum class EntityState_Boss_Inuit
{
	IDLE,
	RUNNING,
	ATTACKING_BASIC,
	ATTACKING_DISTANCE,
	DEAD,
	DASHI,
	FASE_CHANGE,
	NONE,
	STATE_COUNT
};

enum class EntityState_Boss_Igory
{
	IDLE,
	RUNNING,
	ATTACKING_BASIC,
	ATTACKING_CHARGE,
	ATTACKING_DASHI,
	DEAD,
	FASE_CHANGE,
	TAKEHIT,
	GENERATESUREG,
	HEAL,
	NONE,
	STATE_COUNT
};

enum class EntityState_Boss_Musri
{
	IDLE,
	RUNNING,
	ATTACKING_BASIC,
	ATTACKING_DISTANCE,
	DEAD,
	DASHI,
	FASE_CHANGE,
	NONE,
	STATE_COUNT
};
enum class EntityState_Boss_Surma
{
	IDLE,
	RUNNING,
	ATTACKING_BASIC,
	ATTACKING_DISTANCE,
	DEAD,
	DASHI,
	FASE_CHANGE,
	NONE,
	STATE_COUNT
};


class Entity
{
public:

	Entity(EntityType type) : type(type), active(true) {}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool PostUpdate() {
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&)
	{
		return true;
	}

	void Entity::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Entity::Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	virtual void OnCollision(PhysBody* physA, PhysBody* physB) {};
	virtual void OnEndCollision(PhysBody* physA, PhysBody* physB) {};
	virtual void OnExitCollision(PhysBody* physA, PhysBody* physB) {};

	virtual float GetHealth() const {
		return health;
	}

	virtual void TakeDamage(float damage) {
		health -= damage;
	}

	virtual void ApplyPoison(int poisonDamage, float poisonDuration, float poisonTickRate) {
		this->poisonDamage = poisonDamage;
		this->poisonDuration = poisonDuration;
		this->poisonTickRate = poisonTickRate;
	}


protected:
	int poisonDamage;
	float poisonDuration;
	float poisonTickRate;
	float poisonTimer = 0.0f;
	float timeSinceLastTick = 0.0f;

public:

	std::string name;
	EntityType type;
	bool active = true;
	pugi::xml_node parameters; 
	pugi::xml_node config;

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	iPoint position;       
	bool renderable = true;

	bool isAttacking;

	PhysBody* pbodyFoot;
	PhysBody* pbodySensor;
	

	float speed;
	float health;
	float maxHealth;
	float attackDamage;
	float secondAttackDamage;

	bool playerInBossArea = false;
	std::string description;
	std::string titulo;
	std::string tipo;

	SDL_Texture* CloseTexture = NULL;

	bool plused = false;

	MapObject* room = nullptr;
	bool isActive = true;
	bool isOpened = false;
	int id;
	bool primeraMuerte = false;
};

#endif // __ENTITY_H__