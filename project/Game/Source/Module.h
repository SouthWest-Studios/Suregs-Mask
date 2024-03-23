#ifndef __MODULE_H__
#define __MODULE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"

class App;
class GuiControl;

class Module
{
public:

	App* app;

	Module(App* parent, bool start_enabled = true) : app(parent), active(start_enabled)
	{}

	void Enable()
	{
		if (active == false)
		{
			active = true;
			Start();
		}
	}

	void Disable()
	{
		if (active == true)
		{
			active = false;
			CleanUp();
		}
	}

	void Init()
	{
		active = true;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node config)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostLateUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	// L14: TODO 2: Create new virtual methods to LoadState / SaveState

	// Called when we want to load data from XML
	virtual bool LoadState(pugi::xml_node node)
	{
		return true;
	}

	// Called when we want to save data from XML
	virtual bool SaveState(pugi::xml_node node)
	{
		return true;
	}

	virtual bool OnGuiMouseClickEvent(GuiControl* control)
	{
		return true;
	}

public:

	SString name;
	bool sceneReload = false;
	bool active;

};

#endif // __MODULE_H__