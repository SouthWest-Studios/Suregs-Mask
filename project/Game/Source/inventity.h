#ifndef __INVENTITY_H__
#define __INVENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"

enum class InventityType
{
	ITEM,
	ESPADA,
	ESPADA2,
	ARMADURA,
	UNKNOWN
};

class PhysBody;

class Inventity
{
public:

	Inventity(InventityType type) : type(type), active(true) {}

	

	

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

	virtual bool PostUpdate(float dt)
	{
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

	void Inventity::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Inventity::Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	virtual void OnCollision(PhysBody* physA, PhysBody* physB) {

	};



public:

	int id;
	SString name;
	SDL_Texture* icon;
	uint quantity = 1;
	bool active;
	bool stackable = false;
	InventityType type;
	
	

	
};

#endif // __INVENTITY_H__