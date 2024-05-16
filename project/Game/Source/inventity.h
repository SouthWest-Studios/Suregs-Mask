#ifndef __INVENTITY_H__
#define __INVENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"

enum class InventityType
{
	MONEDA = -1,
	ITEM,
	GARRA,
	OJO,
	DIAMANTE,
	VISCERA,
	RUBI,
	POLVORA,
	NOTA,
	HUESO,
	DIENTE,
	COLA,
	CARBON,
	AMATISTA,
	ESPADA2,
	ARMADURA,
	PEZ_PEQUENO,
	PEZ_MEDIANO,
	PEZ_GRANDE,
	SALIVA,
	POCION_VIDA_1,
	POCION_VIDA_2,
	POCION_VIDA_3,
	POCION_VIDA_MAX,
	POCION_REGENERACION,
	POCION_DANO,
	POCION_VELOCIDAD,
	ORBE_MAGICO,
	ZAFIRO,
	 ESPADA_NV2,
	 ESPADA_NV3,
	 ESPADA_NV4,
	 ESPADA_NV5,
	 ESPADA_NV6,
	 ESPADA_NV7,
	 ESPADA_NV8,
	 ESPADA_NV9,
	 ARMADURA_NV2,
	 ARMADURA_NV3,
	 ARMADURA_NV4,
	 ARMADURA_NV5,
	 ARMADURA_NV6,
	 ARMADURA_NV7,
	 ARMADURA_NV8,
	 ARMADURA_NV9,

	//De aqui para abajo no importa el orden
	
	UNKNOWN //<-- Salvo este, el ultimo

};

class PhysBody;

class Inventity
{
public:

	Inventity(InventityType type) : type(type), active(true) {}
	
	Inventity() : active(true) {}

	

	

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
	int objectID = -1;
	SString name;
	SDL_Texture* icon;
	char* iconPath;
	SDL_Texture* closeUpItems;
	char* closeUpItemsPath;
	uint quantity = 1;
	bool active;
	bool stackable = false;
	bool inList = false;
	InventityType type;
	std::string desc;
	std::string tipo;

	
};

#endif // __INVENTITY_H__