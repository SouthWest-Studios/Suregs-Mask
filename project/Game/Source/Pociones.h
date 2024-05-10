#ifndef __POCIONES_H__
#define __POCIONES_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"

enum class PocionesType
{
	POCION_VIDA_1,
	POCION_VIDA_2,
	POCION_VIDA_3,
	POCION_VIDA_MAX,
	POCION_REGENERACION,
	POCION_DANO,
	POCION_VELOCIDAD,
	ORBE_MAGICO,

	//De aqui para abajo no importa el orden
	
	UNKNOWN //<-- Salvo este, el ultimo

};

class PhysBody;

class Pociones
{
public:

	Pociones(PocionesType type) : type(type), active(true) {}
	
	Pociones() : active(true) {}
	

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

	void Pociones::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Pociones::Disable()
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
	SDL_Texture* closeUpItems;
	char* closeUpItemsPath;
	uint quantity = 1;
	bool active;
	bool stackable = false;
	PocionesType type;
	std::string desc;
	std::string tipo;

	
};

#endif // __POCIONES_H__