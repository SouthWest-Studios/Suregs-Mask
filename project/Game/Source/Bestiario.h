#ifndef __BESTIARIO_H__
#define __BESTIARIO_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"

enum class BestiarioType
{
	
	BEST, 
	UNKNOWN//<-- Salvo este, el ultimo

};

class PhysBody;

class Bestiario
{
public:

	Bestiario(BestiarioType type) : type(type), active(true) {}
	
	Bestiario() : active(true) {}

	

	

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

	void Bestiario::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Bestiario::Disable()
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
	char* iconPath;
	SDL_Texture* closeUpBestiarios;
	char* closeUpBestiariosPath;
	uint quantity = 1;
	bool active;
	bool stackable = false;
	BestiarioType type;
	std::string desc;
	std::string tipo;
	bool zoom = false;

	
};

#endif // __INVENTITY_H__