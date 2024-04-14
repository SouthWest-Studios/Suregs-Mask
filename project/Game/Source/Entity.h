#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"

enum class EntityType
{
	PLAYER,
	ITEM,
	ENEMY_OSIRIS,
	ENEMY_OLS,
	ENEMY_SHAR,
	DIALOG_TRIGGER,
	TP_ENTITY,
	RESOURCE_CUERNO,
	ITEM_DIAMANTE,
	RESOURCE_ARMADURA,
	NPC_VENDEDOR,
	NPC_PESCADOR,
	NPC_BRUJA,
	NPC_HERRERA,
	NPC_ABUELO,
	NPC_MUJER_PREOCUPADA,
	NPC_MUJER_ENAMORADA,
	NPC_BULLY_1,
	NPC_BULLY_2,
	UNKNOWN
};

class PhysBody;



enum class EntityState
{
	IDLE,
	RUNNING,
	ATTACKING,
	DEAD,
	REVIVING,
	MASK_ATTACK,
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
	virtual void OnExitCollision(PhysBody* physA, PhysBody* physB) {};

	virtual void TakeDamage(float damage) {
		health -= damage;
	}

public:

	SString name;
	EntityType type;
	bool active = true;
	pugi::xml_node parameters; 

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	iPoint position;       
	bool renderable = true;

	bool isAttacking;

	PhysBody* pbody;

	float speed;
	float health;
	float maxHealth;
	float attackDamage;
};

#endif // __ENTITY_H__