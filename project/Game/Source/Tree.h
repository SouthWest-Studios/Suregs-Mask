#ifndef __TREE_H__
#define __TREE_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"

enum class TreeType
{
	
	MASK0,
	MASK1,
	MASK2,
	MASK3,
	BUTTON,
	UNKNOWN//<-- Salvo este, el ultimo

};

class PhysBody;

class Tree
{
public:

	Tree(TreeType type) : type(type), active(true) {}
	
	Tree() : active(true) {}

	

	

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

	void Tree::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Tree::Disable()
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
	SString name2 = "";
	SString name3 = "";
	SString name4 = "";
	SDL_Texture* icon;
	char* iconPath;
	SDL_Texture* closeUpTrees;
	char* closeUpTreesPath;
	uint quantity = 1;
	bool active;
	bool stackable = false;
	TreeType type;
	std::string desc;
	std::string desc1 = "";
	std::string desc2 = "";
	std::string desc3 = "";
	std::string tipo;
	bool zoom = false;
	int nivelArbol;
	int nivelMejora;
	bool usable1 = false;
	bool usable2 = false;
	bool usable3 = false;
	bool usable4 = false;

	bool used1 = false;
	bool used2 = false;
	bool used3 = false;
	bool used4 = false;

	SDL_Texture* icon0 = nullptr;
	SDL_Texture* icon1 = nullptr;
	SDL_Texture* icon2 = nullptr;
	SDL_Texture* icon3 = nullptr;

	SDL_Texture* iconSelected0 = nullptr;
	SDL_Texture* iconSelected1 = nullptr;
	SDL_Texture* iconSelected2 = nullptr;
	SDL_Texture* iconSelected3 = nullptr;

	SDL_Texture* iconLocked0 = nullptr;
	SDL_Texture* iconLocked1 = nullptr;
	SDL_Texture* iconLocked2 = nullptr;
	SDL_Texture* iconLocked3 = nullptr;

	
};

#endif // __INVENTITY_H__