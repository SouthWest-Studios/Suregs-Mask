#ifndef __COMMERCE_H__
#define __COMMERCE_H__

#include "Module.h"
#include "Entity.h"
#include "inventity.h"
#include "List.h"
#include "Textures.h"

class Commerce : public Module
{
public:

	Commerce(App* app, bool start_enabled = true);

	// Destructor
	virtual ~Commerce();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	

public:





private:


	
};

#endif // __COMMERCE_H__
