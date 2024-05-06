#ifndef __NOTE_H__
#define __NOTE_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"

enum class NoteType
{
	
	NOTE, 
	UNKNOWN//<-- Salvo este, el ultimo

};

class PhysBody;

class Note
{
public:

	Note(NoteType type) : type(type), active(true) {}
	
	Note() : active(true) {}

	

	

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

	void Note::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Note::Disable()
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
	SDL_Texture* closeUpNotes;
	char* closeUpNotesPath;
	uint quantity = 1;
	bool active;
	bool stackable = false;
	NoteType type;
	std::string desc;
	std::string title;
	std::string tipo;
	bool zoom = false;

	
};

#endif // __INVENTITY_H__